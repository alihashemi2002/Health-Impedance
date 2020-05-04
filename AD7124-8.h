#ifndef _AD7124_8_H
#define	_AD7124_8_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
    
/**********************************************************************************************************************************/
/*=== PIN ASSIGNMENTS ============================================================================================================*/
/**********************************************************************************************************************************/
#define adcCsPin()                      (LATDbits.LATD12)                       // CS pin
#define adcDrdyPin()                    (PORTDbits.RD4)                         // DRDY pin
#define adcSyncPin()                    (LATDbits.LATD13)                       // Sync pin
#define adcSyncGet()                    (PORTDbits.RD13)                        // Sync pin
#define adcSpiCloseFrame()              (adcCsPin() = 1)                        // Close ADC SPI frame
#define adcSpiOpenFrame()               (adcCsPin() = 0)                        // Open ADC SPI frame
#define adcSyncSet()                    (adcSyncPin() = 1)                      // Set ADC SYNC pin
#define adcSyncClr()                    (adcSyncPin() = 0)                      // Clear ADC SYNC pin

/**********************************************************************************************************************************/
/*=== SYSTEM ASSIGNMENTS =========================================================================================================*/
/**********************************************************************************************************************************/
#define adcSpiBuf()                     SPI1BUF                                 // ADC SPI buffer
#define adcSpiBusy()                    SPI1STATbits.SPIBUSY                    // ADC SPI busy if true
#define adcSpiRxFifoEmpty()             SPI1STATbits.SPIRBE                     // ADC SPI rx empty if true
#define adcSpiTxFifoEmpty()             SPI1STATbits.SPITBE                     // ADC SPI tx empty if true

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define adcBufSize()                    (UCHAR_MAX+1)                           // SPI RX and TX buffer length
#define spiBufSize()                    8                                       // SPI enhanced buffer length
#define adcSpiNop()                     0x00                                    // ADC SPI NOP used to clock data during readout
#define adcMaxChIndex()                 15                                      // ADC max channel index
#define adcMaxCfgIndex()                7                                       // ADC max configuration index
#define adcMaxFilterIndex()             7                                       // ADC max filter index
#define adcMaxOffsetIndex()             7                                       // ADC max offset index
#define adcMaxGainIndex()               7                                       // ADC max gain index
#define adcDefaultOffset()              0x800000                                // ADC default offset
#define adcWriteOp()                    0x01                                    // ADC write operation was done
    
/*=== ADC COMMAND VALUES =========================================================================================================*/
#define adcCmdWrite()                   0x00                                    // AD7124-8 write (OR w/ address)
#define adcCmdRead()                    0x40                                    // AD7124-8 read (OR w/ address)
#define adcCalReq()                     0x03                                    // Calibrate ADC request
#define adcRate10Req()                  0x04                                    // Set sampling rate to 10 sps request
#define adcRate60Req()                  0x05                                    // Set sampling rate to 60 sps request 
#define adcRate100Req()                 0x06                                    // Set sampling rate to 100 sps request 
#define adcRate1kReq()                  0x07                                    // Set sampling rate to 1000 sps request
#define adcRate4k8Req()                 0x08                                    // Set sampling rate to 4800 sps request
#define adcRate9k6Req()                 0x09                                    // Set sampling rate to 9600 sps request
#define adcRate19k2Req()                0x0A                                    // Set sampling rate to 19200 sps request
#define adcShntOnReq()                  0x0B                                    // Turn on shunt request
#define adcShntOffReq()                 0x0C                                    // Turn off shunt request

/*=== ADC REGISTER DEFAULTS ======================================================================================================*/
#define adcControlDflt()                0x07C0                                  // DATA STATUS_ON; CS_ON; REF ON; FULL POWER; CONT CONV; USE INT OSC
#define adcCh0Dflt()                    0x8112                                  // CH0 on; SETUP 0; AINP AIN8; AINM INT_REF
#define adcConfig0Dflt()                0x09F0                                  // BIPOLAR; REFBUF ON; AINBUF ON; INT REF; PGA=1
#define adcFilter0Dflt()                0x100780                                // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
#define adcControlCalGainDflt()         0x0318                                  // REF ON; LOW POWER; CAL GAIN; USE INT OSC
#define adcControlCalOfstDftl()         0x0314                                  // REF ON; LOW POWER; CAL OFFSET; USE INT OSC

