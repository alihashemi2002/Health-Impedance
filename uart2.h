#ifndef _UART2_H
#define	_UART2_H

#include <xc.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <sys/attribs.h>

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define FPB()                       50000000UL                                  // Prephiral bus clock frequency
#define uart2ParityErr()            0x08                                        // Set bit in status if RX parity error detected (OR)
#define uart2FrameErr()             0x04                                        // Set bit in status if RX frame error detected (OR)
#define uart2Overrun()              0x02                                        // Set bit in status if RX overrun detected (OR)

/*=== REGISTERS ==================================================================================================================*/
#define uart2RegMode()              U2MODE                                      // UxMODE register
#define uart2RegSta()               U2STA                                       // UxSTA register
#define uart2RegRx()                U2RXREG                                     // UxRXREG register
#define uart2RegTx()                U2TXREG                                     // UxTXREG register
#define uart2RegBaud()              U2BRG                                       // UxBRG register

/*=== INTERRUPTS ==================================================================================================================*/
#define uart2RxIntWakeSet()         (U2MODEbits.WAKE = 1)                       // Ux RX interrupt wake up bit
#define uart2RxIntFlagSet()         (IFS4bits.U2RXIF = 1)                       // Ux RX interrupt flag set
#define uart2RxIntFlagClr()         (IFS4bits.U2RXIF = 0)                       // UX RX interrupt flag clear
#define uart2TxIntFlagClr()         (IFS4bits.U2TXIF = 0)                       // UX TX interrupt flag clear
#define uart2RxIntEn()              (IEC4bits.U2RXIE = 1)                       // UX RX interrupt enable
#define uart2RxIntDis()             (IEC4bits.U2RXIE = 0)                       // UX RX interrupt disable
#define uart2TxIntEn()              (IEC4bits.U2TXIE = 1)                       // UX TX interrupt enable
#define uart2TxIntDis()             (IEC4bits.U2TXIE = 0)                       // UX TX interrupt disable

/*=== COMMANDS ===================================================================================================================*/
#define uart2En()                   (uart2RegMode() |= 0x8000)                  // Enable UART peripheral
#define uart2Dis()                  (uart2RegMode() &= 0x7FFF)                  // Enable UART peripheral
#define uart2BrgHigh()              (U2MODEbits.BRGH = 1)                       // UART baud-rate mode high
#define uart2TxEn()                 (uart2RegSta()  |= 0x0400)                  // Enable UART TX
#define uart2TxDis()                (uart2RegSta()  &= 0x1FFFBFF)               // Disable UART TX
#define uart2RxEn()                 (uart2RegSta()  |= 0x1000)                  // Enable UART RX
#define uart2RxDis()                (uart2RegSta()  &= 0x1FFEFFF)               // Disable UART RX
#define uart2TxbFull()              (uart2RegSta()  &  0x000200)                // UART TX buffer full bit (R)
#define uart2TxDone()               (uart2RegSta()  &  0x000100)                // UART transmit done bit (R)
#define uart2RxIdle()               (uart2RegSta()  &  0x000010)                // UART receiver idle bit (R)
#define uart2Perr()                 (uart2RegSta()  &  0x000008)                // UART parity error bit (R)
#define uart2Ferr()                 (uart2RegSta()  &  0x000004)                // UART framing error bit (R)
#define uart2Overflow()             (uart2RegSta()  &  0x000002)                // UART receive overflow bit (R/W)
#define uart2OverflowClr()          (uart2RegSta()  &= 0x1FFFFFD)               // UART clear overflow bit
#define uart2DataInRx()             (uart2RegSta()  &  0x000001)                // UART data present in RX buffer bit (R)


/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
typedef struct{
    volatile uint8_t tx[UINT8_MAX+1];                                           // UART TX soft buffer
    volatile uint8_t rx[UINT8_MAX+1];                                           // UART RX soft buffer
    volatile uint8_t txStart;                                                   // UART TX buffer start index
    volatile uint8_t txEnd;                                                     // UART TX buffer end index
    volatile uint8_t rxStart;                                                   // UART RX buffer start index
    volatile uint8_t rxEnd;                                                     // UART RX buffer end index
} uart2_obj;

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** uart2Initialize **************************************************************************************************************/
/* This subroutine initializes UART2 for use and clears the circular buffer indexes.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */
void uart2Initialize(void);

