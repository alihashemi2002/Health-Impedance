#ifndef _25AA640AT_H  
#define _25AA640AT_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/**********************************************************************************************************************************/
/*=== PIN ASSIGNMENTS ============================================================================================================*/
/**********************************************************************************************************************************/
#define promCsPin()                 (LATBbits.LATB0)                            // PROM CS pin
#define promWpPin()                 (LATBbits.LATB2)                            // PROM WP pin
#define promSpiOpenFrame()          (promCsPin() = 0)                           // Open SPI frame
#define promSpiCloseFrame()         (promCsPin() = 1)                           // Close SPI frame
#define promSpiEnhBufSize()         15                                          // PROM used SPI port enhanced buffer size (base 0)
#define promWpOn()                  (promWpPin() = 0)                           // Write protect enable
#define promWpOff()                 (promWpPin() = 1)                           // Write protect disable

/**********************************************************************************************************************************/
/*=== SYSTEM ASSIGNMENTS =========================================================================================================*/
/**********************************************************************************************************************************/
#define promSpiBuf()                SPI4BUF                                     // SPI hardware buffer in use
#define promSpiTxBufFull()          SPI4STATbits.SPITBF                         // SPI transmit buffer full flag
#define promSpiRxBufEmpty()         SPI4STATbits.SPIRBE                         // SPI receive buffer empty flag
#define promSpiBusy()               SPI4STATbits.SPIBUSY                        // SPI busy flag
#define promSpiCon()                (SPI4CON)                                   // SPIxCON associated with PROM allocated SPI port
#define promSpiCon2()               (SPI4CON2)                                  // SPIxCON2 associated with PROM allocated SPI port
#define promSpiBrg()                (SPI4BRG)                                   // SPIxBRG associated with PROM allocated SPI port
#define promSpiEnhBufSize()         15                                          // PROM used SPI port enhanced buffer size (base 0)

/**********************************************************************************************************************************/
/*=== TIMING ASSIGNMENTS =========================================================================================================*/
/**********************************************************************************************************************************/
#define promDelayMs()               10                                          // Wait time after write
#define servCallRatekHz()           10                                          // Service call rate in kHz
#define promDelay()                 (promDelayMs()*servCallRatekHz())           // Wait time counter target

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define promBufSize()               (UINT8_MAX+1)                               // Soft buffer size
#define promSize()                  0x1FFF                                      // Memory size
#define promPage()                  32                                          // Page size in bytes
#define promNop()                   0                                           // NOP
#define promFltNumSize()            4                                           // Number of bytes in float

/*=== PROM COMMAND VALUES ========================================================================================================*/
#define promWrsrInstr()             0x01                                        // Write status instruction
#define promWriteInstr()            0x02                                        // Write instruction
#define promReadInstr()             0x03                                        // Read instruction
#define promRdsrInstr()             0x05                                        // Read status instruction
#define promWrdiInstr()             0x04                                        // Disable write instruction
#define promWrenInstr()             0x06                                        // Enable write instruction
#define promProtNone()              0x00                                        // Array protect: NONE
#define promProt14()                0x01                                        // Array protect: 1/4 (address 0x1800 - 0x1FFF)
#define promProt12()                0x02                                        // Array protect: 1/2 (address 0x1000 - 0x1FFF)
#define promProtAll()               0x03                                        // Array protect: ALL (address 0x0000 - 0x1FFF)

/*=== PROM STATE MACHINE VALUES ==================================================================================================*/
#define promInitialState()          0                                           // initial state
#define promWrsrState()             1                                           // Write status state
#define promParseState()            2                                           // Parse state
#define promWriteState0()           3                                           // Write state 0
#define promWriteState1()           4                                           // Write state 1
#define promWriteState2()           5                                           // Write state 2
#define promWriteState3()           6                                           // Write state 3
#define promWaitState()             7                                           // Wait state
#define promReadState0()            8                                           // Read state 0
#define promReadState1()            9                                           // Read state 1
#define promReadState2()            10                                          // Read state 2

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
typedef struct{
    uint8_t     tx[promBufSize()];                                              // PROM TX buffer
    uint8_t     txStart;                                                        // PROM TX buffer start index
    uint8_t     txEnd;                                                          // PROM TX buffer end index
    uint8_t     rx[promBufSize()];                                              // PROM RX buffer
    uint8_t     rxStart;                                                        // PROM RX buffer start index
    uint8_t     rxEnd;                                                          // PROM RX buffer end index
    uint8_t     state;                                                          // PROM state machine index
    uint8_t     statePrev;                                                      // PROM state machine previous index
    uint16_t    addr;                                                           // PROM address
    uint16_t    bytes;                                                          // PROM number of bytes to read or write
    uint8_t     bufLoad;                                                        // Keeps track of SPI buffer load
    uint16_t    delayCntr;                                                      // PROM delay counter
} prom_obj; 

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** promSpiInit ******************************************************************************************************************/
/* This subroutine initializes the SPI port for the prom
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promSpiInit(void);

/*** promSpiRxFifoClear ***********************************************************************************************************/
/* This subroutine clears the prom SPI port receive FIFO
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promSpiRxFifoClear(void);

/*** promLockBlock1 ***************************************************************************************************************/
/* This subroutine...
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promLockBlock1(void);

/*** promLockBlock2 ***************************************************************************************************************/
/* This subroutine...
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promLockBlock2(void);

/*** promLockBlock3 ***************************************************************************************************************/
/* This subroutine...
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promLockBlock3(void);

/*** promUnlock *******************************************************************************************************************/
/* This subroutine...
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promUnlock(void);

/*** promWriteData ****************************************************************************************************************/
/* This subroutine writes the write instruction to the PROM and selects how many bytes to write.
 * 
 *          INPUT   - starting PROM address
 *                  - number of bytes to write
 *          RETURNS - NONE
 */
