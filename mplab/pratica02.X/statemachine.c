#include <xc.h>
#include <pic18f4550.h>
#include <stdio.h>

#include "globalscope.h"
#include "lcd.h"
#include "config.h"
#include "pinconfig.h"

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
                    
                    if(p1concorrency){
                        player2Time = player1Time;
                        player2Pressed = true;
                        LED1 = 1;
                        LED2 = 1;
                    }

                    if(!player2Pressed) {
                        firstPlayer = PLAYER1;
                        LED1 = 1;
                    }
                    
                    player1Pressed = true;
                    displayPlayers();
                }
            }
            break;
        }
        case ST_P2: {
            if(gameStatus){
                if(!player2Pressed){
                    player2Time = (float)currentTime/100.0;
                    
                    if(p2concorrency){
                        player1Time = player2Time;
                        player1Pressed = true;
                        LED1 = 1;
                        LED2 = 1;
                    }

                    if(!player1Pressed) {
                        firstPlayer = PLAYER2;
                        LED2 = 1;
                    }

                    player2Pressed = true;
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