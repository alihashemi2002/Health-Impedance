#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H

#include <xc.h>

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define hBeat_Pin()                   (LATGbits.LATG9)                          // hBeat pin
#define hBeat_SetHigh()               (hBeat_Pin() = 1)                         // Sets hBeat signal high
#define hBeat_SetLow()                (hBeat_Pin() = 0)                         // Sets hBeat signal low
#define hBeat_Toggle()                (hBeat_Pin() ^= 1)                        // Toggles hBeat signal

#define tiaEn_Pin()                   (LATFbits.LATF1)                          // TIA enable pin
#define tiaEn()                       (tiaEn_Pin() = 1)                         // Sets TIA enable high
#define tiaDis()                      (tiaEn_Pin() = 0)                         // Sets TIA enablel low

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** pinManagerInitialize *********************************************************************************************************/        
/* This routine configures the pins direction, initial state, analog setting. The peripheral pin select, PPS, configuration are also
 * handled by this routine.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void pinManagerInitialize(void);

#endif                                                                          // End _PIN_MANAGER_H