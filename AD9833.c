#include "AD9833.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
       dds_obj     dds              = {.smState = ddsSmStateInitial(),
                                       .txEnd   = 0,
                                       .txStart = 0,
                                       .writing = 0};                                                         
static ddsReg_obj  ddsReg           = {.cntrl   = ddsCntrlRstDflt(),
                                       .freq0   = ddsFreq0Dflt(), 
                                       .freq1   = ddsFreq1Dflt(),
                                       .phase0  = ddsPhase0Dflt(),
                                       .phase1  = ddsPhase1Dflt()};
static ddsDone_obj ddsDone          = {.cntrl   = false,
                                       .freq0   = false,
                                       .freq1   = false,
                                       .phase0  = false,
                                       .phase1  = false};

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** ddsSpiInit ******************************************************************************************************************/
void ddsSpiInit(void){
    ddsSpiCon()  = 0;                                                           // Clear register                                                            
    ddsSpiCon2() = 0;                                                           // Clear register     
    ddsSpiCon()  = 0x18360;                                                     // ON; ENH BUF; 8-BIT; SMP=1; CKE=1; CKP=1; MASTER;
    ddsSpiBrg()  = 0x0018;                                                      // PB2CLK / 25 (1MHz)
}//end ddsSpiInit()

/*** ddsRxFifoClear ***************************************************************************************************************/ 
void ddsRxFifoClear(void){
    uint8_t temp;
    while(!ddsSpiRxFifoEmpty()) {temp = ddsSpiBuf();}
}//end ddsRxFifoClear()

/*** ddsInitialize ****************************************************************************************************************/
void ddsInitialize(void){
    ddsSpiCloseFrame();                                                         // Close SPI frame
    ddsSpiInit();                                                               // Initialize SPI bus for DDS use
        
    ddsWrCntrlReg();                                                            // Write register
    ddsWrFreq0Reg();                                                            // Write register
//    ddsWrFreq1Reg();                                                            // Write register
    ddsWrPhase0Reg();                                                           // Write register    
//    ddsWrPhase1Reg();                                                           // Write register     
    ddsReg.cntrl  = ddsCntrlDflt();                                             // Write default to register mirror
    ddsWrCntrlReg();                                                            // Write control register
}//end ddsInitialize()

/*** ddsFlagSet *******************************************************************************************************************/
void ddsFlagSet(void){
    switch(dds.writing){
        case ddsWrAddrCntrl() : ddsDone.cntrl  = true; break;                   // Control register write is done--exit
        case ddsWrAddrFreq0() : ddsDone.freq0  = true; break;                   // Frequency 0 register write is done--exit
        case ddsWrAddrFreq1() : ddsDone.freq1  = true; break;                   // Frequency 1 register write is done--exit
        case ddsWrAddrPhase0(): ddsDone.phase0 = true; break;                   // Phase 0 register write is done--exit
        case ddsWrAddrPhase1(): ddsDone.phase1 = true; break;                   // Phase 1 register write is done--exit
        default:                                       break;                   // Exit switch
    }//end switch
}//end ddsFlagSet()

/*** ddsWrCntrlReg ****************************************************************************************************************/
void ddsWrCntrlReg(void){
    ddsDone.cntrl       =  false;                                               // Clear done flag
    dds.tx[dds.txEnd++] =  ddsWrAddrCntrl();                                    // Register identifier to buffer    
    dds.tx[dds.txEnd++] =  ddsReg.cntrl >> 8;                                   // Write MSB
    dds.tx[dds.txEnd++] =  ddsReg.cntrl;                                        // Write LSB
}//end ddsWrCntrlReg()

/*** ddsWrFreq0Reg ****************************************************************************************************************/
void ddsWrFreq0Reg(void){
    ddsDone.freq0       =  false;                                               // Clear done flag
    dds.tx[dds.txEnd++] =  ddsWrAddrFreq0();                                    // Register identifier to buffer
    dds.tx[dds.txEnd++] =  ddsReg.freq0 >> 8;                                   // Write MSB of LSB
    dds.tx[dds.txEnd++] =  ddsReg.freq0;                                        // Write LSB of LSB
    dds.tx[dds.txEnd++] =  ddsReg.freq0 >> 24;                                  // Write MSB of MSB
    dds.tx[dds.txEnd++] =  ddsReg.freq0 >> 16;                                  // Write LSB of MSB
}//end ddsWrFreq0Reg()

