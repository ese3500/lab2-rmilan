//
// Created by Roberto Enrique Milan on 2/9/23.
//
#include <Arduino.h>
#include <util/delay.h>

ISR(TIMER1_CAPT_vect)
{
    if(TCCR1B & (1<<ICES1)) {
        PORTB |= (1 << PORTB5);
        TCCR1B &= ~(1<<ICES1);
    } else {
        PORTB &= ~(1 << PORTB5);
        TCCR1B |= (1<<ICES1);
    }
}

void Initialize()
{
    cli();
    //input capture
    DDRB &= ~(1<<DDB0);
    //read
    DDRB |= (1<<DDB5);

    //counts at 1 tick per 1us so 16 bit (how long till overflow?)
    //not really necessary yet but
    TCCR1B &= ~(1<<CS10);
    TCCR1B |= (1<<CS11);
    TCCR1B &= ~(1<<CS12);

    TCCR1A &= ~(1<<WGM10);
    TCCR1A &= ~(1<<WGM11);
    TCCR1B &= ~(1<<WGM12);
    TCCR1B &= ~(1<<WGM13);

    /*look for rising edge,
    send to ISR and then look for falling edge and run ISR
     once, needs to toggle in ISR*/
    TCCR1B |= (1<<ICES1); //sends to ISR when rising edge is found

    TIFR1 |= (1<<ICF1);

    TIMSK1 |= (1<<ICIE1);

    sei();
}

int main(void)
{
    //runs once
    Initialize();
    while(1){}

}*/