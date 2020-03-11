#include "ADG731_2.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
sw2_obj sw2;                                                                    // SW variables

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** sw2SpiInit *******************************************************************************************************************/
void sw2SpiInit(void){
    sw2SpiCon()  = 0;                                                           // Clear register                                                            
    sw2SpiCon2() = 0;                                                           // Clear register     
    sw2SpiCon()  = 0x18020;                                                     // ON; ENH BUF; 8-BIT; SMP=0; CKE=0; CKP=0; MASTER;
    sw2SpiBrg()  = 0x0003;                                                      // PB2CLK / 8 (6.25MHz)
}//end sw2SpiInit()

/*** sw2SpiRxFifoClear ************************************************************************************************************/
void sw2SpiRxFifoClear(void){
    uint8_t var;
    while(!sw2SpiRxBufEmpty()) var = sw2SpiBuf();                               // Read SW SPI RX FIFO
}//end sw2SpiRxFifoClear()

/*** sw2Initialize ****************************************************************************************************************/
void sw2Initialize(void){        
    
    sw2SpiCloseFrame();                                                         // Close SPI frame
    sw2SpiInit();                                                               // Configure SPI port
    
    sw2.txEnd   = 0;                                                            // Initialize TX end index
    sw2.txStart = 0;                                                            // Initialize TX start index
    
    sw2.actv    = sw2Min();                                                     // Initialize active switch to 0
    sw2.state   = sw2WriteState();                                              // Initialize state machine to write state
}//end sw2Initialize()

/*** sw2Sel ***********************************************************************************************************************/
bool sw2Sel(uint8_t swNum){
    if(swNum > sw2Max())  return false;                                         // Switch number requested is invalid--exit
    if(sw2.actv == swNum) return false;                                         // Switch number requested is currently active--exit
    sw2.tx[sw2.txEnd++] = swNum;                                                // Requested sw2itch to buffer
}//end sw2Sel

/*** sw2TxBytes *******************************************************************************************************************/
uint8_t sw2TxBytes(void){return ((uint8_t)(sw2.txEnd - sw2.txStart));}

/*** sw2Service *******************************************************************************************************************/
void sw2Service(void){
    switch(sw2.state){
        case sw2WriteState():
            if(sw2TxBytes() < 1) break;                                         // Nothing to write--exit 
            sw2SpiOpenFrame();                                                  // Open SPI frame
            sw2SpiBuf() = sw2.tx[sw2.txStart++];                                // Write byte
            while(sw2SpiBusy());                                                // Wait for SPI transaction to finish
            sw2SpiCloseFrame();                                                 // Close SPI frame
            sw2SpiRxFifoClear();                                                // Clear RX FIFO
            break;                                                              // Exit
        default:
            sw2SpiCloseFrame();                                                 // Close SPI frame
            sw2SpiRxFifoClear();                                                // Clear RX FIFO
            sw2.state = sw2WriteState();                                        // Set run state to write state
            break;                                                              // Exit
    }//end switch
}//end sw2Service()