/*** ddsWrFreq1Reg ****************************************************************************************************************/
void ddsWrFreq1Reg(void){
    ddsDone.freq1       =  false;                                               // Clear done flag
    dds.tx[dds.txEnd++] =  ddsWrAddrFreq1();                                    // Register identifier to buffer    
    dds.tx[dds.txEnd++] =  ddsReg.freq1 >> 8;                                   // Write MSB of LSB
    dds.tx[dds.txEnd++] =  ddsReg.freq1;                                        // Write LSB of LSB
    dds.tx[dds.txEnd++] =  ddsReg.freq1 >> 24;                                  // Write MSB of MSB
    dds.tx[dds.txEnd++] =  ddsReg.freq1 >> 16;                                  // Write LSB of MSB
}//end ddsWrFreq1Reg()

/*** ddsWrPhase0Reg ***************************************************************************************************************/
void ddsWrPhase0Reg(void){
    ddsDone.phase0      =  false;                                               // Clear done flag
    dds.tx[dds.txEnd++] =  ddsWrAddrPhase0();                                   // Register identifier to buffer    
    dds.tx[dds.txEnd++] =  ddsReg.phase0 >> 8;                                  // Write MSB
    dds.tx[dds.txEnd++] =  ddsReg.phase0;                                       // Write LSB
}//end ddsWrPhase0Reg()

/*** ddsWrPhase1Reg ***************************************************************************************************************/
void ddsWrPhase1Reg(void){
    ddsDone.phase1      =  false;                                               // Clear done flag
    dds.tx[dds.txEnd++] =  ddsWrAddrPhase1();                                   // Register identifier to buffer
    dds.tx[dds.txEnd++] =  ddsReg.phase1 >> 8;                                  // Write MSB
    dds.tx[dds.txEnd++] =  ddsReg.phase1;                                       // Write LSB
}//end ddsWrPhase1Reg()

/*** ddsSetFreq0 ******************************************************************************************************************/
bool ddsSetFreq0(uint32_t freq){
    if(freq > ddsMaxFreq()) return false;                                       // Invalid frequency--exit    
    uint16_t freqLsb;                                                           // Variable to hold LSB of frequency
    uint16_t freqMsb;                                                           // Variable to hold MSB of frequency
    freqLsb  = (freq & 0x3FFF);                                                 // Record LSB
    freqLsb |= ddsAddrFreq0();                                                  // Append address
    freqMsb  = ((freq >> 14) & 0x3FFF);                                         // Record MSB
    freqMsb |= ddsAddrFreq0();                                                  // Append address
    ddsReg.freq0  = 0;                                                          // Reset value
    ddsReg.freq0  = (freqMsb << 16);                                            // Write MSB    
    ddsReg.freq0 |= freqLsb;                                                    // Write LSB
    ddsWrCntrlReg();                                                            // Write control register
    ddsWrFreq0Reg();                                                            // Write frequency 0 registers
    return true;                                                                // Return success
}//end ddsSetFreq0()

/*** ddsSetFreq1 ******************************************************************************************************************/
bool ddsSetFreq1(uint32_t freq){
    if(freq > ddsMaxFreq()) return false;                                       // Invalid frequency--exit    
    uint16_t freqLsb;                                                           // Variable to hold LSB of frequency
    uint16_t freqMsb;                                                           // Variable to hold MSB of frequency
    freqLsb  = (freq & 0x3FFF);                                                 // Record LSB
    freqLsb |= ddsAddrFreq1();                                                  // Append address
    freqMsb  = ((freq >> 14) & 0x3FFF);                                         // Record MSB
    freqMsb |= ddsAddrFreq1();                                                  // Append address
    ddsReg.freq1  = 0;                                                          // Reset value
    ddsReg.freq1  = (freqMsb << 16);                                            // Write MSB    
    ddsReg.freq1 |= freqLsb;                                                    // Write LSB
    ddsWrCntrlReg();                                                            // Write control register
    ddsWrFreq1Reg();                                                            // Write frequency 1 registers
    return true;                                                                // Return success
}//end ddsSetFreq1()

