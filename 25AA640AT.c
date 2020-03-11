#include "25AA640AT.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
prom_obj prom;                                                                  // PROM variables

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** promSpiInit ******************************************************************************************************************/
void promSpiInit(void){
    promSpiCon()    = 0;                                                        // Clear register                                                            
    promSpiCon2()   = 0;                                                        // Clear register     
    promSpiCon()    = 0x18120;                                                  // ON; ENH BUF; 8-BIT; SMP=0; CKE=1; CKP=0; MASTER;
    promSpiBrg()    = 0x0003;                                                   // PB2CLK / 8 (6.25MHz)
}//end promSpiInit()

/*** promSpiRxFifoClear ***********************************************************************************************************/
void promSpiRxFifoClear(void){
    uint8_t var;
    while(!promSpiRxBufEmpty()) var = promSpiBuf();                             // READ PROM SPI RX FIFO
    prom.bufLoad = 0;                                                           // Clear number of bytes in FIFO
}//end promSpiRxFifoClear()

/*** promLockBlock1 ***************************************************************************************************************/
void promLockBlock1(void){
    prom.tx[prom.txEnd++] = promWrsrInstr();                                    // WRSR instruction
    prom.tx[prom.txEnd++] = 0x84;                                               // Configuration word
}//end promLockBlock1()

/*** promLockBlock2 ***************************************************************************************************************/
void promLockBlock2(void){
    prom.tx[prom.txEnd++] = promWrsrInstr();                                    // WRSR instruction
    prom.tx[prom.txEnd++] = 0x88;                                               // Configuration word
}//end promLockBlock2()

/*** promLockBlock3 ***************************************************************************************************************/
void promLockBlock3(void){
    prom.tx[prom.txEnd++] = promWrsrInstr();                                    // WRSR instruction
    prom.tx[prom.txEnd++] = 0x8C;                                               // Configuration word
}//end promLockBlock3()

/*** promUnlock *******************************************************************************************************************/
void promUnlock(void){
    prom.tx[prom.txEnd++] = promWrsrInstr();                                    // WRSR instruction
    prom.tx[prom.txEnd++] = 0x80;                                               // Configuration word
}//end promUnlock()

/*** promWriteData ****************************************************************************************************************/
void promWriteData(uint16_t promAddr, uint16_t numOfBytes){
    prom.tx[prom.txEnd++] = promWriteInstr();                                   // Write instruction
    prom.tx[prom.txEnd++] = promAddr >> 8;                                      // Start address MSB
    prom.tx[prom.txEnd++] = promAddr;                                           // Start address LSB
    prom.tx[prom.txEnd++] = numOfBytes >> 8;                                    // Bytes to write MSB
    prom.tx[prom.txEnd++] = numOfBytes;                                         // Bytes to write LSB
}//end promWriteData()

/*** promWriteUint8 ***************************************************************************************************************/
void promWriteUint8(uint8_t val, uint16_t address){
   promWriteData(address, 1);                                                   // Instruction, address, number of bytes to buffer                                                  
   prom.tx[prom.txEnd++] = val;                                                 // Val to buffer
}//promWriteUint8()

/*** promWriteUint16 **************************************************************************************************************/
void promWriteUint16(uint16_t val, uint16_t address){
   promWriteData(address, 2);                                                   // Instruction, address, number of bytes to buffer 
   prom.tx[prom.txEnd++] = val >> 8;                                            // Val MSB to buffer
   prom.tx[prom.txEnd++] = val;                                                 // Val LSB to buffer
}//promWriteUint16()

/*** promWriteUint32 **************************************************************************************************************/
void promWriteUint32(uint32_t val, uint16_t address){
   promWriteData(address, 4);                                                   // Instruction, address, number of bytes to buffer 
   prom.tx[prom.txEnd++] = val >> 24;                                           // Val MSB to buffer
   prom.tx[prom.txEnd++] = val >> 16;                                           // Val byte 2 to buffer
   prom.tx[prom.txEnd++] = val >> 8;                                            // Val byte 1 to buffer
   prom.tx[prom.txEnd++] = val;                                                 // Val LSB to buffer
}//promWriteUint32()

/*** promWriteInt32 ***************************************************************************************************************/
void promWriteInt32(int32_t val, uint16_t address){
   promWriteData(address, 4);                                                   // Instruction, address, number of bytes to buffer 
   prom.tx[prom.txEnd++] = val >> 24;                                           // Val MSB to buffer
   prom.tx[prom.txEnd++] = val >> 16;                                           // Val byte 2 to buffer
   prom.tx[prom.txEnd++] = val >> 8;                                            // Val byte 1 to buffer
   prom.tx[prom.txEnd++] = val;                                                 // Val LSB to buffer
}//promWriteInt32()

