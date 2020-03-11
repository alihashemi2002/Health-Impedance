#include "AD7124-8.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
adc_obj      adc;                                                               // ADC variables
adcReg_obj   adcReg;                                                            // ADC registers
adcFlags_obj adcDone;                                                           // ADC done flags

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** adcSpiInit ******************************************************************************************************************/
void adcSpiInit(void){

}//end adcSpiInit()

/*** adcRxFifoClear ***************************************************************************************************************/ 
void adcRxFifoClear(void){
    uint8_t temp;
    while(!adcSpiRxFifoEmpty()) {temp = adcSpiBuf();}
}//end adcRxFifoClear()

/*** adcInitialize ****************************************************************************************************************/
void adcInitialize(void){
    uint8_t n;                                                                  // Counter
    //--- CYCLE ANALOG POWER -------------------------------------------------------------------------------------------------------
    adcSyncClr();                                                               // Clear ADC SYNC to prevent damage
        
    //--- SPI CONFIGURATION --------------------------------------------------------------------------------------------------------
    adcSpiCloseFrame();                                                         // Close SPI frame
    adcSyncSet();                                                               // Pull up SYNC pin
    adcSpiInit();                                                               // Initialize SPI port for adc use
    
    //--- VARIABLES ----------------------------------------------------------------------------------------------------------------
    adc.txStart  = 0;                                                           // Initialize TX buffer trailing index
    adc.txEnd    = 0;                                                           // Initialize TX buffer leading index
    for(n = 0; n <= adcMaxChIndex(); n++){                                      // Initialize all channels
        adc.ch[n].rxStart  = 0;                                                 // Initialize RX buffer trailing index
        adc.ch[n].rxEnd    = 0;}                                                // Initialize RX buffer leading index
    adc.writing  = 0;                                                           // No written register
    adc.reading  = 0;                                                           // No currently read register
    adc.bToRead  = 0;                                                           // No bytes to read
    adc.crntCh   = adcCh0();                                                    // Channel 0 converted first
    adc.statusEn = false;                                                       // Status byte not enabled
    
    //--- ADC CONFIG ---------------------------------------------------------------------------------------------------------------
    adcWrReg(adcAddrControl(),  adcControlDflt());                              // Write control register default
    
    adcWrReg(adcAddrCh0(),      adcCh0Dflt());                                  // Write CH0 register default
    adcWrReg(adcAddrConfig0(),  adcConfig0Dflt());                              // Write configuration 0 register default
    adcWrReg(adcAddrFilter0(),  adcFilter0Dflt());                              // Write filter 0 register default
    
    adcWrReg(adcAddrControl(),  adcControlCalGainDflt());                       // Perform internal gain calibration on low power mode
    adcWrReg(adcAddrControl(),  adcControlCalOfstDftl());                       // Perform internal offset calibration on low power mode
    
    adcWrReg(adcAddrControl(),  adcControlDflt());                              // Write control register default and start sampling
    
    adcSpiOpenFrame();                                                          // Open SPI frame
}//end adcInitialize()

/*** adcSizeOfReg *****************************************************************************************************************/
uint8_t adcSizeOfReg(uint8_t addr){
    //--- Return byte length
    if(addr == adcAddrComms())                     return adcRlenComms();       // Return communication register size
    if(addr == adcAddrStatus())                    return adcRlenStatus();      // Return status register size    
    if(addr == adcAddrControl())                   return adcRlenControl();     // Return control register size
    if(addr == adcAddrData())                      return adcRlenData();        // Return data register size
    if(addr == adcAddrIoCtrl1())                   return adcRlenIoCtrl1();     // Return io control 1 register size
    if(addr == adcAddrIoCtrl2())                   return adcRlenIoCtrl2();     // Return io control 2 register size
    if(addr == adcAddrId())                        return adcRlenId();          // Return id register size
    if(addr == adcAddrError())                     return adcRlenError();       // Return error register size
    if(addr == adcAddrErrorEn())                   return adcRlenErrorEn();     // Return error enable register size
    if(addr == adcAddrMclkCnt())                   return adcRlenMclkCnt();     // Return clock count register size
    if(addr <= (adcAddrCh0()+adcMaxChIndex()))     return adcRlenCh();          // Return channel register size
    if(addr <= (adcAddrConfig0()+adcMaxCfgIndex()))return adcRlenConfig();      // Return configuration register size
    if(addr <= (adcAddrGain0()+adcMaxGainIndex())) return adcRlenGain();        // Return gain register size
    return 0;                                                                   // Error due to invalid parameter entry
}//end adcSizeOfReg()

