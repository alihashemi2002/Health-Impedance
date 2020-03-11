#ifndef _AD9833_H
#define _AD9833_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/**********************************************************************************************************************************/
/*=== PIN ASSIGNMENTS ============================================================================================================*/
/**********************************************************************************************************************************/
#define ddsCsPin()                      (LATDbits.LATD14)                       // CS pin
#define ddsSpiCloseFrame()              (ddsCsPin() = 1)                        // Close SPI frame
#define ddsSpiOpenFrame()               (ddsCsPin() = 0)                        // Open SPI frame

/**********************************************************************************************************************************/
/*=== SYSTEM ASSIGNMENTS =========================================================================================================*/
/**********************************************************************************************************************************/
#define ddsSpiBuf()                     SPI6BUF                                 // SPI buffer
#define ddsSpiBusy()                    SPI6STATbits.SPIBUSY                    // SPI busy if true
#define ddsSpiRxFifoEmpty()             SPI6STATbits.SPIRBE                     // SPI rx empty if true
#define ddsSpiTxFifoEmpty()             SPI6STATbits.SPITBE                     // SPI tx empty if true
#define ddsSpiCon()                     (SPI6CON)                               // SPIxCON associated with DDS allocated SPI port
#define ddsSpiCon2()                    (SPI6CON2)                              // SPIxCON2 associated with DDS allocated SPI port
#define ddsSpiBrg()                     (SPI6BRG)                               // SPIxBRG associated with DDS allocated SPI port

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define ddsBufSize()                    (UCHAR_MAX+1)                           // Software TX buffer length
#define ddsNumFreqReg()                 2                                       // Number of frequency registers
#define ddsNumPhaseReg()                2                                       // Number of phase registers
#define ddsMaxFreq()                    0x0FFFFFFF                              // Maximum output frequency
#define ddsMaxPhase()                   0x0FFF                                  // Maximum output frequency

/*=== DDS REGISTER ADDRESSES =====================================================================================================*/
#define ddsAddrCntrl()                  0x0000                                  // Control register address
#define ddsAddrFreq0()                  0x4000                                  // Frequency 0 register address
#define ddsAddrFreq1()                  0x8000                                  // Frequency 1 register address
#define ddsAddrPhase0()                 0xC000                                  // Phase 0 register address
#define ddsAddrPhase1()                 0xE000                                  // Phase 1 register address

/*=== DDS REGISTER ADDRESSES =====================================================================================================*/
#define ddsWrAddrCntrl()                1                                       // Control register address identifier
#define ddsWrAddrFreq0()                2                                       // Frequency 0 register address identifier
#define ddsWrAddrFreq1()                3                                       // Frequency 1 register address identifier
#define ddsWrAddrPhase0()               4                                       // Phase 0 register address identifier
#define ddsWrAddrPhase1()               5                                       // Phase 1 register address identifier

/*=== DDS REGISTER DEFAULTS ======================================================================================================*/
#define ddsCntrlRstDflt()               0x2100                                  // B28_ON; HLB_OFF; FSEL_0; PSEL_0; RST_ON; SLP1_OFF; SLP12_OFF; OP_0; DIV2_0; MODE_SIN;
#define ddsCntrlDflt()                  0x2000                                  // B28_ON; HLB_OFF; FSEL_0; PSEL_0; RST_OFF; SLP1_OFF; SLP12_OFF; OP_0; DIV2_0; MODE_SIN;
#define ddsFreq0Dflt()                  0x400050C7                              // 400Hz
#define ddsFreq1Dflt()                  0x828F980F                              // 1MHz
#define ddsPhase0Dflt()                 0xC000                                  // 0 phase
#define ddsPhase1Dflt()                 0xE000                                  // 0 phase

/*=== DDS STATE MACHINE STATES ===================================================================================================*/
#define ddsSmStateInitial()             0x00                                    // State machine initial state identifier

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/

//--- dds_obj ---------------------------------------------------------------------------------------------------------------------
typedef struct{
    uint8_t  tx[ddsBufSize()];                                                  // TX buffer
    uint8_t  txStart;                                                           // TX buffer lagging index
    uint8_t  txEnd;                                                             // TX buffer leading index
    uint8_t  smState;                                                           // State machine state
    uint16_t writing;                                                           // Register that is being written
} dds_obj;

//--- ddsReg_obj ------------------------------------------------------------------------------------------------------------------
typedef struct{
    uint16_t cntrl;                                                             // Control register mirror
    uint32_t freq0;                                                             // Frequency 0 register mirror (28-bit)
    uint32_t freq1;                                                             // Frequency 1 register mirror (28-bit)
    uint16_t phase0;                                                            // Phase 0 register mirror
    uint16_t phase1;                                                            // Phase 1 register mirror
} ddsReg_obj;

//--- ddsDone_obj -----------------------------------------------------------------------------------------------------------------
typedef struct{
    bool     cntrl;                                                             // Control register write done flag
    bool     freq0;                                                             // Frequency 0 register write done flag
    bool     freq1;                                                             // Frequency 1 register write done flag
    bool     phase0;                                                            // Phase 0 register write done flag
    bool     phase1;                                                            // Phase 1 register write done flag
} ddsDone_obj;

/**********************************************************************************************************************************/
/*=== FUNCTION PROTOTYPES ========================================================================================================*/
/**********************************************************************************************************************************/

/*** ddsSpiInit ******************************************************************************************************************/
void ddsSpiInit(void);

/*** ddsRxFifoClear ***************************************************************************************************************/ 
void ddsRxFifoClear(void);

/*** ddsInitialize ****************************************************************************************************************/
void ddsInitialize(void);

/*** ddsFlagSet *******************************************************************************************************************/
void ddsFlagSet(void);

/*** ddsWrCntrlReg ****************************************************************************************************************/
void ddsWrCntrlReg(void);

/*** ddsWrFreq0Reg ****************************************************************************************************************/
void ddsWrFreq0Reg(void);

/*** ddsWrFreq1Reg ****************************************************************************************************************/
void ddsWrFreq1Reg(void);

/*** ddsWrPhase0Reg ***************************************************************************************************************/
void ddsWrPhase0Reg(void);

/*** ddsWrPhase1Reg ***************************************************************************************************************/
void ddsWrPhase1Reg(void);

/*** ddsSetFreq0 ******************************************************************************************************************/
bool ddsSetFreq0(uint32_t freq);

/*** ddsSetFreq1 ******************************************************************************************************************/
bool ddsSetFreq1(uint32_t freq);

/*** ddsSetPhase0 *****************************************************************************************************************/
bool ddsSetPhase0(uint16_t phase);

/*** ddsSetPhase1 *****************************************************************************************************************/
bool ddsSetPhase1(uint16_t phase);

/*** ddsTxBytes *******************************************************************************************************************/
uint8_t ddsTxBytes(void);

/*** ddsService *******************************************************************************************************************/
void ddsService(void);

#endif                                                                          // End _AD9833_H