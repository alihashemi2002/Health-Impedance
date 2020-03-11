/*=== DEVCFG3 ====================================================================================================================*/
// USERID = No Setting
#pragma config FMIIEN    = OFF                                                  // Ethernet RMII/MII Enable (RMII Enabled)
#pragma config FETHIO    = OFF                                                  // Ethernet I/O Pin Select (Alternate Ethernet I/O)
#pragma config PGL1WAY   = OFF                                                  // Permission Group Lock One Way Configuration (Allow multiple reconfigurations)
#pragma config PMDL1WAY  = OFF                                                  // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY   = ON                                                   // Peripheral Pin Select Configuration (Allow only one reconfiguration)
#pragma config FUSBIDIO  = ON                                                   // USB USBID Selection (Controlled by the USB Module)

/*=== DEVCFG2 ====================================================================================================================*/
#pragma config FPLLICLK  = PLL_FRC                                              // PLL clock source (FRC)
#pragma config FPLLIDIV  = DIV_1                                                // PLL input diviser (1)
#pragma config FPLLRNG   = RANGE_5_10_MHZ                                       // PLL range (5-10MHz)
#pragma config FPLLMULT  = MUL_50                                               // PLL multiplier (50)
#pragma config FPLLODIV  = DIV_4                                                // PLL outpuet diviser (4)
#pragma config UPLLFSEL  = FREQ_12MHZ                                           // USB PLL frequency select (12MHz)

/*=== DEVCFG1 ====================================================================================================================*/
#pragma config FNOSC     = SPLL                                                 // Oscillator Selection Bits (System PLL)
#pragma config DMTINTV   = WIN_127_128                                          // DMT Count Window Interval (Window/Interval value is 127/128 counter value)
#pragma config FSOSCEN   = OFF                                                  // Secondary Oscillator Enable (Disable SOSC)
#pragma config IESO      = OFF                                                  // Internal/External Switch Over (Disabled)
#pragma config POSCMOD   = OFF                                                   // Primary Oscillator Configuration (External clock mode)
#pragma config OSCIOFNC  = OFF                                                  // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FCKSM     = CSECME                                               // Clock Switching and Monitor Selection (Clock Switch Enabled, FSCM Enabled)
#pragma config WDTPS     = PS1048576                                            // Watchdog Timer Postscaler (1:1048576)
#pragma config WDTSPGM   = STOP                                                 // Watchdog Timer Stop During Flash Programming (WDT stops during Flash programming)
#pragma config WINDIS    = NORMAL                                               // Watchdog Timer Window Mode (Watchdog Timer is in non-Window mode)
#pragma config FWDTEN    = OFF                                                  // Watchdog Timer Enable (WDT Disabled)
#pragma config FWDTWINSZ = WINSZ_25                                             // Watchdog Timer Window Size (Window size is 25%)
#pragma config DMTCNT    = DMT31                                                // Deadman Timer Count Selection (2^31 (2147483648))
#pragma config FDMTEN    = OFF                                                  // Deadman Timer Enable (Deadman Timer is disabled)