/*** adcLogReg ********************************************************************************************************************/
void adcLogReg(uint8_t addr, uint32_t data){
    switch(addr){
        case adcAddrControl():
            adcReg.adcControl   = data;                                         // Log ADC_CONTROL
            if(adcReg.adcControl & 0x0400) adc.statusEn = true;                 // Check if STATUS_BYTE is enabled; Set status enabled flag
            else adc.statusEn = false;                                          // STATUS_BYTE not enabled; Clear status enabled flag
            break;                                                              // Exit
        case adcAddrIoCtrl1():      adcReg.ioControl1   = data; break;          // Log IO_CONTROL_1
        case adcAddrIoCtrl2():      adcReg.ioControl2   = data; break;          // Log IO_CONTROL_2
        case adcAddrErrorEn():      adcReg.errorEn      = data; break;          // Log ERROR_EN
        case adcAddrCh0():          adcReg.ch0          = data; break;          // Log CHANNEL_0
        case (adcAddrCh0()+1):      adcReg.ch1          = data; break;          // Log CHANNEL_1
        case (adcAddrCh0()+2):      adcReg.ch2          = data; break;          // Log CHANNEL_2
        case (adcAddrCh0()+3):      adcReg.ch3          = data; break;          // Log CHANNEL_3
        case (adcAddrCh0()+4):      adcReg.ch4          = data; break;          // Log CHANNEL_4
        case (adcAddrCh0()+5):      adcReg.ch5          = data; break;          // Log CHANNEL_5
        case (adcAddrCh0()+6):      adcReg.ch6          = data; break;          // Log CHANNEL_6
        case (adcAddrCh0()+7):      adcReg.ch7          = data; break;          // Log CHANNEL_7
        case (adcAddrCh0()+8):      adcReg.ch8          = data; break;          // Log CHANNEL_8
        case (adcAddrCh0()+9):      adcReg.ch9          = data; break;          // Log CHANNEL_9
        case (adcAddrCh0()+10):     adcReg.ch10         = data; break;          // Log CHANNEL_10
        case (adcAddrCh0()+11):     adcReg.ch11         = data; break;          // Log CHANNEL_11
        case (adcAddrCh0()+12):     adcReg.ch12         = data; break;          // Log CHANNEL_12
        case (adcAddrCh0()+13):     adcReg.ch13         = data; break;          // Log CHANNEL_13
        case (adcAddrCh0()+14):     adcReg.ch14         = data; break;          // Log CHANNEL_14
        case (adcAddrCh0()+15):     adcReg.ch15         = data; break;          // Log CHANNEL_15
        case adcAddrConfig0():      adcReg.config0      = data; break;          // Log CONFIG_0
        case (adcAddrConfig0()+1):  adcReg.config1      = data; break;          // Log CONFIG_1
        case (adcAddrConfig0()+2):  adcReg.config2      = data; break;          // Log CONFIG_2
        case (adcAddrConfig0()+3):  adcReg.config3      = data; break;          // Log CONFIG_3
        case (adcAddrConfig0()+4):  adcReg.config4      = data; break;          // Log CONFIG_4
        case (adcAddrConfig0()+5):  adcReg.config5      = data; break;          // Log CONFIG_5
        case (adcAddrConfig0()+6):  adcReg.config6      = data; break;          // Log CONFIG_6
        case (adcAddrConfig0()+7):  adcReg.config7      = data; break;          // Log CONFIG_7
        case adcAddrFilter0():      adcReg.filter0      = data; break;          // Log FILTER_0
        case (adcAddrFilter0()+1):  adcReg.filter1      = data; break;          // Log FILTER_1
        case (adcAddrFilter0()+2):  adcReg.filter2      = data; break;          // Log FILTER_2
        case (adcAddrFilter0()+3):  adcReg.filter3      = data; break;          // Log FILTER_3
        case (adcAddrFilter0()+4):  adcReg.filter4      = data; break;          // Log FILTER_4
        case (adcAddrFilter0()+5):  adcReg.filter5      = data; break;          // Log FILTER_5
        case (adcAddrFilter0()+6):  adcReg.filter6      = data; break;          // Log FILTER_6
        case (adcAddrFilter0()+7):  adcReg.filter7      = data; break;          // Log FILTER_7
        case adcAddrOffset0():      adcReg.offset0      = data; break;          // Log OFFSET_0
        case (adcAddrOffset0()+1):  adcReg.offset1      = data; break;          // Log OFFSET_1
        case (adcAddrOffset0()+2):  adcReg.offset2      = data; break;          // Log OFFSET_2
        case (adcAddrOffset0()+3):  adcReg.offset3      = data; break;          // Log OFFSET_3
        case (adcAddrOffset0()+4):  adcReg.offset4      = data; break;          // Log OFFSET_4
        case (adcAddrOffset0()+5):  adcReg.offset5      = data; break;          // Log OFFSET_5
        case (adcAddrOffset0()+6):  adcReg.offset6      = data; break;          // Log OFFSET_6
        case (adcAddrOffset0()+7):  adcReg.offset7      = data; break;          // Log OFFSET_7
        case adcAddrGain0():        adcReg.gain0        = data; break;          // Log GAIN_0
        case (adcAddrGain0()+1):    adcReg.gain1        = data; break;          // Log GAIN_1
        case (adcAddrGain0()+2):    adcReg.gain2        = data; break;          // Log GAIN_2
        case (adcAddrGain0()+3):    adcReg.gain3        = data; break;          // Log GAIN_3
        case (adcAddrGain0()+4):    adcReg.gain4        = data; break;          // Log GAIN_4
        case (adcAddrGain0()+5):    adcReg.gain5        = data; break;          // Log GAIN_5
        case (adcAddrGain0()+6):    adcReg.gain6        = data; break;          // Log GAIN_6
        case (adcAddrGain0()+7):    adcReg.gain7        = data; break;          // Log GAIN_7
        default:                                                break;          // Exit switch
    }//end switch
}//end adcLogReg()

