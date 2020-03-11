#include "interrupt_manager.h"

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** interruptInitialize **********************************************************************************************************/
void interruptInitialize (void){
    INTCONbits.MVEC  = 1;                                                       // Enable multivector interrupts
    IPC3bits.INT2IP  = 7;                                                       // Set external interupt 2 primary priority
    IPC36bits.U2RXIP = 6;                                                       // Set UART 2 RX interrupt primary priority
    IPC1bits.T1IP    = 5;                                                       // Set timer 1 interrupt primary priority
    asm volatile("ei");                                                         // Enable all interrupts
}//end interruptInitialize()
