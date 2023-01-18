#include <FastLED.h> 

#define CLK 11
#define DT 12
#define BUTTON_PIN 2
#define LED_PIN 7
#define NUM_LED 50
#define SENSOR1_PIN 10 //red
#define SENSOR2_PIN 9 //green
#define SENSOR3_PIN 8 //blue
#define SENSOR4_PIN 5 //yellow

long MILLIS_PER_DIFFICULTY=10000; //30 seconds per difficulty
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
long start;
int difficulty=0;
int max=0;
int flag=0;
//                    0    1    2    3    4    5    6    7    8    9    10   11   12
const int sounds[] = {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};
//                   DO   DO#   RE  RE#   MI   FA  FA#  SOL  SOL#  LA  LA#   SI   DO
const int finalSounds[][4] = {
  {sounds[2], sounds[2], sounds[0], sounds[0]},
  {sounds[0], sounds[0], sounds[2], sounds[2]},
  {sounds[2], sounds[0], sounds[2], sounds[2]},
  {sounds[5], sounds[7], sounds[9], sounds[12]},
};
const int BUZZER_PIN1 = A1;

/*---------*/
/*  setup  */
/*---------*/
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

/*--------*/
/*  loop  */
/*--------*/
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

/*------------------------------*/
/*  difficulty selection phase  */
/*------------------------------*/
void configPhase(){
    value = digitalRead(CLK);
    if(value != rotation){
        if(digitalRead(DT)!=value){//Clockwise
            LeftRight = true;
            if(RotPosition<70){
                RotPosition ++;
            }
        }else{//Counterclockwise
            LeftRight = false;
            if(RotPosition>0){
                RotPosition--;
            }
        }
        ledManager();
    }
    rotation = value;
    if(digitalRead(BUTTON_PIN)==LOW && difficulty!=0){
        ledOff();
        long difficultyMolt = MILLIS_PER_DIFFICULTY*((long)difficulty);
        start = (long)millis();
        start=start+difficultyMolt;
        phase=1;
    }
}

/*--------------*/
/*  game phase  */
/*--------------*/
void gamePhase(){
    for(int i=0;i<4;i++){
        motionSensors(i);
    }
    winCheck();
}

/*-----------------*/
/*  winners phase  */
/*-----------------*/
void endPhase(){
    if(flag==0){
        for(int i=0;i<num_of_winners;i++){
            colorWinners(winner[i]);
            FastLED.show();
            delay(100);
        }
        flag=1;
    }
    for(int i=0;i<4;i++){
        soundSpeaker(i);
    }
    if(digitalRead(BUTTON_PIN)==LOW){
        ledOff();
        prepareNewGame();
        phase=0;
    }
}

/*-------------*/
/*  functions  */
/*-------------*/
void ledManager(){
    if(RotPosition>=0 && RotPosition<10){
        if(enabler!=0){
            ledOff();
            enabler=0;
            difficulty=0;
        }
    }else if(RotPosition>=10 && RotPosition<30){
        if(enabler!=1){
            ledOn(0);
            enabler=1;
            difficulty=3;
        }
    }else if(RotPosition>=30 && RotPosition<50){
        if(enabler!=2){
            ledOn(1);
            enabler=2;
            difficulty=2;
        }
    }else{
        if(enabler!=3){
            ledOn(2);
            enabler=3;
            difficulty=1;
        }
    }
}

void ledOff(){
    FastLED.clear();
    FastLED.show();
}

void ledOn(int color){
    for (int i = 0 ; i < 20 ; i++) {
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
    delay(40);//Might want to adjust this value
    for (int j = (i*5) ; j < 1+(i*5) ; j++) {
        colorLeds(i, (j+(figuresPassed[i])));
        FastLED.show();
        delay(40);//Might want to adjust this value
    }
    figuresPassed[i]++;
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
    for (int j = (i*5) ; j < 5+(i*5) ; j++) {
        colorLeds(i, j);
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
    if(start<(long)millis()){
        for(int i=0;i<4;i++){
            if(figuresPassed[i]>max){
                max=figuresPassed[i];
            }
        }
        Serial.println(max);
        for(int i=0;i<4;i++){
            if(figuresPassed[i]==max){
              Serial.println(figuresPassed[i]);
                winner[num_of_winners]=i;
                num_of_winners++;
            }
        }
        ledOff();
        phase=2;
    }
}

void prepareNewGame(){
    phase=0;
    enabler=4;
    for(int i=0;i<4;i++){
        val[i]=0;
        state[i]=0;
        figuresPassed[i]=0;
    }
    num_of_winners=0;
    max=0;
    flag=0;
    ledManager();
    delay(1000);
}

void soundSpeaker(int _s){
  for (byte j = 0 ; j < 4 ; j++) {
    tone(BUZZER_PIN1, finalSounds[_s][j]);
  }
  noTone(BUZZER_PIN1);
}