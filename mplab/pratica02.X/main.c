#include <xc.h>
#include <pic18f4550.h>

#include <stdio.h>

#include "globalscope.h"
#include "lcd.h"
#include "config.h"
#include "pinconfig.h"

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
    gameStatus = false;
    currentTime = 0;
    firstPlayer = NO_PLAYER;
    
    player1Time = 0;
    player2Time = 0;
    
    player1Pressed = false;
    player2Pressed = false;
   
    LED1 = 0;
    LED2 = 0;
    
    currentState = ST_IDLE;
    
    limpa_lcd();
    printf("IDLE...");
}

void stateMachine(){
    switch(currentState){
        case ST_IDLE: {
            break;
        }
        case ST_WAIT: {
            if(player1Pressed && player2Pressed) gameStatus = false;
            break;
        }
        case ST_INIT: {
            BUZZ = 1;
            __delay_ms(500);
            BUZZ = 0;
            
            currentState = ST_WAIT;
            
            limpa_lcd();
            printf("WAITING...");
            gameStatus = true;
            currentTime = 0;
            break;
        }
        case ST_RESET: {
            reset();
            break;
        }
        case ST_MP: {
            if((gameStatus) && (player1Pressed) && (player2Pressed)){
                currentState = ST_RESET;
            }else if(!gameStatus) {
                currentState = ST_INIT;
            }
            else {
                currentState = ST_WAIT;
            }
            break;
        }
        case ST_P1: {
            if(gameStatus){
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
            break;
        }
        case ST_P2: {
            if(gameStatus){
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
            break;
        }
        default: {
            break;
        }
    }
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
        currentState = ST_P1;
        INTCONbits.INT0IF = 0;
    }
        
    if(INTCON3bits.INT1IF){
        currentState = ST_P2;
        INTCON3bits.INT1IF = 0;
    }
    
    if(INTCON3bits.INT2IF){
        currentState = ST_MP;
        INTCON3bits.INT2IF = 0;
    }
    
    if(TMR2IF){
        currentTime++; // 10ms
        TMR2IF = 0;
    }
}