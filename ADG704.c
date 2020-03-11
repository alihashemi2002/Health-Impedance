#include "ADG704.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
mux_obj mux;                                                                    // Mux variables

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** muxInitialize ****************************************************************************************************************/
void muxInitialize(void){
    muxDis();                                                                   // Disable mux
    muxA0Dis();                                                                 // Signal A off
    muxA1Dis();                                                                 // Signal B off
    muxEn();                                                                    // Enable mux
    mux.inp    = muxInpS1();                                                    // Initialize input to X0
}//end muxInitialize()

/*** muxSetInp ********************************************************************************************************************/
bool muxSetInp(uint8_t inp){
    if((inp < muxMinInp()) || (inp > muxMaxInp())) return false;                // Input mode invalid--return false
    if(mux.inp == inp) return true;
   
    switch(inp){
        case muxInpS1(): muxA0Dis(); muxA1Dis(); break;                         // Set A0 and A1 for S1
        case muxInpS2(): muxA0En();  muxA1Dis(); break;                         // Set A0 and A1 for S2
        case muxInpS3(): muxA0Dis(); muxA1En();  break;                         // Set A0 and A1 for S3
        case muxInpS4(): muxA0En();  muxA1En();  break;                         // Set A0 and A1 for S4
    }//end switch
    
    mux.inp = inp;                                                              // Record input
    return true;                                                                // Operation successful
}//end muxSetInp()

/*** muxInp ***********************************************************************************************************************/
uint8_t muxInp(void){return mux.inp;}                                           // Return currently selected input