/*=== ADC REGISTER ADDRESSES =====================================================================================================*/
#define adcAddrComms()                  0x00                                    // AD7124-8 COMMS address (W)
#define adcAddrStatus()                 0x00                                    // AD7124-8 STATUS address (R)
#define adcAddrControl()                0x01                                    // AD7124-8 ADC_CONTROL address (R/W)
#define adcAddrData()                   0x02                                    // AD7124-8 DATA address (R)
#define adcAddrIoCtrl1()                0x03                                    // AD7124-8 IO_CONTROL_1 address (R/W)
#define adcAddrIoCtrl2()                0x04                                    // AD7124-8 IO_CONTROL_2 address (R/W)
#define adcAddrId()                     0x05                                    // AD7124-8 ID address (R)
#define adcAddrError()                  0x06                                    // AD7124-8 ERROR address (R)
#define adcAddrErrorEn()                0x07                                    // AD7124-8 ERROR_EN address (R/W)
#define adcAddrMclkCnt()                0x08                                    // AD7124-8 MCLK_COUNT address (R)
#define adcAddrCh0()                    0x09                                    // AD7124-8 CHANNEL_0 address (R/W)
#define adcAddrCh15()                   0x18                                    // AD7124-8 CHANNEL_15 address (R/W)
#define adcAddrConfig0()                0x19                                    // AD7124-8 CONFIG_0 address (R/W)
#define adcAddrConfig7()                0x20                                    // AD7124-8 CONFIG_7 address (R/W)
#define adcAddrFilter0()                0x21                                    // AD7124-8 FILTER_0 address (R/W)
#define adcAddrFilter7()                0x28                                    // AD7124-8 FILTER_7 address (R/W)
#define adcAddrOffset0()                0x29                                    // AD7124-8 OFFSET_0 address (R/W)
#define adcAddrOffset7()                0x30                                    // AD7124-8 OFFSET_7 address (R/W)
#define adcAddrGain0()                  0x31                                    // AD7124-8 GAIN_0 address (R/W)
#define adcAddrGain7()                  0x38                                    // AD7124-8 GAIN_7 address (R/W)

/*=== ADC REGISTER BYTE LENGTH ===================================================================================================*/
#define adcRlenComms()                  1                                       // COMMS register byte length
#define adcRlenStatus()                 1                                       // STATUS register byte length
#define adcRlenControl()                2                                       // ADC_CONTROL register byte length
#define adcRlenData()                   4                                       // DATA register byte length
#define adcRlenIoCtrl1()                3                                       // IO_CONTROL_1 register byte length
#define adcRlenIoCtrl2()                2                                       // IO_CONTROL_2 register byte length
#define adcRlenId()                     1                                       // ID register byte length
#define adcRlenError()                  3                                       // ERROR register byte length
#define adcRlenErrorEn()                3                                       // ERROR_EN register byte length
#define adcRlenMclkCnt()                1                                       // MCLK_COUNT register byte length
#define adcRlenCh()                     2                                       // CHANNEL_n register byte length
#define adcRlenConfig()                 2                                       // CONFIG_n register byte length
#define adcRlenFilter()                 3                                       // FILTER_n register byte length
#define adcRlenOffset()                 3                                       // OFFSET_n register byte length
#define adcRlenGain()                   3                                       // GAIN_n register byte length
    
/*=== ADC STATE MACHINE VALUES ===================================================================================================*/
#define adcSmInitial()                  0x01                                    // Initial state
#define adcSmRead()                     0x02                                    // Read state
#define adcSmWrite()                    0x03                                    // Write state

