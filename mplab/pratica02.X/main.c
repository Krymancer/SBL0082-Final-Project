#include <xc.h>
#include <pic18f4550.h>

#include <stdio.h>

#include "lcd.h"
#include "config.h"
#include "pinconfig.h"

#define ST_IDLE 0xf01
#define ST_INIT 0xf02
#define ST_WAIT 0xf03
#define ST_RESET 0xf04

#define NO_PLAYER 0
#define PLAYER1 0xd01
#define PLAYER2 0xd02

int currentState = 0xff;

int currentTime = 0;

int firstPlayer = NO_PLAYER;

int player1Pressed = 0;
int player2Pressed = 0;

float player1Time = 0;
float player2Time = 0;

// Override putch function to redirect printf from stdio to lcd
void putch(char data)
{
    escreve_lcd(data);   
}

void displayPlayers(){
    limpa_lcd();
    if(player1Pressed && player2Pressed) {
        printf("P1: %.2f s",player1Time);        
        caracter_inicio(2,0);
        printf("P2: %.2f s", player2Time);
    }else if(player1Pressed && !player2Pressed) {
        printf("P1: %.2f s",player1Time);        
        caracter_inicio(2,0);
        printf("P2: - s");
    }else if(!player1Pressed && player2Pressed) {
        printf("P1: - s");        
        caracter_inicio(2,0);
        printf("P2: %.2f s", player2Time);
    }else {
        printf("P1: - s");        
        caracter_inicio(2,0);
        printf("P2: - s");
    }
}

void reset(){
    currentTime = 0;
    currentState = ST_IDLE;
    
    firstPlayer = NO_PLAYER;
    
    player1Time = 0;
    player2Time = 0;
    
    player1Pressed = 0;
    player2Pressed = 0;
    
    firstPlayer = 0;
    
    LED1 = 0;
    LED2 = 0;
    
    limpa_lcd();
    printf("IDLE...");
}

void stateMachine(){
    switch(currentState){
        case ST_IDLE: {
            break;
        }
        case ST_INIT: {
            BUZZ = 1;
            __delay_ms(500);
            BUZZ = 0;
            
            currentState = ST_WAIT;
            currentTime = 0;
            break;
        }
        case ST_WAIT: {
            break;
        }
        case ST_RESET: {
            reset();
            break;
        }
        default: {
            
        }
    }
}


void main(void)
{
    setup();
    
    printf("IDLE...");
    
    currentState = ST_IDLE;
   
    while (1)
    {
        stateMachine();
    }
}
void __interrupt(high_priority) isr(void)
{
    if (INTCONbits.INT0IF){
        if(currentState == ST_WAIT){
            if(!player1Pressed){
                player1Time = (float)currentTime/100.0;
            
                if(!player2Pressed) {
                    firstPlayer = PLAYER1;
                    LED1 = 1;
                }

                player1Pressed = 1;
                displayPlayers();
            }
        }
        
        INTCONbits.INT0IF = 0;
    }
        
    if (INTCON3bits.INT1IF) {
        if(currentState == ST_WAIT){
            if(!player2Pressed){
                player2Time = (float)currentTime/100.0;

                if(!player1Pressed) {
                    firstPlayer = PLAYER2;
                    LED2 = 1;
                }

                player2Pressed = 1;
                displayPlayers();
            }
        }
        
        INTCON3bits.INT1IF = 0;
    }
        

    if (INTCON3bits.INT2IF){
        if(currentState == ST_IDLE){
            currentState = ST_INIT;
        }
        
        if(currentState == ST_WAIT){
            currentState = ST_RESET;
        }
        INTCON3bits.INT2IF = 0;
    }
       
    
    if(TMR2IF) {
        currentTime++; // 10ms
        TMR2IF = 0;
    }
}