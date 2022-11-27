#ifndef PINCONFIG_H
#define	PINCONFIG_H

#define LED1 PORTCbits.RC0
#define LED2 PORTCbits.RC1
#define LED3 PORTCbits.RC2

#define SWT1 PORTBbits.RB0
#define SWT2 PORTBbits.RB1
#define SWT3 PORTBbits.RB2

#define BUZZ PORTDbits.RD2

void configurePins();
void configureIRQ();
void initTimer();
void setup();

#endif	/* PINCONFIG_H */