/*=== ADC CHANNEL NUMBER VALUES ==================================================================================================*/
#define adcCh0()                        0                                       // Channel 0
#define adcCh1()                        1                                       // Channel 1
#define adcCh2()                        2                                       // Channel 2
#define adcCh3()                        3                                       // Channel 3
#define adcCh4()                        4                                       // Channel 4
#define adcCh5()                        5                                       // Channel 5
#define adcCh6()                        6                                       // Channel 6
#define adcCh7()                        7                                       // Channel 7
#define adcCh9()                        9                                       // Channel 9
#define adcCh10()                       10                                      // Channel 10
#define adcCh11()                       11                                      // Channel 11
#define adcCh12()                       12                                      // Channel 12
#define adcCh13()                       13                                      // Channel 13
#define adcCh14()                       14                                      // Channel 14
#define adcCh15()                       15                                      // Channel 15

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
typedef struct{
    uint8_t rx[adcBufSize()];                                                   // RX buffer
    uint8_t rxStart;                                                            // RX buffer lagging index
    uint8_t rxEnd;                                                              // RX buffer leading index    
} adcCh_obj;

typedef struct{
    uint8_t   tx[adcBufSize()];                                                 // TX buffer
    uint8_t   txStart;                                                          // TX buffer lagging index
    uint8_t   txEnd;                                                            // TX buffer leading index
    uint8_t   writing;                                                          // Currently written register
    uint8_t   reading;                                                          // Currently read register
    uint8_t   bToRead;                                                          // Bytes to read
    uint8_t   crntCh;                                                           // Current channel being converted
    adcCh_obj ch[adcMaxChIndex()];                                              // Channel variables
    bool      statusEn;                                                         // Status byte enabled flag
    uint8_t   setBytes;                                                         // Number of setup bytes
    bool      initDone;                                                         // Initialization done flag
} adc_obj;

typedef struct{
    uint8_t     comms;                                                          // ADC COMMS register mirror
    uint8_t     status;                                                         // ADC STATUS register mirror
    uint16_t    adcControl;                                                     // ADC ADC_CONTROL register mirror
    uint32_t    ioControl1;                                                     // ADC IO_CONTROL_1 register mirror
    uint16_t    ioControl2;                                                     // ADC IO_CONTROL_2 register mirror
    uint8_t     id;                                                             // ADC ID register mirror
    uint32_t    error;                                                          // ADC ERROR register mirror
    uint32_t    errorEn;                                                        // ADC ERROR_EN register mirror
    uint8_t     mclkCount;                                                      // ADC MCLK_COUNT register mirror
    uint16_t    ch0;                                                            // ADC CHANNEL_0  register mirror
    uint16_t    ch1;                                                            // ADC CHANNEL_1  register mirror
    uint16_t    ch2;                                                            // ADC CHANNEL_2  register mirror
    uint16_t    ch3;                                                            // ADC CHANNEL_3  register mirror
    uint16_t    ch4;                                                            // ADC CHANNEL_4  register mirror
    uint16_t    ch5;                                                            // ADC CHANNEL_5  register mirror
    uint16_t    ch6;                                                            // ADC CHANNEL_6  register mirror
    uint16_t    ch7;                                                            // ADC CHANNEL_7  register mirror
    uint16_t    ch8;                                                            // ADC CHANNEL_8  register mirror
    uint16_t    ch9;                                                            // ADC CHANNEL_9  register mirror
    uint16_t    ch10;                                                           // ADC CHANNEL_10 register mirror
    uint16_t    ch11;                                                           // ADC CHANNEL_11 register mirror
    uint16_t    ch12;                                                           // ADC CHANNEL_12 register mirror
    uint16_t    ch13;                                                           // ADC CHANNEL_13 register mirror
    uint16_t    ch14;                                                           // ADC CHANNEL_14 register mirror
    uint16_t    ch15;                                                           // ADC CHANNEL_15 register mirror
    uint16_t    config0;                                                        // ADC CONFIG_0 register mirror
    uint16_t    config1;                                                        // ADC CONFIG_1 register mirror
    uint16_t    config2;                                                        // ADC CONFIG_2 register mirror
    uint16_t    config3;                                                        // ADC CONFIG_3 register mirror
    uint16_t    config4;                                                        // ADC CONFIG_4 register mirror
    uint16_t    config5;                                                        // ADC CONFIG_5 register mirror
    uint16_t    config6;                                                        // ADC CONFIG_6 register mirror
    uint16_t    config7;                                                        // ADC CONFIG_7 register mirror
    uint32_t    filter0;                                                        // ADC FILTER_0 register mirror
    uint32_t    filter1;                                                        // ADC FILTER_1 register mirror
    uint32_t    filter2;                                                        // ADC FILTER_2 register mirror
    uint32_t    filter3;                                                        // ADC FILTER_3 register mirror
    uint32_t    filter4;                                                        // ADC FILTER_4 register mirror
    uint32_t    filter5;                                                        // ADC FILTER_5 register mirror
    uint32_t    filter6;                                                        // ADC FILTER_6 register mirror
    uint32_t    filter7;                                                        // ADC FILTER_7 register mirror
    uint32_t    offset0;                                                        // ADC OFFSET_0 register mirror
    uint32_t    offset1;                                                        // ADC OFFSET_1 register mirror
    uint32_t    offset2;                                                        // ADC OFFSET_2 register mirror
    uint32_t    offset3;                                                        // ADC OFFSET_3 register mirror
    uint32_t    offset4;                                                        // ADC OFFSET_4 register mirror
    uint32_t    offset5;                                                        // ADC OFFSET_5 register mirror
    uint32_t    offset6;                                                        // ADC OFFSET_6 register mirror
    uint32_t    offset7;                                                        // ADC OFFSET_7 register mirror
    uint32_t    gain0;                                                          // ADC GAIN_0 register mirror
    uint32_t    gain1;                                                          // ADC GAIN_1 register mirror
    uint32_t    gain2;                                                          // ADC GAIN_2 register mirror
    uint32_t    gain3;                                                          // ADC GAIN_3 register mirror
    uint32_t    gain4;                                                          // ADC GAIN_4 register mirror
    uint32_t    gain5;                                                          // ADC GAIN_5 register mirror
    uint32_t    gain6;                                                          // ADC GAIN_6 register mirror
    uint32_t    gain7;                                                          // ADC GAIN_7 register mirror
} adcReg_obj;

