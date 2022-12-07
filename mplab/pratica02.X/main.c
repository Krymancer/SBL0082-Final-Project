#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>

#include "config.h"
#include "globalscope.h"
#include "lcd.h"
#include "pinconfig.h"
#include "statemachine.h"

// Override putch function to redirect printf from stdio to lcd
void putch(char data)
{
    escreve_lcd(data);   
}

void main(void)
{
    setup();
    reset();
    while (1) stateMachine();
}

void __interrupt(high_priority) isr(void)
{
    if(INTCONbits.INT0IF){
        if(currentState == ST_P2) {
            p1concorrency = 1;
        }
        currentState = ST_P1;
        INTCONbits.INT0IF = 0;
        return;
    }
        
    if(INTCON3bits.INT1IF){
        if(currentState == ST_P1) {
            p2concorrency = 1;
        }
        currentState = ST_P2;
        INTCON3bits.INT1IF = 0;
        return;
    }
    
    if(INTCON3bits.INT2IF){
        currentState = ST_MP;
        INTCON3bits.INT2IF = 0;
        return;
    }
    
    if(TMR2IF){
        currentTime++; // 10ms
        TMR2IF = 0;
        return;
    }
}