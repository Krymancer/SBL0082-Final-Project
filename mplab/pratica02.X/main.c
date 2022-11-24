#include <xc.h>
#include <pic18f4550.h>

#pragma config PBADEN = OFF
#pragma WDTEN=OFF

#define LED1 PORTDbits.RD0
#define LED2 PORTDbits.RD1
#define LED3 PORTDbits.RD2

#define SWT1 PORTBbits.RB0
#define SWT2 PORTBbits.RB1
#define SWT3 PORTBbits.RB2

void configurePins(){
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    
    TRISBbits.TRISB0 = 1;   /* Make INT0 pin as an input pin*/
    TRISBbits.TRISB1 = 1;   /* Make INT1 pin as an input pin*/
    TRISBbits.TRISB2 = 1;   /* Make INT2 pin as an input pin*/
}

void configureIRQ(){
    ADIE = 0;               /* A/D interrupts will be used */
    PEIE = 1;               /* all peripheral interrupts are enabled */
    ei();                   /* enable all interrupts */
    
    INTCON2 = 0x00;         /* Set Interrupt detection on falling Edge*/
    INTCON3 = 0x00;
    
    INTCONbits.INT0IF = 0;	/* Clear INT0IF flag*/
    INTCONbits.INT0IE = 1;	/* Enable INT0 external interrupt*/
    
    INTCON3bits.INT1F = 0;	/* Clear INT1IF flag*/
    INTCON3bits.INT1E = 1;	/* Enable INT1 external interrupt*/
    
    INTCON3bits.INT2F = 0;	/* Clear INT2IF flag*/
    INTCON3bits.INT2E = 1;	/* Enable INT2 external interrupt*/
    
    INTCON3bits.INT1IF = 0; // Initialize flag of interrupt
    INTCON3bits.INT2IF = 0; // Initialize flag of interrupt
    INTCONbits.GIE = 1;		/* Enable Global Interrupt*/
    
}

void setup(){
    LED1 = 0;
    LED2 = 0;
    LED3 = 0;
    
    configurePins();
    configureIRQ();
}

void main(void) {
    setup();
    
    while(1) {}
}

void __interrupt(high_priority) isr(void){
    if(INTCONbits.INT0IF) LED1 = ~(LED1);
    
    if(INTCON3bits.INT1IF) LED2 = ~(LED2);
    
    if(INTCON3bits.INT2IF) LED3 = ~(LED3);
    
    INTCONbits.INT0IF=0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT2IF = 0;
}