/*** promWriteArray ***************************************************************************************************************/
void promWriteArray(uint8_t *arr, uint8_t size, uint16_t address){
    uint8_t tempCount;                                                          // Temporary loop counter
    promWriteData(address, size);                                               // Instruction, address, number of bytes to buffer                                               
    for (tempCount = 0; tempCount < size; tempCount++){                         // Write array
       prom.tx[prom.txEnd++] = arr[tempCount]; 
    }//end for
}//end promWriteArray()

/*** promWriteFloat ***************************************************************************************************************/
void promWriteFloat(float val, uint16_t address){
    uint8_t floatBytes[promFltNumSize()];                                       // Temporary array to hold byte values
    memcpy(floatBytes, &val, promFltNumSize());                                 // Float to array
    promWriteArray(floatBytes, promFltNumSize(), address);                      // Float bytes to PROM                                             
}//end promWriteFloat()

/*** promReadData *****************************************************************************************************************/
void promReadData(uint16_t promAddr, uint16_t numOfBytes){
    prom.tx[prom.txEnd++] = promReadInstr();                                    // Read instruction
    prom.tx[prom.txEnd++] = promAddr >> 8;                                      // Start address MSB
    prom.tx[prom.txEnd++] = promAddr;                                           // Start address LSB
    prom.tx[prom.txEnd++] = numOfBytes >> 8;                                    // Bytes to write MSB
    prom.tx[prom.txEnd++] = numOfBytes;                                         // Bytes to write LSB
}//end promReadData()

/*** promReadUint8 ***************************************************************************************************************/
void promReadUint8(uint16_t address){promReadData(address, 1);}                 // Instruction, address, number of bytes to buffer

/*** promReadUint16 **************************************************************************************************************/
void promReadUint16(uint16_t address){promReadData(address, 2);}                // Instruction, address, number of bytes to buffer

/*** promReadUint32 **************************************************************************************************************/
void promReadUint32(uint16_t address){promReadData(address, 4);}                // Instruction, address, number of bytes to buffer

/*** promReadInt32 ***************************************************************************************************************/
void promReadInt32(uint16_t address){promReadData(address, 4);}                 // Instruction, address, number of bytes to buffer

/*** promReadFloat ***************************************************************************************************************/
void promReadFloat(uint16_t address){promReadData(address, 4);}                 // Instruction, address, number of bytes to buffer

/*** promReadBuf ******************************************************************************************************************/ 
void promReadBuf(uint8_t *buf, uint8_t bufStart, uint8_t *bufEnd){
    uint8_t end   = *bufEnd;                                                    // Local end index copy of passed global buffer
    while((uint8_t)(prom.rxEnd - prom.rxStart) > 0){                            // Copy available data
        buf[end++] = prom.rx[prom.rxStart++];
        if(end == bufStart) break;}                                             // Prevent global buffer overflow
    *bufEnd = end;                                                              // Updated global buffer end index
}//end promReadBuf()

/*** promBytesInRx ****************************************************************************************************************/ 
uint8_t promBytesInRx(void){
    return ((uint8_t)(prom.rxEnd - prom.rxStart));                              // Return number of bytes in RX                    
}//end promBytesInRx()

/*** promInitialize ****************************************************************************************************************/ 
void promInitialize(void){
    prom.txStart = 0;                                                           // Initialize PROM TX buffer start index
    prom.txEnd = 0;                                                             // Initialize PROM TX buffer end index
    prom.rxStart = 0;                                                           // Initialize PROM RX buffer start index
    prom.rxEnd = 0;                                                             // Initialize PROM RX buffer end index
    prom.state = promInitialState();                                            // Initialize PROM state machine index
    prom.statePrev = 0;                                                         // Initialize PROM state machine previous index
    prom.addr = 0;                                                              // Initialize PROM address
    prom.bytes = 0;                                                             // Initialize PROM number of bytes to read / write
    prom.delayCntr = 0;                                                         // Initialize PROM delay counter
    prom.bufLoad   = 0;                                                         // Initialize PROM FIFO byte counter
    promWpOn();                                                                 // PROM write protect on
    promSpiCloseFrame();                                                        // Close PROM SPI frame
    promSpiOpenFrame();                                                         // Toggle PROM SPI frame per datasheet
    promSpiCloseFrame();                                                        // Close PROM SPI frame
    promSpiInit();                                                              // Initialize SPI port for PROM use
}//end promInitialize()