/*** adcFlagSet *******************************************************************************************************************/
void adcFlagSet(void){
    if      (adc.writing > adcAddrOffset7())  adcDone.wrGain      = true;       // GAIN_n write done
    else if (adc.writing > adcAddrFilter7())  adcDone.wrOffset    = true;       // OFFSET_n write done
    else if (adc.writing > adcAddrConfig7())  adcDone.wrFilter    = true;       // FILTER_n write done
    else if (adc.writing > adcAddrCh15())     adcDone.wrConfig    = true;       // CONFIG_n write done
    else if (adc.writing > adcAddrMclkCnt())  adcDone.wrCh        = true;       // CHANNEL_n write done
    else{
        switch(adc.writing){
            case adcAddrControl():  adcDone.wrAdcControl    = true; break;      // ADC_CONTROL write done
            case adcAddrIoCtrl1():  adcDone.wrIoControl1    = true; break;      // IO_CONTROL_1 write done
            case adcAddrIoCtrl2():  adcDone.wrIoControl2    = true; break;      // IO_CONTROL_2 write done
            case adcAddrErrorEn():  adcDone.wrErrorEn       = true; break;      // ERROR_EN write done
            default:                                                break;      // Exit switch
        }//end switch
    }//end else
}//end adcSizeOfReg()

/*** adcWrReg *********************************************************************************************************************/
void adcWrReg(uint8_t address, uint32_t data){
    uint8_t bytes = adcSizeOfReg(address);                                      // Get register size in bytes
    if(bytes > 0){                                                              // Check if passed address is valid
        adc.tx[adc.txEnd++] = adcCmdWrite();                                    // Write instruction -> TX buffer
        adc.tx[adc.txEnd++] = address;                                          // Address -> TX buffer
        adc.tx[adc.txEnd++] = bytes;                                            // Data size in bytes -> TX buffer
        while(bytes > 0) adc.tx[adc.txEnd++] = (data >> (8*(--bytes)));         // Write to TX soft buffer
    }//end if
    adcLogReg(address, data);                                                   // Log data into corresponding mirror register
}//end adcWrReg()

/*** adcRdReg *********************************************************************************************************************/
void adcRdReg(uint8_t addr){
    uint8_t bytes = adcSizeOfReg(addr);                                         // Get register size in bytes
    if(bytes > 0){                                                              // Check if passed address is valid
        adc.tx[adc.txEnd++] = adcCmdRead();                                     // Read instruction to TX buffer
        adc.tx[adc.txEnd++] = addr;                                             // Address TX buffer
        adc.tx[adc.txEnd++] = bytes;                                            // Get register size in bytes
    }//end if
}//end adcRdReg()

