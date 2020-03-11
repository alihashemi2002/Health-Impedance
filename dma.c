#include "dma.h"

/**********************************************************************************************************************************/
/*=== VARIABLES ==================================================================================================================*/
/**********************************************************************************************************************************/
static __attribute__((coherent)) dmaTxBuf_obj dmaTx1 = 
                                {.label     = dmaTxBuf1(),
                                 .txCount   = 0,
                                 .usedBy    = dmaNulLabel(),
                                 .error     = dmaErrNop()};
static __attribute__((coherent)) dmaTxBuf_obj dmaTx2 = 
                                {.label     = dmaTxBuf2(),
                                 .txCount   = 0,
                                 .usedBy    = dmaNulLabel(),
                                 .error     = dmaErrNop()};
static __attribute__((coherent)) dmaTxBuf_obj dmaTx3 = 
                                {.label     = dmaTxBuf3(),
                                 .txCount   = 0,
                                 .usedBy    = dmaNulLabel(),
                                 .error     = dmaErrNop()};

       __attribute__((coherent)) dma_obj dma =
                                {.enums.label.dma       = dmaModule(),
                                 .enums.label.src1      = dmaSrc1(),
                                 .enums.label.src2      = dmaSrc2(),
                                 .enums.label.txBuf1    = dmaTxBuf1(),
                                 .enums.label.txBuf2    = dmaTxBuf2(),
                                 .enums.label.txBuf3    = dmaTxBuf3(),
                                 .enums.label.null      = dmaNulLabel(),
                                 .enums.err.allBufInUse = dmaErrAllBufUsed(),
                                 .enums.err.bufOvfl     = dmaErrTxOvfl(),
                                 .enums.err.chBusy      = dmaErrChBusy(),
                                 .enums.err.msgTooBig   = dmaErrMsgTooBig(),
                                 .enums.err.doesNotExist= dmaErrDoesNotExist(),
                                 .enums.err.allBufBusy  = dmaErrAllBufBusy(),
                                 .enums.err.noData      = dmaErrNoData(),
                                 .enums.err.nop         = dmaErrNop(),
                                 .error                 = dmaErrNop()};

/**********************************************************************************************************************************/
/*=== FUNCTIONS ==================================================================================================================*/
/**********************************************************************************************************************************/
       
/*** DMA_Initialize ***************************************************************************************************************/
void DMA_Initialize(void){
    dmaEn();                                                                    // Enable DMA 
    dmaCh0IrqOff();                                                             // DMA CH0 interrupt off (interrupt controller)
    dmaCh0IsrClr();                                                             // Clear DMA CH0 ISR flag (interrupt controller)
    dmaCh1IrqOff();                                                             // DMA CH1 interrupt off (interrupt controller)
    dmaCh1IsrClr();                                                             // Clear DMA CH1 ISR flag (interrupt controller)
    
    // --- INITIALIZE CH0 --------------------------------------------------------------------------------------------------------
    DCH0CON             = dmaCh0ConDflt();                                      // Load defaults
    dmaInt0SetDflt();                                                           // Set defaults
    DCH0ECON            = dmaCh0EconDflt();                                     // Load default trigger and abort settings
    DCH0SSA             = dmaSrc0Dflt();                                        // Load defaults
    DCH0DSA             = dmaDst0Dflt();                                        // Load defaults
    DCH0SSIZ            = dmaSrc0SizeDflt();                                    // Load defaults
    DCH0DSIZ            = dmaDst0SizeDflt();                                    // Load defaults
    DCH0CSIZ            = dmaCellSizeDflt();                                    // Load defaults
    
    // --- INITIALIZE CH1 --------------------------------------------------------------------------------------------------------
    DCH1CON             = dmaCh1ConDflt();                                      // Load defaults
    dmaInt1SetDflt();                                                           // Set defaults
    DCH1ECON            = dmaCh1EconDflt();                                     // Load default trigger and abort settings
    DCH1SSA             = KVA_TO_PA(dmaTx1.tx);                                 // Load defaults
    DCH1DSA             = dmaDst1Dflt();                                        // Load defaults
    DCH1SSIZ            = dmaSrc1SizeDflt();                                    // Load defaults
    DCH1DSIZ            = dmaDst1SizeDflt();                                    // Load defaults
    DCH1CSIZ            = dmaCellSizeDflt();                                    // Load defaults
    
    dmaCh0En();                                                                 // Enable DMA CH0
}//end DMA_Initialize()

