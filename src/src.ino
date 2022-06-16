#include <FastLED.h> 
#define CLK 11
#define DT 12
#define BUTTON_PIN 2
#define LED_PIN 7
#define NUM_LED 50
#define SENSOR1_PIN 10
#define SENSOR2_PIN 6
#define SENSOR3_PIN 12
#define SENSOR4_PIN 13

int S_pin[4]={SENSOR1_PIN,SENSOR2_PIN,SENSOR3_PIN,SENSOR4_PIN};
CRGB leds[NUM_LED];
int phase=0;
int RotPosition=0;
int rotation;  
int value;
boolean LeftRight;
int enabler=0;
int val[4]={0,0,0,0};
int state[4]={0,0,0,0};
int figuresPassed[4]={0,0,0,0};
int winner[4];
int num_of_winners=0;

void setup() {
    Serial.begin(115200);
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LED);
    pinMode (BUTTON_PIN, INPUT);
    pinMode (CLK,INPUT);
    pinMode (DT,INPUT);
    digitalWrite (BUTTON_PIN, true);
    rotation = digitalRead(CLK);
    for(int i=0;i<4;i++){
        pinMode(S_pin[i], INPUT_PULLUP);
    }
}

void loop() {
    switch(phase){
        case 0:
            configPhase();
        break;
        case 1:
            gamePhase();
        break;
        case 2:
            endPhase();
        break;
    }
}

void configPhase(){
    value = digitalRead(CLK);
    if(value != rotation){
        if(digitalRead(DT)!=value){//Clockwise
            LeftRight = true;
            if(RotPosition<70){
                RotPosition ++;
            }
            //Serial.println(RotPosition); //For testing purposes, to be deleted later
        }else{//Counterclockwise
            LeftRight = false;
            if(RotPosition>0){
                RotPosition--;
            }
            //Serial.println(RotPosition); //For testing purposes, to be deleted later
        }
        ledManager();
    }
    rotation = value;
    if(digitalRead(BUTTON_PIN)==LOW){
        ledOff();
        phase=1;
    }
}
// multitrading per la gestione di delay dovuti a piu sensori
void gamePhase(){
  Serial.println(phase);
    for(int i=0;i<4;i++){
        motionSensors(i);
    }
    winCheck();
}

void endPhase(){
    for(int i=0;i<num_of_winners;i++){
        colorWinners(winner[i]);
    }
    FastLED.show();
    if(digitalRead(BUTTON_PIN)==LOW){
        ledOff();
        prepareNewGame();
        phase=0;
    }
}

void ledManager(){
    if(RotPosition>=0 && RotPosition<10){
        if(enabler!=0){
            ledOff();
            enabler=0;
        }
    }else if(RotPosition>=10 && RotPosition<30){
        if(enabler!=1){
            ledOn(0);
            enabler=1;
        }
    }else if(RotPosition>=30 && RotPosition<50){
        if(enabler!=2){
            ledOn(1);
            enabler=2;
        }
    }else{
        if(enabler!=3){
            ledOn(2);
            enabler=3;
        }
    }
}

void ledOff(){
    FastLED.clear();
    FastLED.show();
}

void ledOn(int color){
    for (int i = 0 ; i < 50 ; i++) {
        switch(color){
            case 0:
                leds[i] = CRGB(0, 255, 0);
            break;
            case 1:
                leds[i] = CRGB(255, 255, 0);
            break;
            case 2:
                leds[i] = CRGB(255, 0, 0);
            break;
        }
        FastLED.show();
    }
}

void motionSensors(int i){
    val[i] = digitalRead(S_pin[i]);
    if(val[i]==HIGH){
        delay(50);//Might want to adjust this value
        if(state[i]==LOW){
            state[i]=HIGH;
        }
    }else{
       //delay(50);//Might want to adjust this value
        if(state[i]==HIGH){
            onFigure(i);
            state[i]=LOW;
        }
    }
}

void onFigure(int i){
    figuresPassed[i]++;
    delay(40);//Might want to adjust this value
    for (int j = 1+(i*12) ; j < 4+(i*12) ; j++) {
        colorLeds(i, (j+(figuresPassed[i]*3)));
        FastLED.show();
        delay(40);//Might want to adjust this value
    }
}

void colorLeds(int i, int led){
    switch(i){
        case 0://Red
            leds[led] = CRGB(255, 0, 0);
        break;
        case 1://Green
            leds[led] = CRGB(0, 255, 0);
        break;
        case 2://Blue
            leds[led] = CRGB(0, 0, 255);
        break;
        case 3://yellow
            leds[led] = CRGB(255, 255, 0);
        break;
    }
}

void colorWinners(int i){
    delay(40);//Might want to adjust this value
    for (int j = 1+(i*12) ; j < 4+(i*12) ; j++) {
        colorLeds(i, (j+(figuresPassed[i]*3)));
        FastLED.show();
        delay(40);//Might want to adjust this value
    }
}

void winCheck(){
    for(int i=0;i<4;i++){
        if(figuresPassed[i]>=4){
            winner[0]=i;
            num_of_winners=1;
            ledOff();
            phase=2;
            break;
        }
    }
}

void prepareNewGame(){
    phase=0;
    enabler=4;
    for(int i=0;i<4;i++){
        val[i]=0;
        state[i]=0;
        figuresPassed[i]=0;
        num_of_winners=0;
    }
}
