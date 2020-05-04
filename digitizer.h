#ifndef _DIGITIZER_H  
#define _DIGITIZER_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "AD7124-8.h"
#include "AD9833.h"
#include "ADG704.h"
#include "ADG731_1.h"
#include "ADG731_2.h"
#include "ADG731_3.h"
#include "ADG731_4.h"

/**********************************************************************************************************************************/
/*=== MACROS =====================================================================================================================*/
/**********************************************************************************************************************************/
#define digiAdDwSize()                  2                                       // Bytes in ADC data word

//--- CHANNELS --------------------------------------------------------------------------------------------------------------------
#define digiMinCh()                     0                                       // Digitizer lowest channel
#define digiMaxCh()                     99                                      // Digitizer highest channel identifier
#define digiNumCh()                     (digiMaxCh() + 1)                       // Digitizer total number of channels

//--- SW IDENTIFIERS --------------------------------------------------------------------------------------------------------------
#define digiSw1()                       0x00                                    // Digitizer SW 1 identifier
#define digiSw2()                       0x01                                    // Digitizer SW 2 identifier
#define digiSw3()                       0x02                                    // Digitizer SW 3 identifier
#define digiSw4()                       0x03                                    // Digitizer SW 4 identifier
#define digiSwMax()                     digiSw4()                               // Digitizer SW max identifier

//--- DIGITIZER STATE MACHINES ----------------------------------------------------------------------------------------------------
#define digiDeflt()                     0x00                                    // Digitizer default state
#define digiSmGetData()                 0x01                                    // Get data state (ADCs only)
#define digiSmSendData()                0x11                                    // Send data state (DAC only)
#define digiSmWait()                    0x02                                    // Wait state
#define digiSmAd1()                     0x01                                    // Digitizer ADC 1 state machine
#define digiSmAd2()                     0x02                                    // Digitizer ADC 2 state machine
#define digiSmDac()                     0x03                                    // Digitizer DAC state machine

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/

//--- digiSrc_obj -----------------------------------------------------------------------------------------------------------------
typedef struct{
    const   uint8_t muxCh;                                                      // Output mux channel identifier
    const   uint8_t swNum;                                                      // Switch number identifier
    const   uint8_t swCh;                                                       // Switch channel identifier            
            uint8_t rate;                                                       // Sampling rate setting
            uint8_t rateReq;                                                    // Sampling rate requested
            int32_t data;                                                       // Data    
            bool    smplReq;                                                    // Sampling enable / disable demand flag
            bool    smplEn;                                                     // Sampling enable / disable flag
            bool    updCfg;                                                     // Configuration updated flag    
    volatile bool   newData;                                                    // New data available flag
} digiSrc_obj;

//--- active_obj ------------------------------------------------------------------------------------------------------------------
typedef struct{
    uint8_t adObjNum;                                                           // ADC channel selector
} active_obj;                                                                   // Currently used digitizer settings structure

//--- digi_obj --------------------------------------------------------------------------------------------------------------------
typedef struct{
//    const   enums_obj   enums;                                                  // Enumerated constants
            active_obj  active;                                                 // Currently used digitizer settings object for ADC 1 and ADC 2
//            Done_obj    done;                                                   // Tasks complete flags object
            uint8_t     adState;                                                // Digitizer AD state machine index
} digi_obj;


#endif                                                                          // End _DIGITIZER_H