/*=== DEVCFG0 ====================================================================================================================*/
#pragma config DEBUG     = OFF                                                  // Background Debugger Enable (Debugger is disabled)
#pragma config JTAGEN    = OFF                                                  // JTAG Enable (JTAG Disabled)
#pragma config ICESEL    = ICS_PGx1                                             // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config TRCEN     = ON                                                   // Trace Enable (Trace features in the CPU are enabled)
#pragma config BOOTISA   = MIPS32                                               // Boot ISA Selection (Boot code and Exception code is MIPS32)
#pragma config FECCCON   = OFF_UNLOCKED                                         // Dynamic Flash ECC Configuration (ECC and Dynamic ECC are disabled (ECCCON bits are writable))
#pragma config FSLEEP    = OFF                                                  // Flash Sleep Mode (Flash is powered down when the device is in Sleep mode)
#pragma config DBGPER    = PG_ALL                                               // Debug Mode CPU Access Permission (Allow CPU access to all permission regions)
#pragma config SMCLR     = MCLR_NORM                                            // Soft Master Clear Enable bit (MCLR pin generates a normal system Reset)
#pragma config SOSCGAIN  = GAIN_2X                                              // Secondary Oscillator Gain Control bits (2x gain setting)
#pragma config SOSCBOOST = OFF                                                  // Secondary Oscillator Boost Kick Start Enable bit (Normal start of the oscillator)
#pragma config POSCGAIN  = GAIN_1X                                              // Primary Oscillator Gain Control bits (1x gain setting)
#pragma config POSCBOOST = OFF                                                  // Primary Oscillator Boost Kick Start Enable bit (Normal start of the oscillator)
#pragma config EJTAGBEN  = NORMAL                                               // EJTAG Boot (Normal EJTAG functionality)

/*=== DEVCP0 ====================================================================================================================*/
#pragma config CP = OFF                                                         // Code Protect (Protection Disabled)

/*=== SEQ3 =======================================================================================================================*/

/*=== DEVADC0 ====================================================================================================================*/

/*=== DEVADC1 ====================================================================================================================*/

/*=== DEVADC2 ====================================================================================================================*/

/*=== DEVADC3 ====================================================================================================================*/

/*=== DEVADC4 ====================================================================================================================*/

/*=== DEVADC7 ====================================================================================================================*/

#include "system.h"
#include "globals.h"

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** SYSTEM_Initialize ************************************************************************************************************/
void SYSTEM_Initialize(void){
    uint16_t n;
    sysConfigClocks();                                                          // Initialize system clocks
    pinManagerInitialize();                                                     // Initialize I/O
    interruptInitialize();                                                      // Initialize interrupt controller
    promInitialize();                                                           // Initialize PROM
//    adcInitialize();                                                            // Initialize ADC
    ddsInitialize();                                                            // Initialize DDS
//    extInt2Initialize();                                                        // Initialize external interrupt 2
//    muxInitialize();                                                            // Initialize mux
//    sw1Initialize();                                                            // Initialize switch 1
    tmr1Initialize();                                                           // Initialize timer 1
    uart2Initialize();                                                          // Initialize UART2
}//end SYSTEM_Initialize()

/*** sysConfigClocks **************************************************************************************************************/
void sysConfigClocks(void){
    /*--- PERIPHERAL CLOCKS ------------------------------------------------------------------------------------------------------*/
    SYSKEY = 0xAA996655;                                                        //UNLOCKING SEQUENCE
    SYSKEY = 0x556699AA;
    
    // Peripheral Bus 2 - used by SPI and UART modules
    PB2DIVbits.ON = 0;                                                          // Disable Peripheral Bus 2
    while(!PB2DIVbits.PBDIVRDY){}                                               // Wait for bus to go off
    PB2DIV = 0x8001;                                                            // Enable Peripheral Bus 2; SYSCLK DIV = 2 (50MHz)
    
    // Peripheral Bus 3 - used by Timer and OC modules
    PB3DIVbits.ON = 0;                                                          // Disable Peripheral Bus 3
    while(!PB3DIVbits.PBDIVRDY){}                                               // Wait for bus to go off
    PB3DIV = 0x8013;                                                            // Enable Peripheral Bus 3; SYSCLK DIV = 20 (5MHz)
    
    // Peripheral Bus 4 - used by PORT IO module
    PB4DIVbits.ON = 0;                                                          // Disable Peripheral Bus 4
    while(!PB4DIVbits.PBDIVRDY){}                                               // Wait for bus to go off
    PB4DIV = 0x8003;                                                            // Enable Peripheral Bus 4; SYSCLK DIV = 4 (25MHz)
    
    SYSKEY = 0x33333333;                                                        //LOCKING SEQUENCE
}//end sysConfigClocks()