/*** _DMA0Interrupt ***************************************************************************************************************/
void __ISR(_DMA0_VECTOR, IPL3AUTO) _DMA0Interrupt(void){
    dmaCh0IsrClr();
}//end _DMA0Interrupt()

/*** _DMA1Interrupt ***************************************************************************************************************/
void __ISR(_DMA1_VECTOR, IPL2AUTO) _DMA1Interrupt(void){
    dmaCh1IsrClr();
}//end _DMA1Interrupt()

/*** dmaCh0Init *******************************************************************************************************************/
void dmaCh0Init(void){
    dmaCh0Dis();                                                                // Disable CH0
    DCH0CON             = dmaCh0ConDflt();                                      // Load CH0 configuration defaults
    dmaInt0SetDflt();                                                           // Load CH0 configuration defaults
    DCH0ECON            = dmaCh0EconDflt();                                     // Load CH0 trigger and abort defaults
    DCH0SSA             = dmaSrc0Dflt();                                        // Load CH0 source defaults
    DCH1DSA             = dmaDst1Dflt();                                        // Load defaults
    DCH0SSIZ            = dmaSrc0SizeDflt();                                    // Load CH0 source size defaults
    DCH0DSIZ            = dmaDst0SizeDflt();                                    // Load CH0 destination size defaults
    DCH0CSIZ            = dmaCellSizeDflt();                                    // Load CH0 transfer size per event defaults
    dmaCh0En();                                                                 // Enable CH0
}//end dmaCh0Init()

/*** dmaCh1Init *******************************************************************************************************************/
void dmaCh1Init(void){
    dmaCh1Dis();                                                                // Disable CH1
    DCH1CON             = dmaCh1ConDflt();                                      // Load CH1 configuration defaults
    dmaInt1SetDflt();                                                           // Load CH1 configuration defaults
    DCH1SSA             = KVA_TO_PA(dmaTx1.tx);                                 // Load defaults
    DCH1DSA             = dmaDst1Dflt();                                        // Load CH1 destination defaults
    DCH1SSIZ            = dmaSrc1SizeDflt();                                    // Load CH1 source size defaults
    DCH1DSIZ            = dmaDst1SizeDflt();                                    // Load CH1 destination size defaults
    DCH1CSIZ            = dmaCellSizeDflt();                                    // Load CH1 transfer size per event defaults
    dmaCh1En();                                                                 // Enable CH1
}//end dmaCh1Init()

/*** dmaCh0RxChars ****************************************************************************************************************/
uint16_t dmaCh0RxChars(void){return DCH0DPTR;}

/*** *getDmaUartRxBuf *************************************************************************************************************/
volatile uint8_t *getDmaUartRxBuf(void){return dma.rx1;}

/*** dmaUartTxServEn **************************************************************************************************************/
void dmaUartTxServEn(void){dma.uartTxServReq = true;}

/*** dmaUartTxServDis *************************************************************************************************************/
void dmaUartTxServDis(void){dma.uartTxServReq = false;}

/*** dmaUartTxServ ****************************************************************************************************************/
uint8_t dmaUartTxServ(void){
    if(dmaCh1EnStat()&&(DCH1CONbits.CHBUSY))    return dma.enums.err.chBusy;    // DMA CH1 is busy
    if(dma.error != dma.enums.err.nop)          return dma.error;               // DMA error present; Return error
    
    uint8_t n;
    dmaTxBuf_obj *src;                                                          // Local dmaTxBuf_obj pointer
    
    for(n = dmaTxLabelMin(); n < (dmaTxLabelMax()+1); n++){
        src = getDmaBufObj(n);                                                  // Get TX object
        if(src->usedBy == dma.enums.label.dma){                                 // Was object used by the DMA module?
            src->usedBy = dma.enums.label.null; continue;}                      // Release object and skip iteration
        if((src->usedBy == dma.enums.label.null)&&(src->txCount > 0)){          // Object is not actively written to and has data
            src->usedBy     = dma.enums.label.dma;                              // Assign object to DMA
            DCH1SSA         = KVA_TO_PA(&src->tx);                              // Assign buffer to DMA source
            DCH1DSA         = dmaDst1Dflt();                                    // Load defaults
            DCH1SSIZ        = src->txCount;                                     // Load number of bytes to send
            src->txCount    = 0;                                                // Reset counter
            DCH1DSIZ        = dmaDst1SizeDflt();                                // Load CH1 destination size defaults
            DCH1CSIZ        = dmaCellSizeDflt();                                // Load CH1 transfer size per event defaults
            dmaCh1En();                                                         // Enable DMA channel 1
            uart2TxEn();                                                        // Enable UART 2 TX
            return dma.enums.err.nop;}                                          // Return success
    }//end for
    return dma.enums.err.noData;                                                // No data available
}//end dmaUartTxServ()


