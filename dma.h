#ifndef _DMA_H
#define _DMA_H

#include <xc.h>
#include <sys/kmem.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/attribs.h>
#include "uart2.h"
        
/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/

//--- ERROR CODES -----------------------------------------------------------------------------------------------------------------
#define dmaErrAllBufUsed()                  0x01                                // Error: all TX buffers are in use
#define dmaErrTxOvfl()                      0x02                                // Error: TX buffer overflow
#define dmaErrChBusy()                      0x03                                // Error: channel is busy
#define dmaErrMsgTooBig()                   0x04                                // Error: message is too big
#define dmaErrDoesNotExist()                0x05                                // Error: Object does not exist
#define dmaErrAllBufBusy()                  0x06                                // Error: all buffers are busy
#define dmaErrNoData()                      0x07                                // Error: no data available
#define dmaErrNop()                         0xAA                                // Error: no error

//--- LABEL DEFINITIONS -----------------------------------------------------------------------------------------------------------
#define dmaModule()                         0x01                                // Alias: DMA module
#define dmaSrc1()                           0x02                                // Alias: Source 1
#define dmaSrc2()                           0x03                                // Alias: Source 2
#define dmaTxBuf1()                         0x04                                // DMA TX buffer 1
#define dmaTxBuf2()                         0x05                                // DMA TX buffer 2
#define dmaTxBuf3()                         0x06                                // DMA TX buffer 3
#define dmaTxLabelMin()                     dmaTxBuf1()                         // Lowest DMA TX buffer label
#define dmaTxLabelMax()                     dmaTxBuf3()                         // Highest DMA TX buffer label
#define dmaUserLabelMin()                   dmaModule()                         // Lowest object user index
#define dmaUserLabelMax()                   dmaSrc2()                           // Highest object user index
#define dmaNulLabel()                       0xAA                                // DMA null label

#define dmaNumOfTxObj()                     4                                   // Number of TX buffer objects - 1
#define dmaCh0CntrDflt()                    20                                  // DMA CH0 default counter value
#define dmaCh1CntrDflt()                    20                                  // DMA CH1 default counter value
#define dmaBufLen()                         100                                 // DMA buffers length

#if defined(__32MZ2048EFG144__)

//--- DMA MODULE ------------------------------------------------------------------------------------------------------------------
#define dmaEn()                             (DMACONSET  = 0x00008000)           // Enable DMA
#define dmaDis()                            (DMACONCLR  = 0x00008000)           // Disable DMA

//--- DMA CH0 ---------------------------------------------------------------------------------------------------------------------
#define dmaCh0En()                          (DCH0CONSET = 0x00000080)           // Enable DMA CH0
#define dmaCh0EnStat()                      (DCH0CONbits.CHEN)                  // DMA CH0 enable status
#define dmaCh0Dis()                         (DCH0CONCLR = 0x00000080)           // Disable DMA CH0
#define dmaCh0ConDflt()                     0x00000003                          // CHPIGN =0; CHBUSY =0; CHPIGNEN =0; CHPATLEN =0; CHCHNS =0; CHEN =0; CHAED =0; CHCHN =0; CHAEN =0; CHEDET =0; CHPRI =3
#define dmaCh0EconDflt()                    0x00009210                          // CHAIRQ =0; CHSIRQ =146 (UART2 RX); CFORCE =0; CABORT =0; PATEN =0; SIRQEN =1; AIRQEN =0;
#define dmaInt0SetDflt()                    (DCH0INTCLR=0x00ff00ff)             // All interrupts off; clear all events
#define dmaSrc0Dflt()                       (KVA_TO_PA(&U2RXREG))               // Default CH0 source is U2RXREG
#define dmaDst0Dflt()                       (KVA_TO_PA(&dma.rx1))               // Default CH0 destination is array
#define dmaSrc0SizeDflt()                   1                                   // Source size is at most 1 byte by default
#define dmaDst0SizeDflt()                   32                                  // Destination is maxium of 32 bytes by default
#define dmaCellSizeDflt()                   1                                   // Default number of bytes transferred per event
#define dmaCh0IrqOff()                      (IEC4CLR    = 0x00000040)           // Disable DMA CH0 interrupt
#define dmaCh0IrqOn()                       (IEC4SET    = 0x00000040)           // Enable DMA CH0 interrupt
#define dmaCh0IsrClr()                      (IFS4CLR    = 0x00000040)           // Clear DMA CH0 ISR flag
#define dmaCh0IrqCfgDflt()                  (IPC33SET   = 0x000F0000)           // Priority 3; sub-priority 3

