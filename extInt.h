#ifndef _EXT_INT_H
#define	_EXT_INT_H

#include <xc.h>
#include <sys/attribs.h>

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define extInt2FlagClr()                (IFS0bits.INT2IF = 0)                   // Clear EXT_INT2 IRQ flag
#define extInt2Dis()                    (IEC0bits.INT2IE = 0)                   // Disable EXT_INT2
#define extInt2En()                     (IEC0bits.INT2IE = 1)                   // Enable EXT_INT2
#define extInt2SetNegEdge()             (INTCONbits.INT2EP = 0)                 // Setup EXT_INT2 to use falling edge
#define extInt2SetPosEdge()             (INTCONbits.INT2EP = 1)                 // Setup EXT_INT2 to use rising edge

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** extInt2Initialize ************************************************************************************************************/        
/* Initializes external interrupt 2 for use.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void extInt2Initialize(void);

#endif                                                                          // End _EXT_INT_H