typedef struct{
    bool    rdStatus;                                                           // ADC STATUS REGISTER MIRROR
    bool    rdAdcControl;                                                       // ADC ADC_CONTROL read done flag
    bool    wrAdcControl;                                                       // ADC ADC_CONTROL write done flag
    bool    rdData;                                                             // ADC DATA read done flag (single reads only))
    bool    rdIoControl1;                                                       // ADC IO_CONTROL_1 read done flag
    bool    wrIoControl1;                                                       // ADC IO_CONTROL_1 write done flag
    bool    rdIoControl2;                                                       // ADC IO_CONTROL_2 read done flag
    bool    wrIoControl2;                                                       // ADC IO_CONTROL_2 write done flag
    bool    rdId;                                                               // ADC ID read done flag
    bool    rdError;                                                            // ADC ERROR read done flag
    bool    rdErrorEn;                                                          // ADC ERROR_EN read done flag
    bool    wrErrorEn;                                                          // ADC ERROR_EN write done flag
    bool    rdMclkCount;                                                        // ADC MCLK_COUNT read done flag
    bool    rdCh;                                                               // ADC CHANNEL_n read done flag
    bool    wrCh;                                                               // ADC CHANNEL_n write done flag
    bool    rdConfig;                                                           // ADC CONFIG_n read done flag
    bool    wrConfig;                                                           // ADC CONFIG_n write done flag
    bool    rdFilter;                                                           // ADC FILTER_n read done flag
    bool    wrFilter;                                                           // ADC FILTER_n write done flag
    bool    rdOffset;                                                           // ADC OFFSET_n read done flag
    bool    wrOffset;                                                           // ADC OFFSET_n write done flag
    bool    rdGain;                                                             // ADC GAIN_n read done flag
    bool    wrGain;                                                             // ADC GAIN_n write done flag
} adcFlags_obj;
    
/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/   

/*** adcSpiInit *******************************************************************************************************************/ 
/* This subroutine initializes the SPI port for the AD7124-8.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */   
void adcSpiInit(void);

