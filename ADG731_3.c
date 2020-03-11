#include "ADG731_3.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
sw3_obj sw3;                                                                    // SW variables

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** sw3SpiInit *******************************************************************************************************************/
void sw3SpiInit(void){
    sw3SpiCon()  = 0;                                                           // Clear register                                                            
    sw3SpiCon2() = 0;                                                           // Clear register     
    sw3SpiCon()  = 0x18020;                                                     // ON; ENH BUF; 8-BIT; SMP=0; CKE=0; CKP=0; MASTER;
    sw3SpiBrg()  = 0x0003;                                                      // PB2CLK / 8 (6.25MHz)
}//end sw3SpiInit()

/*** sw3SpiRxFifoClear ************************************************************************************************************/
void sw3SpiRxFifoClear(void){
    uint8_t var;
    while(!sw3SpiRxBufEmpty()) var = sw3SpiBuf();                               // Read SW SPI RX FIFO
}//end sw3SpiRxFifoClear()

/*** sw3Initialize ****************************************************************************************************************/
void sw3Initialize(void){        
    
    sw3SpiCloseFrame();                                                         // Close SPI frame
    sw3SpiInit();                                                               // Configure SPI port
    
    sw3.txEnd   = 0;                                                            // Initialize TX end index
    sw3.txStart = 0;                                                            // Initialize TX start index
    
    sw3.actv    = sw3Min();                                                     // Initialize active switch to 0
    sw3.state   = sw3WriteState();                                              // Initialize state machine to write state
}//end sw3Initialize()

/*** sw3Sel ***********************************************************************************************************************/
bool sw3Sel(uint8_t swNum){
    if(swNum > sw3Max())  return false;                                         // Switch number requested is invalid--exit
    if(sw3.actv == swNum) return false;                                         // Switch number requested is currently active--exit
    sw3.tx[sw3.txEnd++] = swNum;                                                // Requested sw3itch to buffer
}//end sw3Sel

/*** sw3TxBytes *******************************************************************************************************************/
uint8_t sw3TxBytes(void){return ((uint8_t)(sw3.txEnd - sw3.txStart));}

/*** sw3Service *******************************************************************************************************************/
void sw3Service(void){
    switch(sw3.state){
        case sw3WriteState():
            if(sw3TxBytes() < 1) break;                                         // Nothing to write--exit 
            sw3SpiOpenFrame();                                                  // Open SPI frame
            sw3SpiBuf() = sw3.tx[sw3.txStart++];                                // Write byte
            while(sw3SpiBusy());                                                // Wait for SPI transaction to finish
            sw3SpiCloseFrame();                                                 // Close SPI frame
            sw3SpiRxFifoClear();                                                // Clear RX FIFO
            break;                                                              // Exit
        default:
            sw3SpiCloseFrame();                                                 // Close SPI frame
            sw3SpiRxFifoClear();                                                // Clear RX FIFO
            sw3.state = sw3WriteState();                                        // Set run state to write state
            break;                                                              // Exit
    }//end switch
}//end sw3Service()