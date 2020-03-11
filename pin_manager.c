#include "pin_manager.h"

/*** pinManagerInitialize *********************************************************************************************************/
void pinManagerInitialize(void){
    
    /*=== Setting the Analog/Digital Configuration SFR(s) ========================================================================*/
    ANSELA  = 0x0000;                                                           // PORTA ANALOG INPUT: NONE
    ANSELB  = 0x0000;                                                           // PORTB ANALOG INPUT: NONE
    ANSELC  = 0x0000;                                                           // PORTC ANALOG INPUT: NONE
    ANSELD  = 0x0000;                                                           // PORTD ANALOG INPUT: NONE
    ANSELE  = 0x0000;                                                           // PORTE ANALOG INPUT: NONE
    ANSELF  = 0x0000;                                                           // PORTF ANALOG INPUT: NONE
    ANSELG  = 0x0000;                                                           // PORTG ANALOG INPUT: NONE

    /*=== Setting the GPIO Direction SFR(s) ======================================================================================*/
    TRISA   = 0xFFE1;                                                           // OUTPUTS: RC4-RC1
    TRISB   = 0x7FFF;                                                           // OUTPUTS: RB15
    TRISC   = 0x9FFF;                                                           // OUTPUTS: RC14-RC13
    TRISD   = 0x03F4;                                                           // OUTPUTS: RD15-RD10, RD3, RD1-RD0
    TRISE   = 0xFFE3;                                                           // OUTPUTS: RE4-RE2
    TRISF   = 0xDFF9;                                                           // OUTPUTS: RF13, RF2, RF0
    TRISG   = 0xFDBF;                                                           // OUTPUTS: RG9, RG6
    
    /*=== Setting the Output Latch SFR(s) ========================================================================================*/
    LATA    = 0x0000;                                                           // SET: NONE
    LATB    = 0x0005;                                                           // SET: RB2, RB0
    LATC    = 0x000E;                                                           // SET: RC3-RC1
    LATD    = 0x7000;                                                           // SET: RD14, RD13-RD12
    LATE    = 0x0000;                                                           // SET: NONE
    LATF    = 0x2004;                                                           // SET: RF13, RF2
    LATG    = 0x0000;                                                           // SET: NONE
    
    /*=== Setting the Open Drain SFR(s) ==========================================================================================*/
    ODCA    = 0x0000;                                                           // PORTA OD: NONE
    ODCB    = 0x0000;                                                           // PORTB OD: NONE
    ODCC    = 0x0000;                                                           // PORTC OD: NONE
    ODCD    = 0x0000;                                                           // PORTD OD: NONE
    ODCE    = 0x0000;                                                           // PORTE OD: NONE
    ODCF    = 0x0000;                                                           // PORTF OD: NONE
    ODCG    = 0x0000;                                                           // PORTG OD: NONE

    /*=== Setting the Weak Pull Up SFR(s) ========================================================================================*/
    CNPUA   = 0x0000;                                                           // PORTA PU: NONE
    CNPUB   = 0x0000;                                                           // PORTB PU: NONE
    CNPUC   = 0x0000;                                                           // PORTC PU: NONE
    CNPUD   = 0x0000;                                                           // PORTD PU: NONE
    CNPUE   = 0x0000;                                                           // PORTE PU: NONE
    CNPUF   = 0x0000;                                                           // PORTF PU: NONE
    CNPUG   = 0x0000;                                                           // PORTG PU: NONE

    /*=== Setting the Weak Pull Down SFR(s) ======================================================================================*/
    CNPDA   = 0x0000;                                                           // PORTA PD: NONE
    CNPDB   = 0x0000;                                                           // PORTB PD: NONE
    CNPDC   = 0x0000;                                                           // PORTC PD: NONE
    CNPDD   = 0x0000;                                                           // PORTD PD: NONE
    CNPDE   = 0x0000;                                                           // PORTE PD: NONE
    CNPDF   = 0x0000;                                                           // PORTF PD: NONE
    CNPDG   = 0x0000;                                                           // PORTG PD: NONE  
    
    /*=== Setting the Change Notice Control SFR(s) ===============================================================================*/
    CNCONA  = 0x0000;                                                           // PORTA CN: NONE
    CNCONB  = 0x0000;                                                           // PORTB CN: NONE
    CNCONC  = 0x0000;                                                           // PORTC CN: NONE
    CNCOND  = 0x0000;                                                           // PORTD CN: NONE
    CNCONE  = 0x0000;                                                           // PORTE CN: NONE
    CNCONF  = 0x0000;                                                           // PORTF CN: NONE  
    CNCONG  = 0x0000;                                                           // PORTG CN: NONE
    
    /*=== Setting the Change Notice Control Negative Edge SFR(s) =================================================================*/
    CNNEA   = 0x0000;                                                           // PORTA CN NE: NONE
    CNNEB   = 0x0000;                                                           // PORTB CN NE: NONE
    CNNEC   = 0x0000;                                                           // PORTC CN NE: NONE
    CNNED   = 0x0000;                                                           // PORTD CN NE: NONE
    CNNEE   = 0x0000;                                                           // PORTE CN NE: NONE
    CNNEF   = 0x0000;                                                           // PORTF CN NE: NONE  
    CNNEG   = 0x0000;                                                           // PORTG CN NE: NONE
    
    /*=== Setting the Change Notice Control Negative Edge SFR(s) =================================================================*/
    CNENA   = 0x0000;                                                           // PORTA CN PE: NONE
    CNENB   = 0x0000;                                                           // PORTB CN PE: NONE
    CNENC   = 0x0000;                                                           // PORTC CN PE: NONE
    CNEND   = 0x0000;                                                           // PORTD CN PE: NONE
    CNENE   = 0x0000;                                                           // PORTE CN PE: NONE
    CNENF   = 0x0000;                                                           // PORTF CN PE: NONE  
    CNENG   = 0x0000;                                                           // PORTG CN PE: NONE   
    
    /*=== Set the PPS ============================================================================================================*/
    SDI1R   = 0x00;                                                             // MISO1    -> RPD2
//    SDI2R   = 0x00;                                                             // MISO2    -> NOT USED
    SDI4R   = 0x07;                                                             // MISO4    -> RPC13
//    SDI6R   = 0x00;                                                             // MISO6    -> NOT USED
    
    RPD3R   = 0x05;                                                             // MOSI1    -> RPD3
    RPC4R   = 0x06;                                                             // MOSI2    -> RPC4
    RPD11R  = 0x08;                                                             // MOSI4    -> RPD11
    RPB15R  = 0x0A;                                                             // MOSI6    -> RPB15
    
    U2RXR   = 0x09;                                                             // UART2 RX -> RPF12
    RPF13R  = 0x02;                                                             // UART2 TX -> RPF13
    
    INT2R   = 0x04;                                                             // INT2     -> RPD4
}//end pinManagerInitialize()