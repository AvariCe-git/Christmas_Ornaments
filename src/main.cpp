#include <Arduino.h>

long random_int(long x);

const int pin_enable = PB0;
const int pinA = PB1;
const int pinB = PB2;
const int pinC = PB3;
const int random_read = PB4;

const unsigned int delay_timer = 250;
const int i_max = 128;
long random_number = 0;

unsigned int leds[8][3] =   {{0,0,0}
                            ,{0,0,1}
                            ,{0,1,0}
                            ,{0,1,1}
                            ,{1,0,0}
                            ,{1,0,1}
                            ,{1,1,0}
                            ,{1,1,1}
                            };

void setup(){

    DIDR0 = 0x3F; // disable digital input buffers
    ACSR |= (1 << ACD); // disable analog comparator
    wdt_disable(); // disable watchdog timer

    //Black magic for the pwm to work. I don't understand it, but it works
    TCCR0A  = ((5<<WGM00) |(2<<COM0A0)) ; // mode Phase correct PWM
    TCCR0B  = ( (1<<CS00)); // clock input don't divide
    OCR0A = 0; // now it seems to be duty cycle - initial value

    pinMode(pin_enable,OUTPUT);
    pinMode(pinA,OUTPUT);
    pinMode(pinB,OUTPUT);
    pinMode(pinC,OUTPUT);
    pinMode(random_read,INPUT);
    
    randomSeed((random_read));
    random_number = random_int(0); 
}

void loop(){

    int i = 0;
    
    digitalWrite(pinA,leds[random_number][0]);
    digitalWrite(pinB,leds[random_number][1]);
    digitalWrite(pinC,leds[random_number][2]);
    
    digitalWrite(pin_enable, 1);
    
    for (i = 0; i < i_max; i++){
        OCR0A = i;
        delayMicroseconds(delay_timer);
    }
    
    for (i = i_max; i > 0; i--){
        OCR0A = i;
        delayMicroseconds(delay_timer);
    }
    
    digitalWrite(pin_enable, 0);
    
    random_number = random_int(random_number);
}

long random_int(long x){
    
    long y = random(0,8);
    
    if (y == x){
        y = random_int(y);
    }

    return y;
}