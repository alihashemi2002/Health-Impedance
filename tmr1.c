#include "tmr1.h"
#include "pin_manager.h"
/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
static tmr1_obj tmr1;                                                           // Timer 1 object

/*** tmr1Initialize ***************************************************************************************************************/
void tmr1Initialize(void){
    tmr1.hbeatCntr      = 0;                                                    // Clear heartbeat counter
    tmr1.hbeatTgl       = false;                                                // Clear heartbeat toggle enable flag
    tmr1.sysServEn      = false;                                                // Clear system service enable flag
    tmr1Cntr()          = 0x0000;                                               // Clear timer 1 counter
    tmr1Period()        = 0x01F4;                                               // Period = 0.0001s; Frequency = 12MHz; PR1 = Frequency * Period = 500; 
    tmr1Con()           = 0x8000;                                               // TCKPS 1:1; Timer On; TSIDL OFF; TCS FOSC/2; TECS SOSC; TSYNC OFF; TGATE OFF;
    tmr1IntFlagClr();                                                           // Clear IRQ flag
    tmr1IntEn();                                                                // Enable timer 1 interrupts
}//end tmr1Initialize()

/*** tmr1Interrupt ****************************************************************************************************************/
void __ISR(_TIMER_1_VECTOR, IPL5AUTO) tmr1Interrupt(void){
    tmr1CallBack();                                                             // Timer 1 call-back function
    tmr1IntFlagClr();                                                           // Clear IRQ flag
}//end tmr1Interrupt()

/*** tmr1Period16BitSet ***********************************************************************************************************/
void tmr1Period16BitSet(uint16_t value){tmr1Period() = value;}                  // Write to timer 1 period register (PR1)

/*** tmr1Period16BitGet ***********************************************************************************************************/
uint16_t tmr1Period16BitGet(void){return(tmr1Period());}                        // Return timer 1 period register (PR1)

/*** tmr1Counter16BitSet **********************************************************************************************************/
void tmr1Counter16BitSet(uint16_t value){tmr1Cntr() = value;}                   // Write to timer 1 counter (TMR1)

/*** tmr1Counter16BitGet **********************************************************************************************************/
uint16_t tmr1Counter16BitGet(void){return(tmr1Cntr());}                         // Return timer 1 counter (TMR1)

/*** tmr1CallBack *****************************************************************************************************************/
void tmr1CallBack(void){

    // ------ SYSTEM SERVICE --------------------------------------------------------------------------------------------------
    tmr1.sysServEn      = true;                                                 // Enable system service
        
    // ------ HEARTBEAT -------------------------------------------------------------------------------------------------------
    if(tmr1.hbeatCntr > hBeatPeriod()){                                         // Target count reached
        tmr1.hbeatCntr  = 0;                                                    // Clear heartbeat counter
        tmr1.hbeatTgl   = true;                                                 // Limit reached; reset counter and set flag
    }//end if
    else tmr1.hbeatCntr++;                                                      // Increment heartbeat timing counter
    
    tmr1IntFlagClr();                                                           // Clear IRQ flag
}//end tmr1CallBack()

/*** hBeatTgl *********************************************************************************************************************/
bool hBeatTgl(void){return tmr1.hbeatTgl;}                                      // Return heartbeat status flag

/*** hBeatTglClr ******************************************************************************************************************/
void hBeatTglClr(void){tmr1.hbeatTgl = false;}                                  // Reset heartbeat toggle flag

/*** sysServiceOn *****************************************************************************************************************/
bool sysServiceOn(void){return tmr1.sysServEn;}                                 // Return system service enable flag

/*** sysServiceOff ****************************************************************************************************************/
void sysServiceOff(void){tmr1.sysServEn = false;}                               // Clear system service enable flag

/*** tmr1Start ********************************************************************************************************************/
void tmr1Start(void){
    tmr1.hbeatCntr          = 0;                                                // Clear heartbeat counter
    tmr1.hbeatTgl           = false;                                            // Clear heartbeat toggle enable flag
    tmr1.sysServEn          = false;                                            // Clear system service enable flag
    tmr1IntEn();                                                                // Enable timer 1 interrupts
    tmr1En();                                                                   // Start the timer
}//end tmr1Start()

/*** tmr1Stop *********************************************************************************************************************/
void tmr1Stop(void){
    tmr1Dis();                                                                  // Stop timer
    tmr1IntDis();                                                               // Disable interrupt
}//end tmr1Stop()