/*** adcRxFifoClear ***************************************************************************************************************/ 
/* This subroutine empties the AD7124-8 SPI port RX FIFO.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */   
void adcRxFifoClear(void);

/*** adcInitialize ****************************************************************************************************************/ 
/* This subroutine initializes AD7124-8 as shown below:
 * Settings:        SPI PORT:           SPI 1
 *                  SPI SETTINGS:       SCK = 4MHz; MODE = 3; 8-bit transfer; Master; Using enhanced buffer (6 byte deep)
 *                  SAMPLING RATE:      1 kSPS
 *                  POWER MODE:         Full
 *                  Ref Buffers:        On
 *                  Oscillator:         Internal
 *                  Conversion Mode:    Continuous
 *                  Filter:             SYNC4
 *                  PGA:                128
 *                  Input:              Bipolar; AINP = CH9, AINM = CH10
 *                  Calibration:        Internal offset and gain
 *                  
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcInitialize(void);

/*** adcSizeOfReg *****************************************************************************************************************/ 
/* This subroutine returns the size of an AD7124-8 register.
 *      PARAMETERS  - register address
 *      RETURNS     - register address size in bytes
 */  
uint8_t adcSizeOfReg(uint8_t addr);

/*** adcLogReg ********************************************************************************************************************/ 
/* This subroutine copies the data written to a register to its mirror.
 *      PARAMETERS  - register address, written data
 *      RETURNS     - NONE
 */  
void adcLogReg(uint8_t addr, uint32_t data);

/*** adcFlagSet *******************************************************************************************************************/ 
/* This subroutine sets a flag indicating that the last command has been executed. It is using the variable 'adcNowWriting' to check
 * the last written register.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcFlagSet(void);

/*** adcWrReg *********************************************************************************************************************/ 
/* This subroutine writes address and data to the TX soft buffer which will then be processed by adcService(). 
 * It calls adcSizeOfReg() to determine how many bytes it needs to write.
 *      PARAMETERS  - register address, data to be written
 *      RETURNS     - NONE
 */ 
void adcWrReg(uint8_t address, uint32_t data);

/*** adcRdReg *********************************************************************************************************************/ 
/* This subroutine writes instruction and address to the TX soft buffer which will then be processed by adsService() in order to 
 * read an AD7124-8 register. The read data is copied to the appropriate register mirror by adcSpiToMirrorReg();
 *      PARAMETERS  - register address
 *      RETURNS     - NONE
 */ 
void adcRdReg(uint8_t addr);

/*** adcSpiToMirrorReg ************************************************************************************************************/ 
/* This subroutine copies register data to its appropriate mirror copy after a successful AD7124-8 register read. It uses
 * the variable 'adcNowReading' to determine which register was read.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcSpiToMirrorReg(void);

/*** adcRdStatus ******************************************************************************************************************/
/* This subroutine places STATUS register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdStatus(void);

/*** adcRdAdcControl **************************************************************************************************************/
/* This subroutine places ADC_CONTROL register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdAdcControl(void);

/*** adcDataRead ******************************************************************************************************************/
/* This subroutine places DATA register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcDataRead(void);

/*** adcRdIoControl1 **************************************************************************************************************/
/* This subroutine places IO_CONTROL register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdIoControl1(void);

/*** adcRdIoControl2 **************************************************************************************************************/
/* This subroutine places IO_CONTROL2 register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdIoControl2(void);

/*** adcRdId **********************************************************************************************************************/
/* This subroutine places ID register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdId(void);

/*** adcRdError *******************************************************************************************************************/
/* This subroutine places ERROR register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdError(void);

/*** adcRdErrorEn *****************************************************************************************************************/
/* This subroutine places ERROR_EN register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdErrorEn(void);

/*** adcRdMclkCount ***************************************************************************************************************/
/* This subroutine places MCLK_COUNT register read instruction into the TX soft buffer.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcRdMclkCount(void);

/*** adcRdCh **********************************************************************************************************************/
/* This subroutine places CHANNEL_n register read instruction into the TX soft buffer.
 *      PARAMETERS  - channel index
 *      RETURNS     - false if channel index is out of range
 *                  - true if executed
 */ 