//--- DMA CH1 ---------------------------------------------------------------------------------------------------------------------
#define dmaCh1En()                          (DCH1CONSET = 0x00000080)           // Enable DMA CH1
#define dmaCh1EnStat()                      (DCH1CONbits.CHEN)                  // DMA CH1 enable status
#define dmaCh1Dis()                         (DCH1CONCLR = 0x00000080)           // Disable DMA CH1
#define dmaCh1ConDflt()                     0x00000002                          // CHPIGN =0; CHBUSY =0; CHPIGNEN =0; CHPATLEN =0; CHCHNS =0; CHEN =0; CHAED =0; CHCHN =0; CHAEN =0; CHEDET =0; CHPRI =2
#define dmaCh1EconDflt()                    0x00009310                          // CHAIRQ =0; CHSIRQ =147 (UART2 TX); CFORCE =0; CABORT =0; PATEN =0; SIRQEN =1; AIRQEN =0;
#define dmaInt1SetDflt()                    (DCH1INTCLR = 0x00ff00ff)           // All interrupts off; clear all events
#define dmaDst1Dflt()                       (KVA_TO_PA(&U2TXREG))               // Default CH1 destination is U2TXREG
#define dmaSrc1SizeDflt()                   32                                  // Source size is at most 32 bytes by default
#define dmaDst1SizeDflt()                   1                                   // Destination is maxium of 1 bytes by default
#define dmaCellSizeDflt()                   1                                   // Default number of bytes transferred per event
#define dmaCh1IrqOff()                      (IEC4CLR    = 0x00000080)           // Disable DMA CH1 interrupt
#define dmaCh1IrqOn()                       (IEC4SET    = 0x00000080)           // Enable DMA CH1 interrupt
#define dmaCh1IsrClr()                      (IFS4CLR    = 0x00000080)           // Clear DMA CH1 ISR flag
#define dmaCh1IrqCfgDflt()                  (IPC33SET   = 0x0B000000)           // Priority 2; sub-priority 3

#endif

#if defined(__32MZ1024EFK100__)

//--- DMA MODULE ------------------------------------------------------------------------------------------------------------------
#define dmaEn()                             (DMACONSET  = 0x00008000)           // Enable DMA
#define dmaDis()                            (DMACONCLR  = 0x00008000)           // Disable DMA

//--- DMA CH0 ---------------------------------------------------------------------------------------------------------------------
#define dmaCh0En()                          (DCH0CONSET = 0x00000080)           // Enable DMA CH0
#define dmaCh0EnStat()                      (DCH0CONbits.CHEN)                  // DMA CH0 enable status
#define dmaCh0Dis()                         (DCH0CONCLR = 0x00000080)           // Disable DMA CH0
#define dmaCh0ConDflt()                     0x00000003                          // CHPIGN =0; CHBUSY =0; CHPIGNEN =0; CHPATLEN =0; CHCHNS =0; CHEN =0; CHAED =0; CHCHN =0; CHAEN =0; CHEDET =0; CHPRI =3
#define dmaCh0EconDflt()                    0x00009210                          // CHAIRQ =0; CHSIRQ =146 (UART2 RX); CFORCE =0; CABORT =0; PATEN =0; SIRQEN =1; AIRQEN =0;
#define dmaInt0SetDflt()                    (DCH0INTCLR=0x00ff00ff)             // All interrupts off; clear all events
#define dmaSrc0Dflt()                       (KVA_TO_PA(&U2RXREG))               // Default CH0 source is U2RXREG
#define dmaDst0Dflt()                       (KVA_TO_PA(&dma.rx1))               // Default CH0 destination is array
#define dmaSrc0SizeDflt()                   1                                   // Source size is at most 1 byte by default
#define dmaDst0SizeDflt()                   32                                  // Destination is maxium of 32 bytes by default
#define dmaCellSizeDflt()                   1                                   // Default number of bytes transferred per event
#define dmaCh0IrqOff()                      (IEC4CLR    = 0x00000040)           // Disable DMA CH0 interrupt
#define dmaCh0IrqOn()                       (IEC4SET    = 0x00000040)           // Enable DMA CH0 interrupt
#define dmaCh0IsrClr()                      (IFS4CLR    = 0x00000040)           // Clear DMA CH0 ISR flag
#define dmaCh0IrqCfgDflt()                  (IPC33SET   = 0x000F0000)           // Priority 3; sub-priority 3