/**** getDmaBufObj ****************************************************************************************************************/
dmaTxBuf_obj *getDmaBufObj(uint8_t selector){
    if(selector == dmaTx1.label){dma.error = dma.enums.err.nop; return &dmaTx1;}// No error; return dmaTx1
    if(selector == dmaTx2.label){dma.error = dma.enums.err.nop; return &dmaTx2;}// No error; return dmaTx2
    if(selector == dmaTx3.label){dma.error = dma.enums.err.nop; return &dmaTx3;}// No error; return dmaTx3
    dma.error = dma.enums.err.doesNotExist; return &dmaTx1;                     // Log error; return dmaTx1
}//end getDmaBufObj()

/**** getDmaUartTxBuf *************************************************************************************************************/
volatile uint8_t *getDmaUartTxBuf(uint8_t msgSize, uint8_t user){
    if(dma.error != dma.enums.err.nop)return dmaTx1.tx;                         // DMA error present; Return txBuf1.tx
    
    uint8_t temp, n;                                                            // Local variables
    dmaTxBuf_obj *src;                                                          // Local pointer
    for(n = dmaTxLabelMin(); n < (dmaTxLabelMax()+1); n++){                     // Look for free buffer object
        src = getDmaBufObj(n);                                                  // Get TX buffer object
        if(src->usedBy == dma.enums.label.null){                                // Object not in use
            temp = (uint8_t)(dmaBufLen() - src->txCount);                       // Get buffer left over space
            if(temp > msgSize){                                                 // There is enough buffer space
                dma.error = dma.enums.err.nop;                                  // No error
                src->usedBy = user; return &src->tx[src->txCount];}}            // Return address of buffer
    }//end for
    dma.error = dma.enums.err.allBufBusy;                                       // Log error
    return dmaTx1.tx;
}//end *getDmaUartTxBuf()

/**** dmaReleaseBuf ***************************************************************************************************************/
volatile uint8_t dmaReleaseBuf(uint8_t user){
    if((user < dmaUserLabelMin())||(user > dmaUserLabelMax())){                 // User label does not exist
        dma.error = dma.enums.err.doesNotExist;                                 // Log error
        return dma.enums.err.doesNotExist;}                                     // Exit
    if      (user == dmaTx1.usedBy) dmaTx1.usedBy = dma.enums.label.null;       // Used object is dmaTx1
    else if (user == dmaTx2.usedBy) dmaTx2.usedBy = dma.enums.label.null;       // Used object is dmaTx2
    else if (user == dmaTx3.usedBy) dmaTx3.usedBy = dma.enums.label.null;       // Used object is dmaTx3
    return dma.enums.err.nop;                                                   // Success
}//end dmaReleaseBuf()

/*** dmaSetTxMsgSize **************************************************************************************************************/
uint8_t dmaSetTxMsgSize(uint8_t numOfBytes, uint8_t user){
    if(dma.error != dma.enums.err.nop)  return dma.error;                       // DMA error present; Return error
    if(numOfBytes > dmaBufLen())        return dma.enums.err.msgTooBig;         // DMA buffer length exceeded
    
    uint8_t temp;                                                               // Local byte counter
    dmaTxBuf_obj *src;                                                          // Local object pointer
    
    if      (user == dmaTx1.usedBy)src = &dmaTx1;                               // Used object is dmaTx1
    else if (user == dmaTx2.usedBy)src = &dmaTx2;                               // Used object is dmaTx2
    else if (user == dmaTx3.usedBy)src = &dmaTx3;                               // Used object is dmaTx3   
    temp = src->txCount + numOfBytes;
    if(temp >= dmaBufLen()) return dma.enums.err.bufOvfl;                       // DMA TX buffer pending overflow
    src->txCount = temp;                                                        // Update object byte counter
    return dma.enums.err.nop;                                                   // Success
}//end dmaSetTxMsgSize()