bool adcRdCh(uint8_t ch);

/*** adcRdConfig ******************************************************************************************************************/
/* This subroutine places CONFIG_n register read instruction into the TX soft buffer.
 *      PARAMETERS  - configuration index
 *      RETURNS     - false if configuration index is out of range
 *                  - true if executed
 */ 
bool adcRdConfig(uint8_t configNum);

/*** adcRdFilter ******************************************************************************************************************/
/* This subroutine places FILTER_n register read instruction into the TX soft buffer.
 *      PARAMETERS  - filter index
 *      RETURNS     - false if filter index is out of range
 *                  - true if executed
 */ 
bool adcRdFilter(uint8_t filterNum);

/*** adcRdOffset ******************************************************************************************************************/
/* This subroutine places OFFSET_n register read instruction into the TX soft buffer.
 *      PARAMETERS  - offset index
 *      RETURNS     - false if offset index is out of range
 *                  - true if executed
 */ 
bool adcRdOffset(uint8_t offsetNum);

/*** adcRdGain ********************************************************************************************************************/
/* This subroutine places GAIN_n register read instruction into the TX soft buffer.
 *      PARAMETERS  - gain index
 *      RETURNS     - false if gain index is out of range
 *                  - true if executed
 */ 
bool adcRdGain(uint8_t gainNum);

/*** adcGetActiveCh ***************************************************************************************************************/
/* This subroutine returns in the index if the active channel.
 *      PARAMETERS  - NONE
 *      RETURNS     - active channel index
 */ 
uint8_t adcGetActiveCh(void);

/*** adcGetErrorFlag **************************************************************************************************************/
/* This subroutine returns the error status flag to the caller.
 *      PARAMETERS  - NONE
 *      RETURNS     - true if error has occurred
 *                  - false if no error has occurred
 */  
bool adcGetErrorFlag(void);

/*** adcContReadEn ****************************************************************************************************************/
/* This subroutine places instruction into the TX soft buffer i order to enable continuous read mode.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcContReadEn(void);

/*** adcCalIOffset ****************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer in order to calibrate the internal offset.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcCalIOffset(void);

/*** adcCalIFs ********************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to initiate internal full scale calibration. in the process the 
 * offset register is reset to default and the power mode is set to low power.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcCalIFs(void);

/*** adcSetLowPwrMode *************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to enable low power mode.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */ 
void adcSetLowPwrMode(void);

/*** adcSetMidPwrMode *************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to enable mid power mode.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcSetMidPwrMode(void);

/*** adcSetFullPwrMode ************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to enable full power mode.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcSetFullPwrMode(void);

/*** adcSetSRate10 ****************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 10 SPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */ 
void adcSetSRate10(uint8_t ch);

/*** adcSetSRate60 ****************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 60 SPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */  
void adcSetSRate60(uint8_t ch);

/*** adcSetSRate100 ***************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 100 SPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */  
void adcSetSRate100(uint8_t ch);

/*** adcSetSRate1000 **************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 1 kSPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */  
void adcSetSRate1000(uint8_t ch);

/*** adcSetSRate4800 **************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 4.8 kSPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */   
void adcSetSRate4800(uint8_t ch);

/*** adcSetSRate9600 **************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 9.6 kSPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */   
void adcSetSRate9600(uint8_t ch);

/*** adcSetSRate19200 *************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to set the sampling rate to 19.2 kSPS for a specified channel.
 *      PARAMETERS  - CH
 *      RETURNS     - NONE
 */   
void adcSetSRate19200(uint8_t ch);

/*** adcSetSRate ******************************************************************************************************************/
/* This subroutine runs the respective subroutine to set the sampling rate of the adc for a specified channel.
 *      PARAMETERS  - sampling rate lookup value, CH
 *      RETURNS     - NONE
 */  
void adcSetSRate(uint8_t sRate, uint8_t chToSet);

/*** adcChOn **********************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to enable the selected channel
 *      PARAMETERS  - channel number
 *      RETURNS     - NONE
 */  
void adcChOn(uint8_t ch);

