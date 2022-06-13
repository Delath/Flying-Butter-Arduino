#include <FastLED.h> 
const int pinLed = 12;
const int numLed = 50;
CRGB leds[numLed];

int val=0;
int state=0;
//int motion=0;

int figurePassed1=0;
int figurePassed2=0;
int figurePassed3=0;
int figurePassed4=0;

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, pinLed, GRB>(leds, numLed);

    pinMode(10, INPUT_PULLUP);
}

void loop() {
    val = digitalRead(10);
    if(val==HIGH){
        delay(500);
        if(state==LOW){
            //motion=0;
            state = HIGH;
        }
    }else{
        delay(500);
        if(state==HIGH){
            //motion=1;
            lightLeds1();
            figurePassed1++;
            state=LOW;
        }
    }
}

void lightLeds1(){
    delay(40);
    for (int i = 0 ; i < 3 ; i++) {
        leds[i+(figurePassed1*3)] = CRGB(255, 0, 0);
        FastLED.show();
        delay(40);
    }
}
void lightLeds2(){
    delay(40);
    for (int i = 12 ; i < 15 ; i++) {
        leds[i+(figurePassed2*3)] = CRGB(0, 255, 0);
        FastLED.show();
        delay(40);
    }
}
void lightLeds3(){
    delay(40);
    for (int i = 24 ; i < 27 ; i++) {
        leds[i+(figurePassed3*3)] = CRGB(0, 0, 255);
        FastLED.show();
        delay(40);
    }
}
void lightLeds4(){
    delay(40);
    for (int i = 36 ; i < 39 ; i++) {
        leds[i+(figurePassed4*3)] = CRGB(255, 255, 0);
        FastLED.show();
        delay(40);
    }
}