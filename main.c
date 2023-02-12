#include <Arduino.h>
#include <util/delay.h>
#include "inc/uart.h"
#include <stdlib.h>
#include <stdio.h>

//#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

char String[25];
char code[5];
char answer[4];
volatile int index;
volatile uint16_t edge1 = 0;
volatile uint16_t edge2 = 0;
volatile uint16_t space = 0;
volatile uint16_t pwm = 0;
volatile uint16_t overflow = 0;
volatile uint16_t on = 0;
volatile uint16_t overflow2 = 0;




void reset() {
    for(int i = 0; i < 5; i++){
        code[i] = '9';
        index = 0;
    }
}

void morse(void){
    if(code[0] == '1') {
        if(code[1] == '9') {
            sprintf(answer, "E \n");
            UART_putstring(answer);

        } else if(code[1] == '1') {
            if(code[2] == '9') {
                sprintf(answer, "I \n");
                UART_putstring(answer);

            } else if(code[2] == '1') {
                if(code[3] == '9') {
                    sprintf(answer, "S \n");
                    UART_putstring(answer);

                } else if(code[3] == '1'){
                    if(code[4] == '9') {
                        sprintf(answer, "H \n");
                        UART_putstring(answer);

                } else if(code[4] == '1') {
                        sprintf(answer, "5 \n");
                        UART_putstring(answer);

                    } else {
                        sprintf(answer, "4 \n");
                        UART_putstring(answer);
                    }
                } else if(code[4] == '9') {
                    sprintf(answer, "V \n");
                    UART_putstring(answer);
                } else {
                    sprintf(answer, "3 \n");
                    UART_putstring(answer);
                }

            } else {
                if(code[3] == '9') {
                    sprintf(answer, "U \n");
                    UART_putstring(answer);

                } else if(code[4] == '9'){
                    sprintf(answer, "F \n");
                    UART_putstring(answer);

                } else {
                    sprintf(answer, "2 \n");
                    UART_putstring(answer);

                }
            }
        } else {
            if(code[2] == '9') {
                sprintf(answer, "A \n");
                UART_putstring(answer);

            } else if(code[2] == '1') {
                if(code[3] == '9') {
                    sprintf(answer, "R \n");
                    UART_putstring(answer);

                } else if (code[3] == '1') {
                    if(code[4] == '9') {
                        sprintf(answer, "L \n");
                        UART_putstring(answer);
                    }
                }
            } else {
                if(code[3] == '9') {
                    sprintf(answer, "W \n");
                    UART_putstring(answer);

                } else if(code[3] == '1') {
                    sprintf(answer, "P \n");
                    UART_putstring(answer);

                } else {
                    if(code[4] == '9'){
                        sprintf(answer, "J \n");
                        UART_putstring(answer);

                    } else {
                        sprintf(answer, "1 \n");
                        UART_putstring(answer);
                    }

                }
            }

        }
    } else if (code[0] == '0'){
        if(code[1] == '9') {
            sprintf(answer, "T \n");
            UART_putstring(answer);

        } else if(code[1] == '1') {
            if(code[2] == '9') {
                sprintf(answer, "N \n");
                UART_putstring(answer);

            } else if(code[2] == '1') {
                if(code[3] == '9') {
                    sprintf(answer, "D \n");
                    UART_putstring(answer);

                } else if(code[3] == '1'){
                    if(code[4] == '9') {
                        sprintf(answer, "B \n");
                        UART_putstring(answer);

                    } else {
                        sprintf(answer, "6 \n");
                        UART_putstring(answer);
                    }
                } else {
                    sprintf(answer, "X \n");
                    UART_putstring(answer);
                }

            } else {
                if(code[3] == '9') {
                    sprintf(answer, "K \n");
                    UART_putstring(answer);

                } else if(code[3] == '1'){
                    sprintf(answer, "C \n");
                    UART_putstring(answer);

                } else {
                    sprintf(answer, "Y \n");
                    UART_putstring(answer);

                }
            }
        } else {
            if(code[2] == '9') {
                sprintf(answer, "M \n");
                UART_putstring(answer);

            } else if(code[2] == '1') {
                if(code[3] == '9') {
                    sprintf(answer, "G \n");
                    UART_putstring(answer);

                } else if (code[3] == '1') {
                    if(code[4] == '9') {
                        sprintf(answer, "Z \n");
                        UART_putstring(answer);
                    } else {
                        sprintf(answer, "7 \n");
                        UART_putstring(answer);
                    }
                } else {
                    sprintf(answer, "Q \n");
                    UART_putstring(answer);
                }
            } else {
                if(code[3] == '9') {
                    sprintf(answer, "O \n");
                    UART_putstring(answer);

                } else if(code[3] == '1') {
                    sprintf(answer, "8 \n");
                    UART_putstring(answer);

                } else {
                    if(code[4] == '1'){
                        sprintf(answer, "9 \n");
                        UART_putstring(answer);

                    } else {
                        sprintf(answer, "0 \n");
                        UART_putstring(answer);
                    }

                }
            }

        }
        }
    }