/*** adcChOff *********************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to disable the selected channel
 *      PARAMETERS  - channel number
 *      RETURNS     - NONE
 */  
void adcChOff(uint8_t ch);

/*** adcShuntOn *******************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to enable the shunt to ground.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcShuntOn(void);

/*** adcShuntOff ******************************************************************************************************************/
/* This subroutine places instructions into the TX soft buffer to disable the shunt to ground.
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */  
void adcShuntOff(void);

/*** adcWrAdcCtrlDone *************************************************************************************************************/
/* This subroutine returns the status of ADC_CONTROL register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */  
bool adcWrAdcCtrlDone(void);

/*** adcWrChDone ******************************************************************************************************************/
/* This subroutine returns the status of CH_n register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */  
bool adcWrChDone(void);

/*** adcWrDoneConfig **************************************************************************************************************/
/* This subroutine returns the status of CONFIG_n register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */  
bool adcWrDoneConfig(void);

/*** adcWrDoneErrorEn *************************************************************************************************************/
/* This subroutine returns the status of ERROR_EN register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */  
bool adcWrDoneErrorEn(void);

/*** adcWrFilterDone **************************************************************************************************************/
/* This subroutine returns the status of FILTER_n register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */   
bool adcWrFilterDone(void);

/*** adcWrGainDone ****************************************************************************************************************/
/* This subroutine returns the status of GAIN_n register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */  
bool adcWrGainDone(void);

/*** adcWrIoCtrl1Done *************************************************************************************************************/
/* This subroutine returns the status of IO_CONTROL_1 register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */   
bool adcWrIoCtrl1Done(void);

/*** adcWrIoCtrl2Done *************************************************************************************************************/
/* This subroutine returns the status of IO_CONTROL_2 register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */    
bool adcWrIoCtrl2Done(void);

/*** adcWrOffsetDone **************************************************************************************************************/
/* This subroutine returns the status of OFFSET_n register write.
 *      PARAMETERS  - NONE
 *      RETURNS     - true is write is done
 *                  - false if write is not done
 */   
bool adcWrOffsetDone(void);

/*** adcService *******************************************************************************************************************/
/* This subroutine parses the information in the TX soft buffer and executes the instructions. If there are no instructions, it
 * reads conversion data from the ADC. 
 *      PARAMETERS  - NONE
 *      RETURNS     - NONE
 */   
void adcService(void);

/*** adcCopyRxBuf *****************************************************************************************************************/
/* This subroutine copies data from the soft RX buffer into a global buffer. The process continues until there is no more data
 * to copy or until the target pointer is full.
 *      PARAMETERS  - pointer to external buffer
 *                  - pointer to external buffer leading index
 *      RETURNS     - NONE
 */   
void adcCopyRxBuf(uint8_t *buf, uint8_t *end, uint8_t chNum);

/*** adcCopy24To16Bit *************************************************************************************************************/
/* This subroutine copies data from the soft RX buffer into a global buffer. The samples taken are 24-bit and they are decimated to
 * 16-bit in the process. The execution continues until there is no more valid data to copy or until the target pointer is full.
 *      PARAMETERS  - pointer to external buffer
 *                  - pointer to external buffer leading index
 *      RETURNS     - NONE
 */ 
void adcCopy24To16Bit(uint8_t *buf, uint8_t *end, uint8_t chNum);

/*** adcChNewData *****************************************************************************************************************/
/* This subroutine checks to see if enough bytes have been read by the ADC for a conversion of the specified channel. It returns 
 * true if the data is collected and false if it is not. 
 *      PARAMETERS  - channel number
 *      RETURNS     - success or failure
 */   
bool adcChNewData(uint8_t chNum);

/*** adcGetChData *****************************************************************************************************************/
/* This subroutine checks for new data from the specified channel. If available, it will write it to the passed variable and return
 * successful. Else it will quit and return false.
 *      PARAMETERS  - channel number, pointer to variable to write to
 *      RETURNS     - success or failure
 */   
bool adcGetChData(uint8_t chNum, uint32_t *var);

#endif                                                                          // End _AD7124_8_H