void promWriteData(uint16_t promAddr, uint16_t numOfBytes);

/*** promWriteUint8 ***************************************************************************************************************/
/* This subroutine writes uint8_t value to PROM at address. 
 *          INPUT   - uint8_t value to be written
 *                  - starting PROM address
 *          RETURNS - NONE
 */
void promWriteUint8(uint8_t val, uint16_t address);

/*** promWriteUint16 **************************************************************************************************************/
/* This subroutine writes uint16_t value to PROM starting at address. 
 *          INPUT   - uint16_t value to be written
 *                  - starting PROM address
 *          RETURNS - NONE
 */
void promWriteUint16(uint16_t val, uint16_t address);

/*** promWriteUint32 **************************************************************************************************************/
/* This subroutine writes uint32_t value to PROM starting at address. 
 *          INPUT   - uint32_t value to be written
 *                  - starting PROM address
 *          RETURNS - NONE
 */
void promWriteUint32(uint32_t val, uint16_t address);

/*** promWriteInt32 ***************************************************************************************************************/
/* This subroutine writes int32_t value to PROM starting at address. 
 *          INPUT   - int32_t value to be written
 *                  - starting PROM address
 *          RETURNS - NONE
 */
void promWriteInt32(int32_t val, uint16_t address);

/*** promWriteArray ***************************************************************************************************************/
/* This subroutine writes uint8_t values in an array[size] to PROM starting at address. 
 *          INPUT   - uint8_t array of values to be written
 *                  - size of array 
 *                  - starting PROM address
 *          RETURNS - NONE
 */
void promWriteArray(uint8_t *arr, uint8_t size, uint16_t address);

/*** promWriteFloat ***************************************************************************************************************/
/* This subroutine writes float value in an array[4] to PROM starting at address. 
 *          INPUT   - float value to be written
 *                  - starting PROM address
 *          RETURNS - NONE
 */
void promWriteFloat(float val, uint16_t address);

/*** promReadData *****************************************************************************************************************/
/* This subroutine...
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promReadData(uint16_t promAddr, uint16_t numOfBytes);

/*** promReadUint8 ****************************************************************************************************************/
/* This subroutine reads uint8_t value from PROM at address. The characters are saved in promRxBuf[].
 *          INPUT   - starting PROM address
 *          RETURNS - NONE
 */
void promReadUint8(uint16_t address);

/*** promReadUint16 ***************************************************************************************************************/
/* This subroutine reads uint16_t from PROM starting at address. The characters are saved in promRxBuf[].
 *          INPUT   - starting PROM address
 *          RETURNS - NONE
 */
void promReadUint16(uint16_t address);

/*** promReadUint32 ***************************************************************************************************************/
/* This subroutine reads uint32_t from PROM starting at address. The character is saved in promRxBuf[].
 *          INPUT   - starting PROM address
 *          RETURNS - NONE
 */
void promReadUint32(uint16_t address);

/*** promReadInt32 ****************************************************************************************************************/
/* This subroutine reads int32_t from PROM starting at address. The character is saved in promRxBuf[].
 *          INPUT   - starting PROM address
 *          RETURNS - NONE
 */
void promReadInt32(uint16_t address);

/*** promReadUint32 ***************************************************************************************************************/
/* This subroutine reads IEEE754 float from PROM starting at address. The character is saved in promRxBuf[].
 *          INPUT   - starting PROM address
 *          RETURNS - NONE
 */
void promReadFloat(uint16_t address);                                          

/*** promReadBuf ******************************************************************************************************************/
/* This subroutine writes an array of uint8_t from the promRxBuf to buf.
 *          INPUT   - buffer to write
 *                  - buffer start index
 *                  - buffer end index
 *          RETURNS - NONE
 */
void promReadBuf(uint8_t *buf, uint8_t bufStart, uint8_t *bufEnd);

/*** promBytesInRx ****************************************************************************************************************/ 
/* This routine returns the difference between the RX end and RX start indeces for the RX soft buffer
 *      INPUTS  - NONE
 *      RETURNS - UINT8 'bytes' total
 */
uint8_t promBytesInRx(void);

/*** promInitialize ***************************************************************************************************************/
/* This subroutine initializes the SPI port for the prom and turns on the hardware write protect
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promInitialize(void);

/*** promService ******************************************************************************************************************/
/* This subroutine...
 * 
 *          INPUT   - NONE
 *          RETURNS - NONE
 */
void promService(void);

#endif                                                                          // End _25AA640AT_H