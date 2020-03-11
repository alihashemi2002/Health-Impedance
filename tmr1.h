#ifndef _TMR1_H  
#define _TMR1_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/attribs.h>

/**********************************************************************************************************************************/
/*=== SYSTEM ASSIGNMENTS =========================================================================================================*/
/**********************************************************************************************************************************/

/*=== REGISTERS ==================================================================================================================*/
#define tmr1Con()                       T1CON                                   // Timer 1 configuration register
#define tmr1Cntr()                      TMR1                                    // Timer 1 counter register
#define tmr1Period()                    PR1                                     // Timer 1 period register

/*=== INTERRUPTS ==================================================================================================================*/
#define tmr1IntFlagSet()                (IFS0bits.T1IF = 1)                     // Timer 1 interrupt flag Set
#define tmr1IntFlagClr()                (IFS0bits.T1IF = 0)                     // Timer 1 interrupt flag clear
#define tmr1IntEn()                     (IEC0bits.T1IE = 1)                     // Timer 1 interrupt enable
#define tmr1IntDis()                    (IEC0bits.T1IE = 0)                     // Timer 1 intterupt disable

/*=== COMMANDS ===================================================================================================================*/
#define tmr1En()                        (T1CONbits.ON = 1)                      // Timer 1 enable
#define tmr1Dis()                       (T1CONbits.ON = 0)                      // Timer 1 disable

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define tmr1Dis()                       (T1CONbits.ON = 0)                      // Timer 1 disable
#define tmr1CallRate()                  10000                                   // Timer 1 call rate
#define hBeatPeriod()                   (tmr1CallRate()/10)                     // Heart beat LED period

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
typedef struct{
    bool        hbeatTgl;                                                       // Heartbeat LED enable toggle flag
    bool        sysServEn;                                                      // System service enable flag
    uint16_t    hbeatCntr;                                                      // Heartbeat LED counter
} tmr1_obj;

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** tmr1Initialize ***************************************************************************************************************/
/* This routine initializes hardware for the instance of the TMR module. It also initializes all necessary internal data.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void tmr1Initialize(void);

/*** tmr1Period16BitSet ***********************************************************************************************************/
/* This routine updates 16-bit timer period value (PR1).
 *      INPUTS  - unsigned int16 value to write to PR1
 *      RETURNS - NONE
 */
void tmr1Period16BitSet(uint16_t value);

/*** tmr1Period16BitGet ***********************************************************************************************************/
/* This routine returns the 16-bit timer period value (PR1).
 *      INPUTS  - NONE
 *      RETURNS - unsigned int16 (PR1)
 */
uint16_t tmr1Period16BitGet(void);

/*** tmr1Counter16BitSet **********************************************************************************************************/
/* This routine updates the timer's 16-bit counter register (TMR1).
 *      INPUTS  - unsigned int16 value to write to TMR1
 *      RETURNS - NONE
 */
void tmr1Counter16BitSet(uint16_t value);

/*** tmr1Counter16BitGet **********************************************************************************************************/
/* This routine returns the timer's 16-bit counter register (TMR1).
 *      INPUTS  - NONE
 *      RETURNS - unsigned int16 value (TMR1)
 */
uint16_t tmr1Counter16BitGet(void);

/*** tmr1CallBack *****************************************************************************************************************/
/* This routine is called at ISR execution. It should contain any user code that need be executed in real time.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void tmr1CallBack(void);

/*** hBeatTgl *********************************************************************************************************************/
/* This routine returns the heartbeat enable flag status.
 *      INPUTS  - NONE
 *      RETURNS - hBeatToggleEn
 */
bool hBeatTgl(void);

/*** hBeatTglClr ******************************************************************************************************************/
/* This routine clears the heartbeat enable flag status.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void hBeatTglClr(void);

/*** sysServiceOn *****************************************************************************************************************/
/* This routine returns the system service enable flag status.
 *      INPUTS  - NONE
 *      RETURNS - sysSrvcEn
 */
bool sysServiceOn(void);

/*** sysServiceOff ****************************************************************************************************************/
/* This routine clears the system service enable flag status.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void sysServiceOff(void);

/*** tmr1Start ********************************************************************************************************************/
/* This routine enables timer interrupts and starts the timer.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void tmr1Start(void);

/*** tmr1Stop *********************************************************************************************************************/
/* This routine stops the timer and disables timer interrupts.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void tmr1Stop(void);

#endif                                                                          // End _TMR1_H