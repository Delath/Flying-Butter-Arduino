#include <FastLED.h> 
const int pinLed = 11;
const int numLed = 50;
CRGB leds[numLed];

int val=0;
int state=0;
int motion=0;

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, pinLed, GRB>(leds, numLed);

    pinMode(10, INPUT_PULLUP);
}

void loop() {
    testLed();

    val = digitalRead(10);
    if(val==HIGH){
        delay(500);
        if(state==LOW){
            motion=0;
            state = HIGH;
        }
    }else{
        delay(500);
        if(state==HIGH){
            motion=1;
            state=LOW;
        }
    }
}

void testLed() {
  for (int i = 0 ; i < numLed ; i++) {
    leds[i] = CRGB(random(255), random(255), random(255));
    FastLED.show();
    delay(40);
  }
}