/*** adcSpiToMirrorReg ************************************************************************************************************/
void adcSpiToMirrorReg(void){
    uint8_t garbage;                                                            // Variable to read garbage
    if(adc.reading == adcAddrData()){                                           // Data read operation
        garbage = adcSpiBuf();                                                  // Read garbage
        if(adc.statusEn){                                                       // Check if status byte is enabled
            adc.crntCh = adcSpiBuf();                                           // Read status byte
            adc.crntCh &= 0x0F;                                                 // Remove all parameters of status byte but the current channel
            adc.ch[adc.crntCh].rx[adc.ch[adc.crntCh].rxEnd++] = adcSpiBuf();    // Read byte 2
            adc.ch[adc.crntCh].rx[adc.ch[adc.crntCh].rxEnd++] = adcSpiBuf();    // Read byte 1
            adc.ch[adc.crntCh].rx[adc.ch[adc.crntCh].rxEnd++] = adcSpiBuf();}   // Read byte 0
        else{                                                                   // Status byte not enabled--assume only reading channel 0
            adc.ch[adcCh0()].rx[adc.ch[adcCh0()].rxEnd++] = adcSpiBuf();        // Read byte 2
            adc.ch[adcCh0()].rx[adc.ch[adcCh0()].rxEnd++] = adcSpiBuf();        // Read byte 1
            adc.ch[adcCh0()].rx[adc.ch[adcCh0()].rxEnd++] = adcSpiBuf();}}      // Read byte 0
    else{                                                                       // All other reads
        uint32_t temp = adcSpiBuf();                                            // Temporary data buffer; get garbage
        temp = 0;                                                               // Clear temp buffer
        while(adc.bToRead > 0){                                                 // Get data
            temp |= adcSpiBuf();                                                // SPI RX FIFO to local data buffer; MSB first
            adc.bToRead--;                                                      // Decrement number of bytes to read
            if(adc.bToRead > 0)temp <<= 8;                                      // Left shift if there is more to read
        }//end while
        switch(adc.reading){
            case adcAddrStatus():       adcReg.status           = temp;         // Get STATUS
                                        adcDone.rdStatus        = true; break;  // STATUS read done
            case adcAddrControl():      adcReg.adcControl       = temp;         // Get ADC_CONTROL
                                        adcDone.rdAdcControl    = true; break;  // ADC_CONTROL read done
            case adcAddrIoCtrl1():      adcReg.ioControl1       = temp;         // Get IO_CONTROL_1
                                        adcDone.rdIoControl1    = true; break;  // IO_CONTROL_1 read done
            case adcAddrIoCtrl2():      adcReg.ioControl2       = temp;         // Get IO_CONTROL_2
                                        adcDone.rdIoControl2    = true; break;  // IO_CONTROL_2 read done
            case adcAddrId():           adcReg.id               = temp;         // Get ID
                                        adcDone.rdId            = true; break;  // ID read done
            case adcAddrError():        adcReg.error            = temp;         // Get ERROR
                                        adcDone.rdError         = true; break;  // ERROR read done
            case adcAddrErrorEn():      adcReg.errorEn          = temp;         // Get ERROR_EN
                                        adcDone.rdErrorEn       = true; break;  // EROR_EN read done
            case adcAddrMclkCnt():      adcReg.mclkCount        = temp;         // Get MCLK_COUNT
                                        adcDone.rdMclkCount     = true; break;  // MCLK_COUNT read done
            case adcAddrCh0():          adcReg.ch0              = temp;         // Get CHANNEL_0
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+1):      adcReg.ch1              = temp;         // Get CHANNEL_1
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+2):      adcReg.ch2              = temp;         // Get CHANNEL_2
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+3):      adcReg.ch3              = temp;         // Get CHANNEL_3
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+4):      adcReg.ch4              = temp;         // Get CHANNEL_4
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+5):      adcReg.ch5              = temp;         // Get CHANNEL_5
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+6):      adcReg.ch6              = temp;         // Get CHANNEL_6
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+7):      adcReg.ch7              = temp;         // Get CHANNEL_7
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+8):      adcReg.ch8              = temp;         // Get CHANNEL_8
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+9):      adcReg.ch9              = temp;         // Get CHANNEL_9
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+10):     adcReg.ch10             = temp;         // Get CHANNEL_10
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+11):     adcReg.ch11             = temp;         // Get CHANNEL_11
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+12):     adcReg.ch12             = temp;         // Get CHANNEL_12
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+13):     adcReg.ch13             = temp;         // Get CHANNEL_13
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+14):     adcReg.ch14             = temp;         // Get CHANNEL_14
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case (adcAddrCh0()+15):     adcReg.ch15             = temp;         // Get CHANNEL_15
                                        adcDone.rdCh            = true; break;  // CHANNEL_n read done
            case adcAddrConfig0():      adcReg.config0          = temp;         // Get CONFIG_0
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+1):  adcReg.config1          = temp;         // Get CONFIG_1
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+2):  adcReg.config2          = temp;         // Get CONFIG_2
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+3):  adcReg.config3          = temp;         // Get CONFIG_3
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+4):  adcReg.config4          = temp;         // Get CONFIG_4
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+5):  adcReg.config5          = temp;         // Get CONFIG_5
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+6):  adcReg.config6          = temp;         // Get CONFIG_6
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case (adcAddrConfig0()+7):  adcReg.config7          = temp;         // Get CONFIG_7
                                        adcDone.rdConfig        = true; break;  // CONFIG_n read done
            case adcAddrFilter0():      adcReg.filter0          = temp;         // Get FILTER_0
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+1):  adcReg.filter1          = temp;         // Get FILTER_1
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+2):  adcReg.filter2          = temp;         // Get FILTER_2
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+3):  adcReg.filter3          = temp;         // Get FILTER_3
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+4):  adcReg.filter4          = temp;         // Get FILTER_4
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+5):  adcReg.filter5          = temp;         // Get FILTER_5
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+6):  adcReg.filter6          = temp;         // Get FILTER_6
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case (adcAddrFilter0()+7):  adcReg.filter7          = temp;         // Get FILTER_7
                                        adcDone.rdFilter        = true; break;  // FILTER_n read done
            case adcAddrOffset0():      adcReg.offset0          = temp;         // Get OFFSET_0
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+1):  adcReg.offset1          = temp;         // Get OFFSET_1
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+2):  adcReg.offset2          = temp;         // Get OFFSET_2
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+3):  adcReg.offset3          = temp;         // Get OFFSET_3
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+4):  adcReg.offset4          = temp;         // Get OFFSET_4
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+5):  adcReg.offset5          = temp;         // Get OFFSET_5
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+6):  adcReg.offset6          = temp;         // Get OFFSET_6
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case (adcAddrOffset0()+7):  adcReg.offset7          = temp;         // Get OFFSET_7
                                        adcDone.rdOffset        = true; break;  // OFFSET_n read done
            case adcAddrGain0():        adcReg.gain0            = temp;         // Get GAIN_0
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+1):    adcReg.gain1            = temp;         // Get GAIN_1
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+2):    adcReg.gain2            = temp;         // Get GAIN_2
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+3):    adcReg.gain3            = temp;         // Get GAIN_3
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+4):    adcReg.gain4            = temp;         // Get GAIN_4
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+5):    adcReg.gain5            = temp;         // Get GAIN_5
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+6):    adcReg.gain6            = temp;         // Get GAIN_6
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            case (adcAddrGain0()+7):    adcReg.gain7            = temp;         // Get GAIN_7
                                        adcDone.rdGain          = true; break;  // GAIN_n read done
            default:                                                    break;  // Exit switch
        }//end switch
    }//end else
}//end adcSpiToShadowReg()

/*** adcRdStatus ******************************************************************************************************************/
void adcRdStatus(void){
    adcRdReg(adcAddrStatus());                                                  // Read STATUS register
    adcDone.rdStatus = false;                                                   // STATUS not yet read
}//end adcRdStatus()

