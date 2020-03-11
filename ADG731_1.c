#include "ADG731_1.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
sw1_obj sw1;                                                                    // SW variables

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** sw1SpiInit *******************************************************************************************************************/
void sw1SpiInit(void){
    sw1SpiCon()  = 0;                                                           // Clear register                                                            
    sw1SpiCon2() = 0;                                                           // Clear register     
    sw1SpiCon()  = 0x18020;                                                     // ON; ENH BUF; 8-BIT; SMP=0; CKE=0; CKP=0; MASTER;
    sw1SpiBrg()  = 0x0003;                                                      // PB2CLK / 8 (6.25MHz)
}//end sw1SpiInit()

/*** sw1SpiRxFifoClear ************************************************************************************************************/
void sw1SpiRxFifoClear(void){
    uint8_t var;
    while(!sw1SpiRxBufEmpty()) var = sw1SpiBuf();                               // Read SW SPI RX FIFO
}//end sw1SpiRxFifoClear()

/*** sw1Initialize ****************************************************************************************************************/
void sw1Initialize(void){        
    
    sw1SpiCloseFrame();                                                         // Close SPI frame
    sw1SpiInit();                                                               // Configure SPI port
    
    sw1.txEnd   = 0;                                                            // Initialize TX end index
    sw1.txStart = 0;                                                            // Initialize TX start index
    
    sw1.actv    = sw1Min();                                                     // Initialize active switch to 0
    sw1.state   = sw1WriteState();                                              // Initialize state machine to write state
}//end sw1Initialize()

/*** sw1Sel ***********************************************************************************************************************/
bool sw1Sel(uint8_t swNum){
    if(swNum > sw1Max())  return false;                                         // Switch number requested is invalid--exit
    if(sw1.actv == swNum) return false;                                         // Switch number requested is currently active--exit
    sw1.tx[sw1.txEnd++] = swNum;                                                // Requested sw1itch to buffer
}//end sw1Sel

/*** sw1TxBytes *******************************************************************************************************************/
uint8_t sw1TxBytes(void){return ((uint8_t)(sw1.txEnd - sw1.txStart));}

/*** sw1Service *******************************************************************************************************************/
void sw1Service(void){
    switch(sw1.state){
        case sw1WriteState():
            if(sw1TxBytes() < 1) break;                                         // Nothing to write--exit 
            sw1SpiOpenFrame();                                                  // Open SPI frame
            sw1SpiBuf() = sw1.tx[sw1.txStart++];                                // Write byte
            while(sw1SpiBusy());                                                // Wait for SPI transaction to finish
            sw1SpiCloseFrame();                                                 // Close SPI frame
            sw1SpiRxFifoClear();                                                // Clear RX FIFO
            break;                                                              // Exit
        default:
            sw1SpiCloseFrame();                                                 // Close SPI frame
            sw1SpiRxFifoClear();                                                // Clear RX FIFO
            sw1.state = sw1WriteState();                                        // Set run state to write state
            break;                                                              // Exit
    }//end switch
}//end sw1Service()