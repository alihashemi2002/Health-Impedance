#include "uart2.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
uart2_obj uart2;

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/

/*** uart2Initialize **************************************************************************************************************/
void uart2Initialize(void){
    uart2RegMode()   = 0x0000;                                                  // UART disabled; standard speed mode
    uart2RegSta()    = 0x0000;                                                  // Reset status register
    uart2RegBaud()   = 0x0000;                                                  // Reset baud-rate register
    uart2En();                                                                  // Enable UART
    uart2TxEn();                                                                // Enable TX
    uart2RxEn();                                                                // Enable RX
    uart2BrgHigh();                                                             // BaudRate high speed mode
    uart2RegBaud()   = (((FPB()/115200)/4) - 1);                                // BaudRate = 115200
    
    uart2.txStart    = 0;                                                       // Initialize TX buffer start index
    uart2.txEnd      = 0;                                                       // Initialize TX buffer end index
    uart2.rxStart    = 0;                                                       // Initialize RX buffer start index
    uart2.rxEnd      = 0;                                                       // Initialize RX buffer end index
    
    uart2RxIntFlagClr();                                                        // Clear UART RX interrupt flag
    uart2RxIntEn();                                                             // Enable UART RX interrupt 
}//end uart2Initialize()

/*** uart2DmaRxInitialize *********************************************************************************************************/
void uart2DmaRxInitialize(void){
    uart2RegMode()   = 0x0000;                                                  // UART disabled; standard speed mode
    uart2RegSta()    = 0x0000;                                                  // Reset status register
    uart2RegBaud()   = 0x0000;                                                  // Reset baud-rate register
    uart2BrgHigh();                                                             // BaudRate high speed mode
    uart2RegBaud()   = (((FPB()/115200)/4) - 1);                                // BaudRate = 115200
    uart2En();                                                                  // Enable UART
    uart2TxEn();                                                                // Enable TX
    uart2RxEn();                                                                // Enable RX
    
    uart2.txStart    = 0;                                                       // Initialize TX buffer start index
    uart2.txEnd      = 0;                                                       // Initialize TX buffer end index
    
    uart2RxIntFlagClr();                                                        // Clear UART RX interrupt flag
}//end uart2DmaRxInitialize()

/*** uart2DmaInitialize ***********************************************************************************************************/
void uart2DmaInitialize(void){
    uart2RegMode()   = 0x0000;                                                  // UART disabled; standard speed mode
    uart2RegSta()    = 0x00008000;                                              // TX IRQ when TX empty; RX IRQ when 1 byte received
    uart2RegBaud()   = 0x0000;                                                  // Reset baud-rate register
    uart2BrgHigh();                                                             // BaudRate high speed mode
    uart2RegBaud()   = (((FPB()/115200)/4) - 1);                                // BaudRate = 115200
    uart2En();                                                                  // Enable UART
    uart2RxEn();                                                                // Enable RX
    uart2RxIntFlagClr();                                                        // Clear UART RX interrupt flag
}//end uart2DmaRxInitialize()

/*** uart2NumBytesInRx ************************************************************************************************************/
uint8_t uart2NumBytesInRx(void){
    return ((uint8_t)(uart2.rxEnd - uart2.rxStart));                            // Return number of bytes in the RX soft buffer
}//end uart2NumBytesInRx()

/*** uart2NumBytesInTx ************************************************************************************************************/
uint8_t uart2NumBytesInTx(void){
    return ((uint8_t)(uart2.txEnd - uart2.txStart));                            // Return number of bytes in the TX soft buffer
}//end uart2NumBytesInTx()

/*** uart2WrIntVar ****************************************************************************************************************/
void uart2WrIntVal(uint64_t data, uint8_t bytes){
    while(bytes > 0) uart2.tx[uart2.txEnd++] = (data >> (8*(--bytes)));         // Write to TX soft buffer
}//end uart2WrIntVar()

/*** uart2WrUint8 *****************************************************************************************************************/
void uart2WrUint8(uint8_t data){uart2.tx[uart2.txEnd++] = data;}                // Write to TX soft buffer