/*** adcRdAdcControl **************************************************************************************************************/
void adcRdAdcControl(void){
    adcRdReg(adcAddrControl());                                                 // Read ADC_CONTROL register
    adcDone.rdAdcControl = false;                                               // ADC_CONTROL not yet read
}//end adcRdAdcControl()

/*** adcDataRead ******************************************************************************************************************/ 
void adcDataRead(void){
    adcRdReg(adcAddrData());                                                    // Read DATA register
    adcDone.rdData = false;                                                     // DATA not yet read
}//end adcDataRead()

/*** adcRdIoControl1 **************************************************************************************************************/
void adcRdIoControl1(void){
    adcRdReg(adcAddrIoCtrl1());                                                 // Read IO_CONTROL_1 register
    adcDone.rdIoControl1 = false;                                               // IO_CONTROL_1 not yet read
}//end adcRdIoControl1()

/*** adcRdIoControl2 **************************************************************************************************************/
void adcRdIoControl2(void){
    adcRdReg(adcAddrIoCtrl2());                                                 // Read IO_CONTROL_2 register
    adcDone.rdIoControl2 = false;                                               // IO_CONTROL_2 not yet read
}//end adcRdIoControl2()

/*** adcRdId **********************************************************************************************************************/
void adcRdId(void){
    adcRdReg(adcAddrId());                                                      // Read ID register
    adcDone.rdId = false;                                                       // ID not yet read
}//end adcRdId()

/*** adcRdError *******************************************************************************************************************/
void adcRdError(void){
    adcRdReg(adcAddrError());                                                   // Read ERROR register
    adcDone.rdError = false;                                                    // ERROR not yet read
}//end adcRdError()

/*** adcRdErrorEn *****************************************************************************************************************/
void adcRdErrorEn(void){
    adcRdReg(adcAddrErrorEn());                                                 // Read ERROR_EN register
    adcDone.rdErrorEn = false;                                                  // ERROR_EN not yet read
}//end adcRdErrorEn()

/*** adcRdMclkCount ***************************************************************************************************************/
void adcRdMclkCount(void){
    adcRdReg(adcAddrMclkCnt());                                                 // Read MCLK_COUNT register
    adcDone.rdMclkCount = false;                                                // MCLK_COUNT not yet read
}//end adcRdMclkCount()

/*** adcRdCh **********************************************************************************************************************/
bool adcRdCh(uint8_t ch){
    if(adcMaxChIndex() < ch) return false;                                      // Index out of range
    adcRdReg(adcAddrCh0() + ch);                                                // Read CHn register
    adcDone.rdCh = false;                                                       // CHn not yet read
    return true;
}//end adcRdCh()

/*** adcRdConfig ******************************************************************************************************************/
bool adcRdConfig(uint8_t configNum){
    if(adcMaxCfgIndex() < configNum) return false;                              // Index out of range
    adcRdReg(adcAddrConfig0()+configNum);                                       // Read CONFIG_n register
    adcDone.rdConfig = false;                                                   // CONFIG_n not yet read
    return true;
}//end adcRdConfig()

/*** adcRdFilter ******************************************************************************************************************/
bool adcRdFilter(uint8_t filterNum){
    if(adcMaxFilterIndex() < filterNum) return false;                           // Index out of range
    adcRdReg(adcAddrFilter0()+filterNum);                                       // Read FILTER_n register
    adcDone.rdFilter = false;                                                   // FILTER_n not yet read
    return true;
}//end adcRdFilter()

/*** adcRdOffset ******************************************************************************************************************/
bool adcRdOffset(uint8_t offsetNum){
    if(adcMaxOffsetIndex() < offsetNum) return false;                           // Index out of range
    adcRdReg(adcAddrOffset0()+offsetNum);                                       // Read OFFSET_n register
    adcDone.rdOffset = false;                                                   // OFFSET_n not yet read
    return true;
}//end adcRdOffset()

/*** adcRdGain ********************************************************************************************************************/
bool adcRdGain(uint8_t gainNum){
    if(adcMaxGainIndex() < gainNum) return false;                               // Index out of range
    adcRdReg(adcAddrGain0()+gainNum);                                           // Read GAIN_n register
    adcDone.rdGain = false;                                                     // GAIN_n not yet read
    return true;
}//end adcRdGain()

/*** adcGetActiveCh ***************************************************************************************************************/
uint8_t adcGetActiveCh(void){return (adcReg.status & 0x0F);}                    // Return the active channel number

/*** adcGetErrorFlag **************************************************************************************************************/ 
bool adcGetErrorFlag(void){
    if((adcReg.status & 0x40) > 0)return true;                                  // Error present
    return false;                                                               // No errors
}//end adcGetErrorFlag

/*** adcContReadEn ****************************************************************************************************************/ 
void adcContReadEn(void){
    uint16_t temp = adcReg.adcControl;                                          // Temporary copy of ADC_CONTROL
    temp |= 0x0800;                                                             // Set bit 11 in ADC_CONTROL
    adcWrReg(adcAddrControl(), temp);                                           // Write TX soft buffer
    adcDone.wrAdcControl = false;                                               // ADC_CONTROL not yet written
}//end adcContReadEn()

