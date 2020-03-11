#ifndef _INTERRUPT_MANAGER_H
#define _INTERRUPT_MANAGER_H

#include <xc.h>

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** interruptInitialize **********************************************************************************************************/        
/* This routine sets the interrupt priorities of the modules that have been configured for the PIC32MZ1024EFG064
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void interruptInitialize(void);

#endif                                                                          // End _INTERRUPT_MANAGER_H