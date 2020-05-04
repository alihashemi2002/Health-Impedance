#include "digitizer.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
       digi_obj    digi                      = {.active = digiMinCh()};         // Digitizer object
       digiSrc_obj digiSens[digiNumCh()];                                       // Digitizer channel objects
extern adc_obj     adc;                                                         // ADC object

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** digiInitialize ***************************************************************************************************************/
void digiInitialize(void){
    adcInitialize();                                                            // Initialize ADC
    ddsInitialize();                                                            // Initialize DDS    
    muxInitialize();                                                            // Initialize mux
    sw1Initialize();                                                            // Initialize switch 1
    sw2Initialize();                                                            // Initialize switch 2
    sw3Initialize();                                                            // Initialize switch 3
    sw4Initialize();                                                            // Initialize switch 4    
}//end digiInitialize()

/*** digiAdChSet ******************************************************************************************************************/
void digiAdChSet(uint8_t ch){digi.active.adObjNum = ch;}                        // Set active object                     

/*** digiAdChSel ******************************************************************************************************************/
uint8_t digiAdChSel(void){
    uint8_t temp;
    if(digi.active.adObjNum < digiMaxCh())temp = digi.active.adObjNum + 1;      // Get next channel
    else temp = digiMinCh();
    while(temp != digi.active.adObjNum){                                        // Select next enabled channel
        if(digiSens[temp].smplEn) return temp;                                  // Is channel assigned to AD1?; Return the new selected channel
        if(temp < digiMaxCh())  temp++;                                         // Advance to next channel
        else temp = digiMinCh();                                                // Roll over to lowest channel
    }//end while
    if((digiSens[temp].smplEn)) return temp;                                    // Channel remains the same
    return digiMaxCh()+1;                                                       // No channel selected; return value out of range
}//end digiAdChSel()

/*** digiChMuxSel *****************************************************************************************************************/
void digiChMuxSel(void){
    uint8_t ch;                                                                 // Current channel
    ch = digi.active.adObjNum;                                                  // Get currently selected channel    
    muxSetInp(digiSens[ch].muxCh);                                              // Set mux input

    switch(digiSens[ch].swNum){
        case digiSw1()  : sw1Sel(digiSens[ch].swCh); sw1Service(); break;       // Channel uses switch 0, set switch, service, exit
        case digiSw2()  : sw2Sel(digiSens[ch].swCh); sw2Service(); break;       // Channel uses switch 1, set switch, service, exit
        case digiSw3()  : sw3Sel(digiSens[ch].swCh); sw3Service(); break;       // Channel uses switch 2, set switch, service, exit
        case digiSw4()  : sw4Sel(digiSens[ch].swCh); sw4Service(); break;       // Channel uses switch 3, set switch, service, exit
        default :                                          break;}              // Invalid switch, exit
}//end digiChMuxSel()

/*** digiAdcNewData ***************************************************************************************************************/
bool digiAdcNewData(void){
    if(((uint8_t)(adc.ch[0].rxEnd - adc.ch[0].rxStart)) > digiAdDwSize()){
        return true;}
    return false;
}//end digiAdcNewData()

/*** digiGetAdcData ***************************************************************************************************************/
bool digiGetAdcData(void){
    int32_t temp = 0;                                                           // ADC value place holder
    uint8_t ch;                                                                 // Current channel
    if(!digiAdcNewData())return false;                                          // Data not ready--exit
    ch = digi.active.adObjNum;                                                  // Get currently selected channel        
    temp |= adc.ch[0].rx[adc.ch[0].rxStart++];    temp <<= 8;                   // Byte 2
    temp |= adc.ch[0].rx[adc.ch[0].rxStart++];    temp <<= 8;                   // Byte 1
    temp |= adc.ch[0].rx[adc.ch[0].rxStart++];                                  // Byte 0
    digiSens[ch].data = temp;                                                   // Data stored in object data member
    digiSens[ch].newData = true;                                                // New data acquired   
    return true;                                                                // Success
}//end digiGetAdcData()

/*** digiService ******************************************************************************************************************/
void digiService(void){

    adcService();                                                               // ADC service execution
    if(!adc.initDone)   return false;                                           // Initialization in progress

    // --- DIGITIZER AD STATE MACHINE ----------------------------------------------------------------------------------------------
    switch(digi.adState){
        case digiSmGetData():
            if(digiGetAdcData()){                                               // Success getting ADC data
                uint8_t temp = digiAdChSel();                                   // Get new channel identifier
                if(temp > digiMaxCh()) digi.adState = digiDeflt();              // Channel error--reset needed
                else{digiAdChSet(temp); digiChMuxSel();}                        // Record next channel, Set SW and MUX
            }//end if
            break;
        default : break;                                                        // Reset state--exit
    }//end switch
}//end digiService()