/*** adcCalIOffset ****************************************************************************************************************/ 
void adcCalIOffset(void){
    uint16_t temp = adcReg.adcControl;                                          // Temporary copy of ADC_CONTROL
    temp &= 0xFFC3;                                                             // Clear mode bits
    temp |= 0x0014;                                                             // Mode bits = '0101'
    adcWrReg(adcAddrControl(), temp);                                           // Write TX soft buffer
    adcDone.wrAdcControl = false;                                               // ADC_CONTROL not yet written
}//end adcCalIOffset()

/*** adcCalIFs ********************************************************************************************************************/ 
void adcCalIFs(void){
    uint16_t temp0 = adcReg.adcControl;                                         // Temporary copy of ADC_CONTROL
    uint32_t temp1 = adcReg.offset0;                                            // Temporary copy of OFFSET_0
    temp1   = adcDefaultOffset();                                               // Reset offset shadow register
    temp0  &= 0xFF03;                                                           // Clear power and mode bits
    temp0  |= 0x0014;                                                           // Mode bits = '0110'
    adcWrReg(adcAddrOffset0(), temp1);                                          // Reset ADC offset 0
    adcWrReg(adcAddrControl(), temp0);                                          // Start internal full scale calibration
    adcDone.wrOffset     = false;                                               // Offset not yet written
    adcDone.wrAdcControl = false;                                               // ADC_CONTROL not yet written
}//end adcCalIFs()

/*** adcSetLowPwrMode *************************************************************************************************************/ 
void adcSetLowPwrMode(void){
    uint16_t temp = adcReg.adcControl;                                          // Temporary copy of ADC_CONTROL
    temp &= 0xFF3F;                                                             // Clear power bits
    adcWrReg(adcAddrControl(), temp);                                           // Set ADC to low power mode
    adcDone.wrAdcControl = false;                                               // ADC_CONTROL not yet written
}//end adcSetLowPwrMode()

/*** adcSetMidPwrMode *************************************************************************************************************/ 
void adcSetMidPwrMode(void){
    uint16_t temp = adcReg.adcControl;                                          // Temporary copy of ADC_CONTROL
    temp  &= 0xFF3F;                                                            // Clear power bits
    temp  |= 0x0040;                                                            // Power bits = '01'
    adcWrReg(adcAddrControl(), temp);                                           // Set ADC to mid power mode
    adcDone.wrAdcControl = false;                                               // ADC_CONTROL not yet written
}//end adcSetMidPwrMode()

/*** adcSetFullPwrMode ************************************************************************************************************/ 
void adcSetFullPwrMode(void){
    uint16_t temp = adcReg.adcControl;                                          // Temporary copy of ADC_CONTROL
    temp  &= 0xFF3F;                                                            // Clear power bits
    temp  |= 0x0080;                                                            // Power bits = '10'
    adcWrReg(adcAddrControl(), temp);                                           // Set ADC to full power mode
    adcDone.wrAdcControl = false;                                               // ADC_CONTROL not yet written
}//end adcSetFullPwrMode()

/*** adcSetSRate10 ****************************************************************************************************************/ 
void adcSetSRate10(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x100780); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 10 SPS
        default :                                              break;           // Default--exit switch
    }//end switch
}//end adcSetSRate10()

/*** adcSetSRate60 ****************************************************************************************************************/ 
void adcSetSRate60(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x100140); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 60 SPS
        default :                                              break;           // Default--exit switch
    }//end switch
}//end adcSetSRate60()

/*** adcSetSRate100 ***************************************************************************************************************/ 
void adcSetSRate100(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x1000C0); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 100 SPS
        default :                                              break;           // Default--exit switch
    }//end switch    
}//end adcSetSRate100()

/*** adcSetSRate1000 **************************************************************************************************************/ 
void adcSetSRate1000(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x100013); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 1000 SPS
        default :                                              break;           // Default--exit switch
    }//end switch    
}//end adcSetSRate1000()

/*** adcSetSRate4800 **************************************************************************************************************/ 
void adcSetSRate4800(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x100004); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 4800 SPS
        default :                                              break;           // Default--exit switch
    }//end switch    
}//end adcSetSRate4800()

/*** adcSetSRate9600 **************************************************************************************************************/ 
void adcSetSRate9600(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x100002); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 9600 SPS
        default :                                              break;           // Default--exit switch
    }//end switch    
}//end adcSetSRate9600()