/*** uart2DmaRxInitialize *********************************************************************************************************/
/* This subroutine initializes UART2 for use with RX DMA and clears the circular buffer indexes.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */
void uart2DmaRxInitialize(void);

/*** uart2NumBytesInRx ************************************************************************************************************/
/* This routine returns the difference between the RX end and RX start indeces for the RX soft buffer
 *      INPUTS  - NONE
 *      RETURNS - UINT8 'bytes' total
 */
uint8_t uart2NumBytesInRx(void);

/*** uart2NumBytesInTx ************************************************************************************************************/
/* This routine returns the difference between the TX end and TX start indeces for the TX soft buffer
 *      INPUTS  - NONE
 *      RETURNS - UINT8 'bytes' total
 */
uint8_t uart2NumBytesInTx(void);

/*** uart2WrIntVar ****************************************************************************************************************/
/* This routine copies integer type variable of specified 'bytes' length to the UART TX soft buffer.
 *      INPUTS  - integer type 'data'
 *              - UINT8 'bytes' length
 *      RETURNS - NONE
 */
void uart2WrIntVal(uint64_t data, uint8_t bytes);

/*** uart2WrUint8 *****************************************************************************************************************/
/* This routine copies a byte into the UART1 TX soft buffer.
 *      INPUTS  - NONE
 *      RETURNS - NONE
 */
void uart2WrUint8(uint8_t data);

/*** uart2WriteBuf ****************************************************************************************************************/ 
/* This routine copies data specified in a buffer by starting and ending indexes into the UART1 TX soft buffer. The starting index 
 * is equal to the ending index upon completion.
 *      INPUTS  - pointer to soft buffer
 *              - pointer to soft buffer starting index
 *              - soft buffer ending index
 *      RETURNS - NONE
 */
void uart2WriteBuf(uint8_t *buf, uint8_t *bufStart, uint8_t bufEnd);

/*** uart2ReadBuf *****************************************************************************************************************/
/* This routine copies data from the soft UART1 RX buffer into specified soft buffer beginning at 'bufStart' and finishing at
 * 'bufEnd'. 'bufEnd' is updated with the last written index+1.
 *      INPUTS  - pointer to soft buffer
 *              - soft buffer starting index
 *              - pointer to soft buffer ending index
 *      RETURNS - NONE
 */ 
void uart2ReadBuf(uint8_t *buf, uint8_t bufStart, uint8_t *bufEnd);

/*** uart2Service *****************************************************************************************************************/
/* This routine services RX and TX data flow to / from the UART1 port.
 * It will first checks for errors and log them into local status variable. The status variable is returned to the caller upon 
 * completion. Next, the service moves any UART1 RX data into the UART1 RX soft buffer. If any data is present in the TX soft buffer 
 * it is copied into the UART1 TX buffer. Data overruns are prevented by monitoring the full status of the UART1 TX buffer. When TX 
 * soft buffer data coping is complete the subroutine execution is done.
 *      INPUTS  - NONE
 *      RETURNS - status (no error; parity, frame, overrun error or any combination of the three)
 */
uint8_t uart2Service(void);

/*** uart2RxService ***************************************************************************************************************/
/* This routine services RX data flow from the UART1 port.
 * It will first checks for errors and log them into local status variable. The status variable is returned to the caller upon 
 * completion. Next, the service moves any UART RX data into the UART1 RX soft buffer.
 *      INPUTS  - NONE
 *      RETURNS - status (no error; parity, frame, overrun error or any combination of the three)
 */
uint8_t uart2RxService(void);

/*** uart2TxService ***************************************************************************************************************/
/* This routine services TX data flow to the UART1 port.
 * It will first checks for errors and log them into local status variable. The status variable is returned to the caller upon 
 * completion. If any data is present in the TX soft buffer, the subroutine moves the contents into the UART1 TX buffer. Data 
 * overruns are prevented by monitoring the full status of the UART1 TX buffer. When TX soft buffer data coping is complete the 
 * subroutine execution is done.
 *      INPUTS  - NONE
 *      RETURNS - status (no error; parity, frame, overrun error or any combination of the three)
 */
uint8_t uart2TxService(void);

#endif                                                                          // End _UART1_H