//--- DMA CH1 ---------------------------------------------------------------------------------------------------------------------
#define dmaCh1En()                          (DCH1CONSET = 0x00000080)           // Enable DMA CH1
#define dmaCh1EnStat()                      (DCH1CONbits.CHEN)                  // DMA CH1 enable status
#define dmaCh1Dis()                         (DCH1CONCLR = 0x00000080)           // Disable DMA CH1
#define dmaCh1ConDflt()                     0x00000002                          // CHPIGN =0; CHBUSY =0; CHPIGNEN =0; CHPATLEN =0; CHCHNS =0; CHEN =0; CHAED =0; CHCHN =0; CHAEN =0; CHEDET =0; CHPRI =2
#define dmaCh1EconDflt()                    0x00009310                          // CHAIRQ =0; CHSIRQ =147 (UART2 TX); CFORCE =0; CABORT =0; PATEN =0; SIRQEN =1; AIRQEN =0;
#define dmaInt1SetDflt()                    (DCH1INTCLR = 0x00ff00ff)           // All interrupts off; clear all events
#define dmaDst1Dflt()                       (KVA_TO_PA(&U2TXREG))               // Default CH1 destination is U2TXREG
#define dmaSrc1SizeDflt()                   32                                  // Source size is at most 32 bytes by default
#define dmaDst1SizeDflt()                   1                                   // Destination is maxium of 1 bytes by default
#define dmaCellSizeDflt()                   1                                   // Default number of bytes transferred per event
#define dmaCh1IrqOff()                      (IEC4CLR    = 0x00000080)           // Disable DMA CH1 interrupt
#define dmaCh1IrqOn()                       (IEC4SET    = 0x00000080)           // Enable DMA CH1 interrupt
#define dmaCh1IsrClr()                      (IFS4CLR    = 0x00000080)           // Clear DMA CH1 ISR flag
#define dmaCh1IrqCfgDflt()                  (IPC33SET   = 0x0B000000)           // Priority 2; sub-priority 3

#endif

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
typedef struct{
    const uint8_t       chBusy;                                                 // Error: DMA channel is busy
    const uint8_t       msgTooBig;                                              // Error: message length exceeds the DMA buffer length
    const uint8_t       bufOvfl;                                                // Error: pending buffer overflow
    const uint8_t       allBufInUse;                                            // Error: all TX buffers are in use
    const uint8_t       doesNotExist;                                           // Error: object does not exist
    const uint8_t       allBufBusy;                                             // Error: buffers are busy
    const uint8_t       noData;                                                 // Error: no data available
    const uint8_t       nop;                                                    // Error: no error
} dmaErr_obj;

typedef struct{
    const uint8_t       dma;                                                    // Label: DMA
    const uint8_t       src1;                                                   // Label: source 1
    const uint8_t       src2;                                                   // Label: source 2
    const uint8_t       txBuf1;                                                 // Label: TX buffer object 1
    const uint8_t       txBuf2;                                                 // Label: TX buffer object 2
    const uint8_t       txBuf3;                                                 // Label: TX buffer object 3
    const uint8_t       null;                                                   // Label: null label
} dmaLabels_obj;

typedef struct{
    dmaErr_obj          err;                                                    // DMA errors object
    dmaLabels_obj       label;                                                  // DMA buffer user object
} dmaEnum_obj;

typedef struct{
    volatile uint8_t    tx[dmaBufLen()];                                        // TX buffer
    volatile uint8_t    txCount;                                                // Number of elements to transfer
    volatile uint8_t    usedBy;                                                 // Logs present user
    volatile uint8_t    error;                                                  // Log object error
    const    uint8_t    label;                                                  // Buffer identifier
} dmaTxBuf_obj;

typedef struct{
    dmaEnum_obj         enums;                                                  // DMA constants
    volatile uint8_t    rx1[dmaBufLen()];                                       // DMA RX1 buffer
    volatile uint8_t    rx2[dmaBufLen()];                                       // DMA RX2 buffer
    volatile uint8_t    error;                                                  // Log error
    volatile bool       uartTxServEn;                                           // DMA UART TX service enable flag
    volatile bool       uartTxServReq;                                          // DMA UART TX service enable request
} dma_obj;

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** DMA_Initialize ***************************************************************************************************************/
/* This subroutine initializes DMA instance: index, making it ready for clients to open and use it. It also initializes any internal 
 * data structures. This routine must be called before any other DMA routine is called. 
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */
void DMA_Initialize(void);