ISR(TIMER1_CAPT_vect)
{
    if(TCCR1B & (1<<ICES1)){
        edge1 = TCNT1;
        on = 1;

        if(((TCNT1 + (overflow2 * 65535)) - space) > 30000) {
            morse();

            overflow2 = 0;
            space = 0;

            reset();

            sprintf(String,"Space \n");
            UART_putstring(String);
        }
        TCCR1B &= ~(1<<ICES1);
    } else if (TCCR1B & ~(1<<ICES1)){
        on = 0;
        edge2 = TCNT1;
        pwm = (65535 * overflow) + edge2 - edge1 + overflow;
        /*sprintf(String, "%u  \n", overflow);
        UART_putstring(String);
        sprintf(String, "%u  \n", edge2);
        UART_putstring(String);
        sprintf(String, "%u  \n", edge1);
        UART_putstring(String);*/
        if((pwm > 1875) && (pwm <= 12500)){

            code[index] = '1';
            index++;

            space = TCNT1;

            PORTB |= (1 << PORTB1);
            _delay_ms(50);
            PORTB &= ~(1 << PORTB1);

            //dot
            sprintf(String, "Dot \n");
            UART_putstring(String);
            TCCR1B |= (1<<ICES1);
        } else if((pwm > 12500) && (pwm <= 30000)){

            code[index] = '0';
            index++;

            space = TCNT1;

            PORTB |= (1 << PORTB2);
            _delay_ms(50);
            PORTB &= ~(1 << PORTB2);

            sprintf(String,"Dash \n");
            UART_putstring(String);
            TCCR1B |= (1<<ICES1);
        }
        overflow = 0;
    }
}

ISR(TIMER1_OVF_vect){
    if(on){
        //sometimes the button got stuck in the on position
        /*sprintf(String, "%u \n",pwm);
        UART_putstring(String);*/
        overflow++;
    } else {
        overflow2++;
    }
}

void Initialize()
{
    cli();
    //input capture
    DDRB &= ~(1<<DDB0);

    reset();

    DDRB |= (1<<DDB1);
    DDRB |= (1<<DDB2);

    //counts at 1 tick per 1us so 16 bit (how long till overflow?)
    //not really necessary yet but
    //set timer to count up to at most 500ms prior to overflow
    TCCR1B &= ~(1<<CS10);
    TCCR1B &= ~(1<<CS11);
    TCCR1B |= (1<<CS12);

    //set clock

    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    TCCR1B &= ~(1<<WGM12);
    TCCR1B &= ~(1<<WGM13);




    /*look for rising edge,
    send to ISR and then look for falling edge and run ISR
     once, needs to toggle in ISR*/
    TCCR1B |= (1<<ICES1); //sends to ISR when rising edge is found

    TIFR1 |= (1<<ICF1);
    TIFR1 |= (1<<TOV1);

    TIMSK1 |= (1<<ICIE1);
    TIMSK1 |= (1<<TOIE1);

    sei();
}

int main(void)
{
    //runs once
    Initialize();
    UART_init(BAUD_PRESCALER);
    while(1){
    }
}