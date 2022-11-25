#include <xc.h>
#include <pic18f4550.h>

#include <stdio.h>

#include "lcd.h"
#include "config.h"

//#pragma config PBADEN = OFF;
#pragma WDTEN = OFF

#define LED1 PORTCbits.RC0
#define LED2 PORTCbits.RC1
#define LED3 PORTCbits.RC2

#define SWT1 PORTBbits.RB0
#define SWT2 PORTBbits.RB1
#define SWT3 PORTBbits.RB2

#define BUZZ PORTCbits.RC7

#define IDLE 0
#define WAIT 1
#define CURRENT IDLE

int currentTime = 0;

// Override putch function to redirect printf from stdio to lcd
void putch(char data)
{
    escreve_lcd(data);   
}

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

    TRISCbits.RC7 = 0; /* Make RC7 pin as an output pin */
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

void initTimer0()
{   
    T2CONbits.T2CKPS1 = 0; // Pre scaler 1:4
    T2CONbits.T2CKPS0 = 1; // Pre scaler 1:4
    PR2 = 250;  // Para estourar a cada 1 ms
    //T2OUTPS3 a T2OUTPS0 controlam quantos 
    // estouros de timer2 precisam pra levar TMR2IF pra 1     
    T2CONbits.T2OUTPS3 = 1;
    T2CONbits.T2OUTPS2 = 0;
    T2CONbits.T2OUTPS1 = 0;
    T2CONbits.T2OUTPS0 = 1;
    
    TMR2IE = 1;
    TMR2IF = 0;
    
    T2CONbits.TMR2ON = 1; // Coloca timer pra contar
}


void setup()
{
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;

    configurePins();
    configureIRQ();

    inicializa_lcd();
    limpa_lcd();
    initTimer0();
}

void main(void)
{
    setup();
    printf("OK");

    while (1)
    {
        
    }
}

void clearInterruptFlags(){
    TMR2IF = 0;   
    INTCONbits.INT0IF = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IF = 0;
}

void __interrupt(high_priority) isr(void)
{
    if (INTCONbits.INT0IF){
        limpa_lcd();
        printf("Timmer: %d",currentTime);
        LED1 = ~(LED1);
        clearInterruptFlags();
        return;
    }
        
    if (INTCON3bits.INT1IF) {
        LED2 = ~(LED2);
        clearInterruptFlags();
        return;
    }
        

    if (INTCON3bits.INT2IF){
        LED3 = ~(LED3);
        clearInterruptFlags();
    }
       
    
    if(TMR2IF) {
        currentTime++;
        clearInterruptFlags();
        return;
    }
}

void state(){
    switch(CURRENT){
        case IDLE:
            break;
        case WAIT:
            break;
    }
}