/*** adcSetSRate19200 *************************************************************************************************************/ 
void adcSetSRate19200(uint8_t ch){
    switch(ch){
        case adcCh0(): adcWrReg(adcAddrFilter0(),   0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh1(): adcWrReg(adcAddrFilter0()+1, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh2(): adcWrReg(adcAddrFilter0()+2, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh3(): adcWrReg(adcAddrFilter0()+3, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh4(): adcWrReg(adcAddrFilter0()+4, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh5(): adcWrReg(adcAddrFilter0()+5, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh6(): adcWrReg(adcAddrFilter0()+6, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        case adcCh7(): adcWrReg(adcAddrFilter0()+7, 0x100001); break;           // SYNC4; 60Hz REJECT; SINGLE CYCLE OFF; 19200 SPS
        default :                                              break;           // Default--exit switch
    }//end switch    
}//end adcSetSRate19200()

/*** adcSetSRate ******************************************************************************************************************/
void adcSetSRate(uint8_t sRate, uint8_t chToSet){
    uint8_t ch = chToSet;
    switch(sRate){
        case adcRate10Req()   : adcSetSRate10(ch);    break;                    // Set rate to 10 SPS
        case adcRate60Req()   : adcSetSRate60(ch);    break;                    // Set rate to 60 SPS
        case adcRate100Req()  : adcSetSRate100(ch);   break;                    // Set rate to 100 SPS
        case adcRate1kReq()   : adcSetSRate1000(ch);  break;                    // Set rate to 1000 SPS
        case adcRate4k8Req()  : adcSetSRate4800(ch);  break;                    // Set rate to 4800 SPS
        case adcRate9k6Req()  : adcSetSRate9600(ch);  break;                    // Set rate to 9600 SPS
        case adcRate19k2Req() : adcSetSRate19200(ch); break;                    // Set rate to 19200 SPS
        default :                                     break;                    // Default--Exit switch
    }//end switch
}//end adcSetSRate()

/*** adcChOn **********************************************************************************************************************/
void adcChOn(uint8_t ch){
    uint16_t temp;                                                              // Temporary variable to store register contents
    switch(ch){
        case 0: temp = adcReg.ch0;                                              // Temporary copy of CH0
        case 1: temp = adcReg.ch1;                                              // Temporary copy of CH1
        case 2: temp = adcReg.ch2;                                              // Temporary copy of CH2
        case 3: temp = adcReg.ch3;                                              // Temporary copy of CH3
        case 4: temp = adcReg.ch4;                                              // Temporary copy of CH4
        case 5: temp = adcReg.ch5;                                              // Temporary copy of CH5
        case 6: temp = adcReg.ch6;                                              // Temporary copy of CH6
        case 7: temp = adcReg.ch7;                                              // Temporary copy of CH7
    }//end switch
    temp |= 0x8000;                                                             // Set bit enable
    adcWrReg(adcAddrCh0() + ch, temp);                                          // Enable channel
    adcDone.wrCh = false;                                                       // CH not yet written
}//end adcChOn()

/*** adcChOff *********************************************************************************************************************/
void adcChOff(uint8_t ch){
    uint16_t temp;                                                              // Temporary variable to store register contents
    switch(ch){
        case 0: temp = adcReg.ch0;                                              // Temporary copy of CH0
        case 1: temp = adcReg.ch1;                                              // Temporary copy of CH1
        case 2: temp = adcReg.ch2;                                              // Temporary copy of CH2
        case 3: temp = adcReg.ch3;                                              // Temporary copy of CH3
        case 4: temp = adcReg.ch4;                                              // Temporary copy of CH4
        case 5: temp = adcReg.ch5;                                              // Temporary copy of CH5
        case 6: temp = adcReg.ch6;                                              // Temporary copy of CH6
        case 7: temp = adcReg.ch7;                                              // Temporary copy of CH7
    }//end switch
    temp |= 0x7FFF;                                                             // Clear bit enable
    adcWrReg(adcAddrCh0() + ch, temp);                                          // Disable channel
    adcDone.wrCh = false;                                                       // CH not yet written
}//end adcChOff()

/*** adcShuntOn *******************************************************************************************************************/
void adcShuntOn(void){
    uint32_t temp = adcReg.ioControl1;                                          // Temporary copy of IO_CONTROL_1
    temp |= 0x00008000;                                                         // Set bit PDSW
    adcWrReg(adcAddrIoCtrl1(), temp);                                           // Enable shunt switch
    adcDone.wrIoControl1 = false;                                               // IO_CONTROL_1 not yet written
}//end adcShuntOn()

/*** adcShuntOff ******************************************************************************************************************/
void adcShuntOff(void){
    uint32_t temp = adcReg.ioControl1;                                          // Temporary copy of IO_CONTROL_1
    temp &= 0xFFFF7FFF;                                                         // Clear bit PDSW
    adcWrReg(adcAddrIoCtrl1(), temp);                                           // Disable shunt switch
    adcDone.wrIoControl1 = false;                                               // IO_CONTROL_1 not yet written
}//end adcShuntOff()

/*** adcWrAdcCtrlDone *************************************************************************************************************/
bool adcWrAdcCtrlDone(void){return adcDone.wrAdcControl;}

/*** adcWrChDone ******************************************************************************************************************/
bool adcWrChDone(void){return adcDone.wrCh;}

/*** adcWrDoneConfig **************************************************************************************************************/
bool adcWrDoneConfig(void){return adcDone.wrConfig;}

/*** adcWrDoneErrorEn *************************************************************************************************************/
bool adcWrDoneErrorEn(void){return adcDone.wrErrorEn;}

/*** adcWrFilterDone **************************************************************************************************************/ 
bool adcWrFilterDone(void){return adcDone.wrFilter;}

/*** adcWrGainDone ****************************************************************************************************************/
bool adcWrGainDone(void){return adcDone.wrGain;}

/*** adcWrIoCtrl1Done *************************************************************************************************************/ 
bool adcWrIoCtrl1Done(void){return adcDone.wrIoControl1;}

/*** adcWrIoCtrl2Done *************************************************************************************************************/ 
bool adcWrIoCtrl2Done(void){return adcDone.wrIoControl2;}

/*** adcWrOffsetDone **************************************************************************************************************/
bool adcWrOffsetDone(void){return adcDone.wrOffset;}

/*** adcService *******************************************************************************************************************/
void adcService(void){
    if(((uint8_t)(adc.txEnd - adc.txStart)) > 3){                               // Check for instructions to parse
        if(adc.tx[adc.txStart] == adcCmdWrite()){                               // Register write instructions
            uint8_t bytes;                                                      // Local byte counter
            adc.txStart++;                                                      // Next element should be command and address
            adc.writing = adc.tx[adc.txStart++];                                // Log command and address
            adcSpiBuf() = adc.writing;                                          // Command and address -> SPI
            bytes       = adc.tx[adc.txStart++];                                // Number of bytes to write
            while(bytes > 0){
                bytes--;
                adcSpiBuf() = adc.tx[adc.txStart++];}                           // Data -> SPI                   
            while(adcSpiBusy());                                                // SPI transaction not done; wait
            adcSpiCloseFrame();                                                 // Strobe chip select
            adcSpiOpenFrame();                                                  
            adcRxFifoClear();                                                   // Clear SPI RX FIFO
            adcFlagSet();                                                       // Set appropriate flags to indicate end of command
        }//end if(...adcCmdWrite())
        else if(adc.tx[adc.txStart] == adcCmdRead()){                           // Register read instructions
            uint8_t bytes;                                                      // Local byte counter
            adc.txStart++;                                                      // Next element should be address
            adc.reading = adc.tx[adc.txStart++];                                // Log address
            adcSpiBuf() = adc.reading | adcCmdRead();                           // Command and address -> SPI
            adc.bToRead = adc.tx[adc.txStart++];                                // Log the number of bytes to read
            for(bytes = adc.bToRead; bytes > 0; bytes--){
                adcSpiBuf() = adcSpiNop();}                                     // NOP clocks -> SPI
            while(adcSpiBusy());                                                // SPI transaction not done; wait
            adcSpiCloseFrame();                                                 // Strobe chip select
            adcSpiOpenFrame();
            adcSpiToMirrorReg();                                                // SPI RX FIFO -> corresponding mirror / buffer
        }//end else if(...adcCmdRead())
    }//end if(adc.txStart...)
    else{                                                                       // No instructions; read ADC data
        adcDone.rdData  = false;                                                // ADC data not yet read
        adcSpiBuf()     = adcCmdRead()|adcAddrData();                           // Send data read command
        if(adc.statusEn) adcSpiBuf() = adcSpiNop();                             // Clock out status byte if enabled
        adcSpiBuf()     = adcSpiNop();                                          // Clock out byte 2
        adcSpiBuf()     = adcSpiNop();                                          // Clock out byte 1
        adcSpiBuf()     = adcSpiNop();                                          // Clock out byte 0
        adc.reading     = adcAddrData();                                        // Log address
        adc.bToRead     = 0;                                                    // Clear the number of bytes to read
        while(adcSpiBusy());                                                    // SPI transaction not done; wait
        adcSpiCloseFrame();                                                     // Strobe chip select
        adcSpiOpenFrame();
        adcSpiToMirrorReg();                                                    // SPI RX FIFO -> corresponding mirror / buffer
    }//end else if
}//end adcService()

/*** adcCopyRxBuf *****************************************************************************************************************/
void adcCopyRxBuf(uint8_t *buf, uint8_t *end, uint8_t chNum){
    uint8_t temp = *end;
    while((uint8_t)(adc.ch[chNum].rxEnd - adc.ch[chNum].rxStart) > 0){          // Copy the ADC buffer
        buf[temp++] = adc.ch[chNum].rx[adc.ch[chNum].rxStart++];}
    *end = temp;                                                                // Copy ending index
}//end adcCopyRxBuf()

/*** adcCopy24To16Bit *************************************************************************************************************/
void adcCopy24To16Bit(uint8_t *buf, uint8_t *end, uint8_t chNum){
    uint8_t temp = *end;                                                        // Copy caller buffer end
    uint8_t localEnd    = adc.ch[chNum].rxEnd;                                  // Local copy of the driver RX buffer end index
    while((uint8_t)(localEnd - adc.ch[chNum].rxStart) > 2){                     // Copy the ADC buffer
        buf[temp++] = adc.ch[chNum].rx[adc.ch[chNum].rxStart++];                // Copy byte 2
        buf[temp++] = adc.ch[chNum].rx[adc.ch[chNum].rxStart++];                // Copy byte 1
        adc.ch[chNum].rxStart++;}                                               // Waste byte 0
    *end = temp;                                                                // Copy ending index
}//end adcCopy24To16Bit()

/*** adcChNewData **************************************************************************************************************/
bool adcChNewData(uint8_t chNum){
    if((uint8_t)(adc.ch[chNum].rxEnd - adc.ch[chNum].rxStart) > 2) return true; // New data available--return true
    return false;                                                               // No new data-return false
}//end adcChNewData()

/*** adcGetChData *****************************************************************************************************************/
bool adcGetChData(uint8_t chNum, uint32_t *var){
    if(adcChNewData(chNum)){                                                    // Check if new data is available
        *var  = adc.ch[chNum].rx[adc.ch[chNum].rxStart++];    *var <<= 8;       // Byte 2
        *var |= adc.ch[chNum].rx[adc.ch[chNum].rxStart++];    *var <<= 8;       // Byte 1
        *var |= adc.ch[chNum].rx[adc.ch[chNum].rxStart++];                      // Byte 0
        return true;}                                                           // Data collected successfully--return true
    return false;                                                               // No new data--return false
}//end adcGetChData()