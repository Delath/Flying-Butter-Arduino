#include <FastLED.h> 
const int pinLed = 7; // or 7
const int numLed = 50;
CRGB leds[numLed];

int CLK = 11;
int DT = 12;
int RotPosition = 0;
int rotation;  
int value;
boolean LeftRight;

//int val=0;
//int state=0;
//int motionDetected=0;
int flag=0;
//int figurePassed1=0;
//int figurePassed2=0;
//int figurePassed3=0;
//int figurePassed4=0;

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, pinLed, GRB>(leds, numLed);

    pinMode (CLK,INPUT);
    pinMode (DT,INPUT);
    rotation = digitalRead(CLK);  

    //pinMode(10, INPUT_PULLUP);
}

void loop() {
    
    value = digitalRead(CLK);
    if (value != rotation){ // we use the DT pin to find out which way we turning.
        if (digitalRead(DT) != value) {  // Clockwise
            LeftRight = true;
            if(RotPosition<70){
                RotPosition ++;
            }
            Serial.println (RotPosition);
        } else { //Counterclockwise
            LeftRight = false;
            if(RotPosition>0){
                RotPosition--;
            }
            Serial.println (RotPosition);
        }

        if(RotPosition>=0 && RotPosition<10){
            if(flag!=0){
                lightOff();
                flag=0;
            }
        }else if(RotPosition>=10 && RotPosition<30){
            if(flag!=1){
                lightGreen();
                flag=1;
            }
        }else if(RotPosition>=30 && RotPosition<50){
            if(flag!=2){
                lightYellow();
                flag=2;
            }
        }else{
            if(flag!=3){
                lightRed();
                flag=3;
            }
        }


    }
    rotation = value;
    //val = digitalRead(10);
    //if(val==HIGH){
    //    delay(500);
    //    if(state==LOW){
    //        //motionDetected=0;
    //        state = HIGH;
    //    }
    //}else{
    //    delay(500);
    //    if(state==HIGH){
    //        //motionDetected=1;
    //        lightLeds1();
    //        figurePassed1++;
    //        state=LOW;
    //    }
    //}
}
void lightOff(){
    fastLED.clear();
    FastLED.show();
}
void lightGreen(){
    //delay(40);
    for (int i = 0 ; i < 50 ; i++) {
        leds[i] = CRGB(0, 255, 0);
        FastLED.show();
        //delay(40);
    }
}
void lightYellow(){
    //delay(40);
    for (int i = 0 ; i < 50 ; i++) {
        leds[i] = CRGB(255, 255, 0);
        FastLED.show();
        //delay(40);
    }
}
void lightRed(){
    //delay(40);
    for (int i = 0 ; i < 50 ; i++) {
        leds[i] = CRGB(255, 0, 0);
        FastLED.show();
        //delay(40);
    }
}

//void lightLeds1(){
//    if(figurePassed1<4){
//        delay(40);
//        for (int i = 1 ; i < 4 ; i++) {
//            leds[i+(figurePassed1*3)] = CRGB(255, 0, 0);
//            FastLED.show();
//            delay(40);
//        }
//    }
//}
//void lightLeds2(){
//    if(figurePassed2<4){
//        delay(40);
//        for (int i = 13 ; i < 16 ; i++) {
//            leds[i+(figurePassed2*3)] = CRGB(0, 255, 0);
//            FastLED.show();
//            delay(40);
//        }
//    }
//}
//void lightLeds3(){
//    if(figurePassed3<4){
//        delay(40);
//        for (int i = 25 ; i < 28 ; i++) {
//            leds[i+(figurePassed3*3)] = CRGB(0, 0, 255);
//            FastLED.show();
//            delay(40);
//        }
//    }
//}
//void lightLeds4(){
//    if(figurePassed4<4){
//        delay(40);
//        for (int i = 37 ; i < 40 ; i++) {
//            leds[i+(figurePassed4*3)] = CRGB(255, 255, 0);
//            FastLED.show();
//            delay(40);
//        }
//    }
//}
