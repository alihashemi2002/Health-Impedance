#include "extInt.h"

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** extInt2Interrupt *************************************************************************************************************/        
void __ISR(_EXTERNAL_2_VECTOR, IPL7AUTO) extInt2Interrupt(void){
    adcService();                                                               // Service ADC
    extInt2FlagClr();                                                           // Clear IRQ flag
}//end extInt2Interrupt()


/*** extInt2Initialize ************************************************************************************************************/        
void extInt2Initialize(void){
    extInt2FlagClr();                                                           // Clear ISR flag
    extInt2SetNegEdge();                                                        // Select falling edge as the IRQ edge
    extInt2En();                                                                // Enable EXT_INT2
}//end extInt2Initialize()