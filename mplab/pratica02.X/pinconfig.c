#include "pinconfig.h"
#include <xc.h>
#include "lcd.h"

void configurePins()
{
    TRISCbits.RC0 = 0; /* Make RC0 pin as an output pin */
    TRISCbits.RC1 = 0; /* Make RC1 pin as an output pin */
    TRISCbits.RC2 = 0; /* Make RC2 pin as an output pin */

    TRISBbits.RB0 = 1; /* Make RB0 pin as an input pin */
    TRISBbits.RB1 = 1; /* Make RB1 pin as an input pin */
    TRISBbits.RB2 = 1; /* Make RB2 pin as an input pin */

    TRISBbits.TRISB0 = 1; /* Make INT0 pin as an input pin*/
    TRISBbits.TRISB1 = 1; /* Make INT1 pin as an input pin*/
    TRISBbits.TRISB2 = 1; /* Make INT2 pin as an input pin*/

    TRISDbits.RD0 = 0; /* Make RD0 pin as an output pin */
    TRISDbits.RD1 = 0; /* Make RD1 pin as an output pin */

    TRISDbits.RD4 = 0; /* Make RD4 pin as an output pin */
    TRISDbits.RD5 = 0; /* Make RD5 pin as an output pin */
    TRISDbits.RD6 = 0; /* Make RD6 pin as an output pin */
    TRISDbits.RD7 = 0; /* Make RD7 pin as an output pin */

    TRISDbits.RD2 = 0; /* Make RC2 pin as an output pin */
   
}

void configureIRQ()
{
    ADIE = 0; /* A/D interrupts will be used */
    PEIE = 1; /* all peripheral interrupts are enabled */
    ei();     /* enable all interrupts */

    INTCON2 = 0x00; /* Set Interrupt detection on falling Edge*/
    INTCON3 = 0x00;

    INTCONbits.INT0IF = 0; /* Clear INT0IF flag*/
    INTCONbits.INT0IE = 1; /* Enable INT0 external interrupt*/

    INTCON3bits.INT1F = 0; /* Clear INT1IF flag*/
    INTCON3bits.INT1E = 1; /* Enable INT1 external interrupt*/

    INTCON3bits.INT2F = 0; /* Clear INT2IF flag*/
    INTCON3bits.INT2E = 1; /* Enable INT2 external interrupt*/

    INTCON3bits.INT1IF = 0; // Initialize flag of interrupt
    INTCON3bits.INT2IF = 0; // Initialize flag of interrupt
    INTCONbits.GIE = 1;     /* Enable Global Interrupt*/
}

void initTimer()
{   
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.T2CKPS0 = 1;
    PR2 = 250; 
    
    T2CONbits.T2OUTPS3 = 1;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS1 = 0;
    T2CONbits.T2OUTPS0 = 1;
    
    TMR2IE = 1;
    TMR2IF = 0;
    
    T2CONbits.TMR2ON = 1;
}


void setup()
{
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    BUZZ = 0;

    configurePins();
    configureIRQ();

    inicializa_lcd();
    limpa_lcd();
    initTimer();
}