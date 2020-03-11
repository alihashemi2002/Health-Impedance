#include "ADG731_4.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
sw4_obj sw4;                                                                    // SW variables

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** sw4SpiInit *******************************************************************************************************************/
void sw4SpiInit(void){
    sw4SpiCon()  = 0;                                                           // Clear register                                                            
    sw4SpiCon2() = 0;                                                           // Clear register     
    sw4SpiCon()  = 0x18020;                                                     // ON; ENH BUF; 8-BIT; SMP=0; CKE=0; CKP=0; MASTER;
    sw4SpiBrg()  = 0x0003;                                                      // PB2CLK / 8 (6.25MHz)
}//end sw4SpiInit()

/*** sw4SpiRxFifoClear ************************************************************************************************************/
void sw4SpiRxFifoClear(void){
    uint8_t var;
    while(!sw4SpiRxBufEmpty()) var = sw4SpiBuf();                               // Read SW SPI RX FIFO
}//end sw4SpiRxFifoClear()

/*** sw4Initialize ****************************************************************************************************************/
void sw4Initialize(void){        
    
    sw4SpiCloseFrame();                                                         // Close SPI frame
    sw4SpiInit();                                                               // Configure SPI port
    
    sw4.txEnd   = 0;                                                            // Initialize TX end index
    sw4.txStart = 0;                                                            // Initialize TX start index
    
    sw4.actv    = sw4Min();                                                     // Initialize active switch to 0
    sw4.state   = sw4WriteState();                                              // Initialize state machine to write state
}//end sw4Initialize()

/*** sw4Sel ***********************************************************************************************************************/
bool sw4Sel(uint8_t swNum){
    if(swNum > sw4Max())  return false;                                         // Switch number requested is invalid--exit
    if(sw4.actv == swNum) return false;                                         // Switch number requested is currently active--exit
    sw4.tx[sw4.txEnd++] = swNum;                                                // Requested sw4itch to buffer
}//end sw4Sel

/*** sw4TxBytes *******************************************************************************************************************/
uint8_t sw4TxBytes(void){return ((uint8_t)(sw4.txEnd - sw4.txStart));}

/*** sw4Service *******************************************************************************************************************/
void sw4Service(void){
    switch(sw4.state){
        case sw4WriteState():
            if(sw4TxBytes() < 1) break;                                         // Nothing to write--exit 
            sw4SpiOpenFrame();                                                  // Open SPI frame
            sw4SpiBuf() = sw4.tx[sw4.txStart++];                                // Write byte
            while(sw4SpiBusy());                                                // Wait for SPI transaction to finish
            sw4SpiCloseFrame();                                                 // Close SPI frame
            sw4SpiRxFifoClear();                                                // Clear RX FIFO
            break;                                                              // Exit
        default:
            sw4SpiCloseFrame();                                                 // Close SPI frame
            sw4SpiRxFifoClear();                                                // Clear RX FIFO
            sw4.state = sw4WriteState();                                        // Set run state to write state
            break;                                                              // Exit
    }//end switch
}//end sw4Service()