/*** ddsSetPhase0 *****************************************************************************************************************/
bool ddsSetPhase0(uint16_t phase){
    if(phase > ddsMaxPhase()) return false;                                     // Invalid phase--exit
    ddsReg.phase0  = 0;                                                         // Clear register  
    ddsReg.phase0  = phase;                                                     // Record to register mirror
    ddsReg.phase0 |= ddsAddrPhase0();                                           // Write address    
    ddsWrPhase0Reg();                                                           // Write register
    return true;                                                                // Return success
}//end ddsSetPhase0()

/*** ddsSetPhase1 *****************************************************************************************************************/
bool ddsSetPhase1(uint16_t phase){
    if(phase > ddsMaxPhase()) return false;                                     // Invalid phase--exit
    ddsReg.phase1  = 0;                                                         // Clear register  
    ddsReg.phase1  = phase;                                                     // Record to register mirror
    ddsReg.phase1 |= ddsAddrPhase1();                                           // Write address    
    ddsWrPhase1Reg();                                                           // Write register
    return true;                                                                // Return success
}//end ddsSetPhase1()

/*** ddsTxBytes *******************************************************************************************************************/
uint8_t ddsTxBytes(void){return ((uint8_t)(dds.txEnd - dds.txStart));}          // Return number of bytes in the TX buffer

/*** ddsService *******************************************************************************************************************/
void ddsService(void){
    switch(dds.smState){
        case ddsSmStateInitial():
            if(ddsTxBytes() == 0) break;                                        // Nothing to write--exit            
            if((ddsTxBytes() > 4) && 
                    ((dds.tx[dds.txStart] == (ddsWrAddrFreq0()))||
                    (dds.tx[dds.txStart] == (ddsWrAddrFreq1())))){              // Check for frequency data to write
                dds.writing  = dds.tx[dds.txStart++];                           // Record writing register identifier
                ddsSpiOpenFrame();                                              // Open SPI frame
                ddsSpiBuf() = dds.tx[dds.txStart++];                            // Write byte
                ddsSpiBuf() = dds.tx[dds.txStart++];                            // Write byte
                while(ddsSpiBusy());                                            // Wait for SPI transaction to finish
                ddsRxFifoClear();                                               // Clear RX FIFO
                ddsSpiCloseFrame();                                             // Close SPI frame
                ddsSpiOpenFrame();                                              // Open SPI frame
                ddsSpiBuf() = dds.tx[dds.txStart++];                            // Write byte
                ddsSpiBuf() = dds.tx[dds.txStart++];                            // Write byte     
                while(ddsSpiBusy());                                            // Wait for SPI transaction to finish
                ddsRxFifoClear();                                               // Clear RX FIFO
                ddsSpiCloseFrame();                                             // Close SPI frame
                ddsFlagSet();                                                   // Set done flag
            }//end if(ddsTxBytes...)
            else if(ddsTxBytes() > 2){                                          // Check for data to write
                dds.writing  = dds.tx[dds.txStart++];                           // Record writing register identifier
                ddsSpiOpenFrame();                                              // Open SPI frame
                ddsSpiBuf() = dds.tx[dds.txStart++];                            // Write byte
                ddsSpiBuf() = dds.tx[dds.txStart++];                            // Write byte
                while(ddsSpiBusy());                                            // Wait for SPI transaction to finish
                ddsRxFifoClear();                                               // Clear RX FIFO
                ddsSpiCloseFrame();                                             // Close SPI frame
                ddsFlagSet();                                                   // Set done flag
            }//end else if(ddsTxBytes...)
            break;                                                              // Exit
        default:
            dds.smState = ddsSmStateInitial();                                  // Set run state to initial state
            break;                                                              // Exit
    }//end switch
}//end ddsService()