/*** dmaCh0RxChars ****************************************************************************************************************/
/* This subroutine returns the number of transfered bytes to the destination.
 *      PARAMETERS  - NONE
 *      RETURNS     - number of transfered bytes
 */
uint16_t dmaCh0RxChars(void);

/*** *getDmaUartRxBuf *************************************************************************************************************/
/* This subroutine returns the soft RX buffer allocated to UART 2 RX DMA.
 *      PARAMETERS  - NONE
 *      RETURNS     - pointer to a buffer
 */
volatile uint8_t *getDmaUartRxBuf(void);

/*** dmaCh0Init *******************************************************************************************************************/
/* This subroutine reinitializes DMA channel 0 for use with UART 2 RX; Default destination is dma.rx1.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */
void dmaCh0Init(void);

/*** dmaCh1Init *******************************************************************************************************************/
/* This subroutine reinitializes DMA channel 1 for use with UART 2 TX. Source is set to dmaTx1.tx.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */
void dmaCh1Init(void);

/*** dmaCh2Init *******************************************************************************************************************/
/* This subroutine disables DMA channel 2, and restores default CH2 configuration. It also clears soft buffer counters and enables 
 * channel 2.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */
void dmaCh2Init(void);

/*** dmaUartTxServEn **************************************************************************************************************/
void dmaUartTxServEn(void);

/*** dmaUartTxServDis *************************************************************************************************************/
void dmaUartTxServDis(void);

/*** dmaUartTxServ ****************************************************************************************************************/
/* This subroutine services the dmaTxBuf_obj instances. If DMA channel 1 is busy or DMA error is stored in dma.error, the subroutine
 * quits by returning status / error. Else, it releases the last used dmaTxBuf_obj, initializes DMA channel 1 with the new 
 * dmaTxBuf_obj settings, enables DMA channel 1, and returns no error. If there is no data in any of the buffer objects, it will 
 * return dmaErrNoData(). 
 *      PARAMETERS  - NONE
 *      RETURNS     - DMA error / status (check enumerations in this file)
 */
uint8_t dmaUartTxServ(void);

/**** getDmaBufObj ****************************************************************************************************************/
/* This subroutine returns the dmaTxBuf_obj which label matches the selector parameter. If the parameter is out of range, dmaTx1
 * is returned and error is stored in dma.error.
 *      PARAMETERS  - NONE
 *      RETURNS     - dmaTxBuf_obj (check enumerations in this file)
 */
dmaTxBuf_obj *getDmaBufObj(uint8_t selector);

/*** *getDmaUartTxBuf *************************************************************************************************************/
/* This subroutine checks for stored DMA errors and of returns dmaTx1.tx if error is present. Else, it checks for unused dmaTxBuf_obj 
 * that can accept the size of the message about to be written and returns pointer to to it. If no buffer can accept the message, 
 * dmaTx1.tx is returned and dmaErrAllBufBusy() is logged in dma.error.
 *      PARAMETERS  - NONE
 *      RETURNS     - pointer to a buffer location
 */
volatile uint8_t *getDmaUartTxBuf(uint8_t msgSize, uint8_t user);

/**** dmaReleaseBuf ***************************************************************************************************************/
/* This subroutine releases the buffer object used by "user" and returns dmaErrNop(). If user does no exist, dmaErrDoesNotExist() 
 * is returned and logged in dma.error.
 *      PARAMETERS  - NONE
 *      RETURNS     - DMA error / status (check enumerations in this file)
 */
volatile uint8_t dmaReleaseBuf(uint8_t user);

/*** dmaSetTxMsgSize **************************************************************************************************************/
/* This subroutine checks if the message to send it too long or concatenating to prior data will cause buffer overflow. If checks 
 * pass, the total number of elements to be sent is accumulated into dmaTxn.txCount.
 *      PARAMETERS  - number of elements for DMA to transfer; object user
 *      RETURNS     - DMA error / status (check enumerations in this file)
 */
uint8_t dmaSetTxMsgSize(uint8_t numOfBytes, uint8_t user);
    
#endif                                                                          // End _DMA_H