/*** promService ******************************************************************************************************************/
void promService(void){
    switch(prom.state){
        case promInitialState():
            if((uint8_t)(prom.txEnd - prom.txStart) > 1){                       // Nothing to send
                if(prom.tx[prom.txStart] == promWrsrInstr()){                   // PROM WRITE STATUS REGISTER instruction
                    prom.statePrev = prom.state;                                // Log last state
                    prom.state = promWrsrState();                               // Write status register
                    break;}}                                                    // Exit
            if((uint8_t)(prom.txEnd - prom.txStart) > 4){
                if((prom.tx[prom.txStart] == promWriteInstr())||                // PROM WRITE REGISTER instruction
                   (prom.tx[prom.txStart] == promReadInstr())){                 // PROM READ REGISTER instruction
                    prom.statePrev = prom.state;                                // Log last state
                    prom.state = promParseState();                              // Next -> parse buffer state
                    break;}}                                                    // Exit
            break;                                                              // Exit
        case promWrsrState():                                                   // Write status register state
            promWpOff();                                                        // Protection off
            promSpiOpenFrame();                                                 // Open SPI frame
            promSpiBuf() = prom.tx[prom.txStart++];                             // WRSR instruction
            promSpiBuf() = prom.tx[prom.txStart++];                             // Status
            prom.bufLoad = 2;                                                   // Bytes in the SPI TX FIFO
            prom.statePrev = prom.state;                                        // Log last state
            prom.state = promWaitState();                                       // Next -> wait state
            break;                                                              // Exit
        case promParseState():                                                  // Parse buffer state
            if(prom.tx[prom.txStart++] == promWriteInstr()){                    // PROM WRITE REGISTER instruction
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promWriteState0();}                                // Next -> write state 0
            else{
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promReadState0();}                                 // Next -> read state
            prom.addr    = prom.tx[prom.txStart++];                             // Address MSB
            prom.addr  <<= 8;
            prom.addr   |= prom.tx[prom.txStart++];                             // Address LSB
            prom.bytes   = prom.tx[prom.txStart++];                             // Bytes MSB
            prom.bytes <<= 8;
            prom.bytes  |= prom.tx[prom.txStart++];                             // Bytes LSB
            break;                                                              // Exit
        case promWriteState0():                                                 // Write state 0
            if(prom.bytes < 1){                                                 // Nothing to send
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promInitialState();                                // Next -> initial state
                break;}                                                         // Exit
            promSpiRxFifoClear();                                               // Clear RX FIFO
            promSpiOpenFrame();                                                 // Open SPI frame
            promSpiBuf() = promWrenInstr();                                     // PROM write enable instruction
            prom.bufLoad++;                                                     // Byte added to TX FIFO
            prom.statePrev = prom.state;                                        // Log last state
            prom.state = promWriteState1();                                     // Next -> write state 1
            break;                                                              // Exit
        case promWriteState1():                                                 // Write state 1
            if(promSpiBusy()) break;
            promSpiCloseFrame();                                                // Close SPI frame
            promSpiRxFifoClear();                                               // Clear RX FIFO
            promSpiOpenFrame();                                                 // Open SPI frame
            promSpiBuf() = promWriteInstr();                                    // PROM write instruction
            promSpiBuf() = prom.addr >> 8;                                      // Address MSB
            promSpiBuf() = prom.addr;                                           // Address MSB
            prom.bufLoad = 3;                                                   // Bytes added to TX FIFO
            while(prom.bytes > 0){
                if(promSpiTxBufFull()||(prom.bufLoad > promSpiEnhBufSize())){   // TX FIFO is full
                    prom.statePrev = prom.state;                                // Log last state
                    prom.state = promWriteState2();                             // FIFO is full...
                    break;}                                                     // Exit
                promSpiBuf() = prom.tx[prom.txStart++];                         // Write data
                prom.bufLoad++;                                                 // Byte added to TX FIFO
                prom.bytes--;                                                   // 1 byte written
                prom.addr++;                                                    // Increase address
                if(prom.addr % (promPage()) == 0){                              // If write cycle needs to advance to next page
                    prom.statePrev = prom.state;                                // Log last state
                    prom.state = promWriteState3();                             // End of PROM page
                    break;}}                                                    // Exit
            if(prom.bytes < 1){                                                 // Available data to write?
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promWaitState();}                                  // Write done
            break;                                                              // Exit
        case promWriteState2():                                                 // TX FIFO full state
            promSpiRxFifoClear();                                               // Clear RX FIFO
            while(prom.bytes > 0){
                if(promSpiTxBufFull()||(prom.bufLoad > promSpiEnhBufSize())){   // SPI FIFO is full
                    break;}                                                     // Exit
                promSpiBuf() = prom.tx[prom.txStart++];                         // Write data
                prom.bufLoad++;                                                 // Byte added to TX FIFO
                prom.bytes--;                                                   // 1 byte written
                prom.addr++;                                                    // Increase address
                if(prom.addr % (promPage()) == 0){
                    prom.statePrev = prom.state;                                // Log last state
                    prom.state = promWriteState3();                             // End of PROM page
                    break;}}                                                    // Exit
            if(prom.bytes == 0) {
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promWaitState();}                                  // Write done
            break;                                                              // Exit
        case promWriteState3():                                                 // End of page state
            promSpiCloseFrame();                                                // Close SPI frame
            promSpiRxFifoClear();                                               // Clear RX FIFO
            if(prom.delayCntr > promDelay()){                                   // PROM write time
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promWriteState0();                                 // Back to write state 0
                prom.delayCntr = 0;                                             // Clear counter
                break;}                                                         // Exit
            prom.delayCntr++;                                                   // PROM delay counter
            break;                                                              // Exit
        case promWaitState():                                                   // End of write state (wait)
            if(promSpiBusy()) break;                                            // SPI is busy - exit
            promSpiCloseFrame();                                                // Close SPI frame
            promWpOn();                                                         // Write protection on
            promSpiRxFifoClear();                                               // Clear RX FIFO
            if(prom.delayCntr > promDelay()){                                      
                prom.delayCntr = 0;                                             // Clear counter
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promInitialState();                                // Next -> initial state
                break;}                                                         // Exit
            prom.delayCntr++;                                                   // PROM delay counter increment
            break;                                                              // Exit
        case promReadState0():                                                  // Read state 0
            promSpiOpenFrame();                                                 // Open SPI frame
            promSpiBuf() = promReadInstr();                                     // PROM read instruction
            promSpiBuf() = prom.addr >> 8;                                      // Address MSB
            promSpiBuf() = prom.addr;                                           // Address MSB
            prom.bufLoad = 3;                                                   // Bytes written to TX FIFO
            prom.statePrev = prom.state;                                        // Log last state
            prom.state = promReadState2();                                      // Next -> read state 2
            break;                                                              // Exit
        case promReadState1():                                                  // Read state 1
            promSpiRxFifoClear();                                               // Clear RX FIFO
            while(prom.bytes > 0){
                if(promSpiTxBufFull()||(prom.bufLoad > promSpiEnhBufSize())){   // SPI FIFO is full
                    break;}                                                     // Exit
                promSpiBuf() = promNop();                                       // Write NOP to SPI buffer
                prom.bufLoad++;                                                 // Byte added to TX FIFO
                prom.bytes--;}                                                  // 1 NOP written
            prom.statePrev = prom.state;                                        // Log last state
            prom.state = promReadState2();                                      // Next -> read state 2
            break;                                                              // Exit
        case promReadState2():                                                  // Read state 2
            if(promSpiBusy()) break;                                            // SPI port busy
            if(prom.statePrev == promReadState0()) promSpiRxFifoClear();        // Clear RX FIFO
            else{
                while(!promSpiRxBufEmpty())prom.rx[prom.rxEnd++] = promSpiBuf();// RX FIFO -> RX soft buffer
                prom.bufLoad = 0;}                                              // SPI FIFO is empty
            if(prom.bytes > 0){                                                 // There is still data to read
                prom.statePrev = prom.state;                                    // Log last state
                prom.state = promReadState1();                                  // Next -> read state 1
                break;}                                                         // Exit
            promSpiCloseFrame();                                                // Close SPI frame
            prom.statePrev = prom.state;                                        // Log last state
            prom.state = promInitialState();                                    // Next -> initial state
            break;                                                              // Exit
        default:                                                                // Default state
            promSpiCloseFrame();                                                // Close SPI frame
            prom.txEnd    = prom.txStart = 0;                                   // Reinitialize TX buffer indexes
            prom.statePrev = prom.state;                                        // Reinitialize state machine state and log
            prom.state = promInitialState();                                    // Next -> initial state
            prom.bufLoad = 0;                                                   // No bytes in the SPI FIFO
            break;                                                              // Exit
    }//end switch
}//end promService()