/*** uart2WriteBuf ****************************************************************************************************************/ 
void uart2WriteBuf(uint8_t *buf, uint8_t *bufStart, uint8_t bufEnd){
    uint8_t start   = *bufStart;                                                // Local start index copy of the passed global buffer
    while((uint8_t)(bufEnd - start) > 0){                                       // Copy available data
        uart2.tx[uart2.txEnd++] = buf[start++];
        if(uart2.txEnd == uart2.txStart)break;                                  // Prevent TX soft buffer overflow
    }//end while
    *bufStart = start;                                                          // Update buffer start index
}//end uart2WriteBuf()

/*** uart2ReadBuf *****************************************************************************************************************/ 
void uart2ReadBuf(uint8_t *buf, uint8_t bufStart, uint8_t *bufEnd){
    uint8_t end   = *bufEnd;                                                    // Local end index copy of the passed global buffer
    while((uint8_t)(uart2.rxEnd - uart2.rxStart) > 0){                          // Copy available data
        buf[end++] = uart2.rx[uart2.rxStart++];
        if(end == bufStart)break;                                               // Prevent global buffer overflow
    }//end while
    *bufEnd = end;                                                              // Update global buffer end index
}//end uart2ReadBuf()

/*** uart2Service *****************************************************************************************************************/
uint8_t uart2Service(void){
    uint8_t status = 0;                                                         // Local status variable
    if(uart2Perr())      status  |= uart2ParityErr();                           // Parity error detected
    if(uart2Ferr())      status  |= uart2FrameErr();                            // Framing error detected
    if(uart2Overflow()){status   |= uart2Overrun(); uart2OverflowClr();}        // Overrun error detected; clear error
    
    while(uart2DataInRx() > 0)uart2.rx[uart2.rxEnd++] = uart2RegRx();           // Move RX data to RX soft buffer
    while(uart2TxbFull() < 1){
        if(uart2.txStart == uart2.txEnd) break;                                 // No more data to send
        uart2RegTx() = uart2.tx[uart2.txStart++];                               // Copy data to UART TX buffer
    }//end while
    return status;                                                              // Return status to caller
}//end uart2Service()

/*** uart2RxService ***************************************************************************************************************/
uint8_t uart2RxService(void){
    uint8_t status = 0;                                                         // Local status variable
    if(uart2Perr())     status  |= uart2ParityErr();                            // Parity error detected
    if(uart2Ferr())     status  |= uart2FrameErr();                             // Framing error detected
    if(uart2Overflow()){status  |= uart2Overrun(); uart2OverflowClr();}         // Overrun error detected; clear error
    
    while(uart2DataInRx() > 0) uart2.rx[uart2.rxEnd++] = uart2RegRx();          // Move RX data to RX soft buffer
    return status;                                                              // Return status to caller
}//end uart2RxService()

/*** uart2TxService ***************************************************************************************************************/
uint8_t uart2TxService(void){
    uint8_t status = 0;                                                         // Local status variable
    if(uart2Perr())     status  |= uart2ParityErr();                            // Parity error detected
    if(uart2Ferr())     status  |= uart2FrameErr();                             // Framing error detected
    if(uart2Overflow()){status  |= uart2Overrun(); uart2OverflowClr();}         // Overrun error detected; clear error
    
    while(uart2.txStart != uart2.txEnd){
        if(uart2TxbFull() < 1) uart2RegTx() = uart2.tx[uart2.txStart++];        // Copy data to UART TX buffer
    }//end while
    return status;                                                              // Return status to caller
}//end uart2TxService()

/*** _U2RXInterrupt ***************************************************************************************************************/
void __ISR(_UART2_RX_VECTOR, IPL6AUTO) uart2RxInterrupt(void){
    uart2RxService();                                                           // Service UART RX
    uart2RxIntFlagClr();                                                        // Clear UART RX interrupt flag
}//end _U2RXInterrupt()