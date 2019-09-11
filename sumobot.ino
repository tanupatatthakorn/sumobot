#define LED 13
//switch
#define SW 2
//motors
#define LINA 12
#define LINB 11
#define LPWM 10
#define RINA 4
#define RINB 5
#define RPWM 3
//line sensors
#define LIR A5
#define RIR A4
//detection sensors
#define NUM_SENS 3
#define DIR0 A2
#define DIR1 A3
#define DIR2 A1
//#define DIR3 A3
//#define DIR4 A3


//RC
#define CH1 9
#define CH2 8
#define CH3 7
#define CH4 6

//const values
#define MAX_POWER 255
#define MIN_POWER 0
#define IR_THRESH 400
#define DIR_THRESH 200
//must be tuned later
#define FULLTURN 355 //ms


int tmpCH1;
int tmpCH2;
int tmpCH3;
int tmpCH4;
boolean off = true;

struct sensVal{
  int detect_sensors[NUM_SENS];
  int line_sensors[2];
}sensors;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  //switches
  pinMode(SW, INPUT);
  //motors
  pinMode(LINA, OUTPUT);
  pinMode(LINB, OUTPUT);
  pinMode(LPWM, OUTPUT);
  pinMode(RINA, OUTPUT);
  pinMode(RINB, OUTPUT);
  pinMode(RPWM, OUTPUT);
  //line sensors
  pinMode(LIR, INPUT);
  pinMode(RIR, INPUT);
  //detection sensors
  pinMode(DIR0, INPUT);
  pinMode(DIR1, INPUT);
  pinMode(DIR2, INPUT);
 
  //RC
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  readSensors();
  debug();

  if(digitalRead(SW)==LOW){
    off = false;
    delay(300);
  }
  Controller();
  if(!off){
    digitalWrite(LED,HIGH);
    }else{
      digitalWrite(LED,LOW);
    }
  
  while(!off){
    forward(150,150);
    delay(500);
    pSTOP();
    delay(500);
    forward(150,150);
    delay(500);
    fSTOP();
    delay(500);
      
    //Search();
    //linedetect(sensors.line_sensors[0],sensors.line_sensors[1]);
 
    if(digitalRead(SW)==LOW){
      delay(300);
      off = true;
      fSTOP();
    }
  }
  

}

void debug(){
//    Serial.print("Channel Cooedinates: ");
//    Serial.print(tmpCH1);
//    Serial.print("\t");
//    Serial.print(tmpCH2);
//    Serial.print("\t");
//    Serial.print(tmpCH3);
//    Serial.print("\t");
//    Serial.print(tmpCH4);
//    Serial.print("\n");
    
//    Serial.print("Line sensors: Left = ");
//    Serial.print(analogRead(LIR));
//    Serial.print(" Right = ");
//    Serial.print(analogRead(RIR));
//    Serial.print("\n");

      Serial.println(sensors.detect_sensors[0]);
      //Serial.println(sensors.detect_sensors[1]);
      //Serial.println(sensors.detect_sensors[2]);
}

void test(){
      //forward(100, 100);
      //backward(100, 100);
//      flspin(100);
//      delay(2000);
//      frspin(100);
//      delay(2000);
//      lspin(100);
//      delay(2000);
//      rspin(100);
//      delay(2000);
    blspin(255);
    delay(600);
}

void forward(int lspd, int rspd){
  digitalWrite(LINA, LOW);
  digitalWrite(LINB, HIGH);
  digitalWrite(RINA, LOW);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, lspd);
  analogWrite(RPWM, rspd);
  
}

void backward(int lspd, int rspd){
  digitalWrite(LINA, HIGH);
  digitalWrite(LINB, LOW);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, LOW);

  analogWrite(LPWM, lspd);
  analogWrite(RPWM, rspd);
  
}

void fSTOP(){
  digitalWrite(LINA, HIGH);  
  digitalWrite(LINB, HIGH);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, 0);
  analogWrite(LPWM, 0);
}

void pSTOP(){
  digitalWrite(LINA, LOW);  
  digitalWrite(LINB, LOW);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, 0);
  analogWrite(LPWM, 0);
}

void flspin(int spd){

  forward(0, spd);

}

void frspin(int spd){

  forward(spd, 0);
  
}

void blspin(int spd){

  backward(0, spd);
  
}

void brspin(int spd){

  backward(spd, 0);
  
}

void lspin(int spd){
  //left back
  digitalWrite(LINA, HIGH);
  digitalWrite(LINB, LOW);
  //right for
  digitalWrite(RINA, LOW);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, spd);
  analogWrite(RPWM, spd);
  
}

void rspin(int spd){
  //left for
  digitalWrite(LINA, LOW);
  digitalWrite(LINB, HIGH);
  //right back
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, LOW);

  analogWrite(LPWM, spd);
  analogWrite(RPWM, spd);
  
}

void readSensors(){
  sensors.detect_sensors[0] = analogRead(DIR0);
  sensors.detect_sensors[1] = analogRead(DIR1);
  sensors.detect_sensors[2] = analogRead(DIR2);

  sensors.line_sensors[0] = analogRead(LIR);
  sensors.line_sensors[1] = analogRead(RIR);
}

void linedetect(int LIR, int RIR){

  if(LIR>IR_THRESH){
    backward(255, 255);
    delay(100);
    
    lspin(255);
    delay(FULLTURN);

  }
  
  if(RIR>IR_THRESH){
    backward(255, 255);
    delay(100);
    
    rspin(255);
    delay(FULLTURN);
    
  }

  
}

void Search(){

  if(sensors.detect_sensors[2]>DIR_THRESH){
    rspin(100);
  }
  else if(sensors.detect_sensors[1]>DIR_THRESH){
    forward(100,100);
  }
  else if(sensors.detect_sensors[1]>DIR_THRESH && sensors.detect_sensors[2]>DIR_THRESH){
    frspin(100);
  }
  else if(sensors.detect_sensors[0]>DIR_THRESH){
    flspin(100); 
  }
  else if(sensors.detect_sensors[0]>DIR_THRESH && sensors.detect_sensors[2]>DIR_THRESH){
    pSTOP();
  }
  else if(sensors.detect_sensors[0]>DIR_THRESH && sensors.detect_sensors[1]>DIR_THRESH){
    flspin(100);
  }
  else if(sensors.detect_sensors[0]>DIR_THRESH && sensors.detect_sensors[1]>DIR_THRESH && sensors.detect_sensors[2]>DIR_THRESH){
    forward(100,100);
  }
  else{
    fSTOP();
  }
}

void Controller(){

  tmpCH1 = pulseIn(CH1, HIGH);
  tmpCH2 = pulseIn(CH2, HIGH);
  tmpCH3 = pulseIn(CH3, HIGH);
  tmpCH4 = pulseIn(CH4, HIGH);
  
  //for/back
  int RU = map(tmpCH2, 1600, 1981, 0, 255);
  int RD = map(tmpCH2, 1400, 995, 0, 255);
  if(tmpCH2>1400 && tmpCH2<1600){
    RU= 0;
    RD=0;
  }
  if(tmpCH2==0){
    RU = 0;
    RD = 0;
  }
  //left/right
  int LR = map(tmpCH4, 1550, 1981, 0, 255);
  int LL = map(tmpCH4, 1450, 995, 0, 255);
  if(tmpCH4>1400 && tmpCH4<1600){
    LR = 0;
    LL = 0;
  }
  
  (RU<0)?RU=0:RU=RU;
  (RD<0)?RD=0:RD=RD;

  (LR<0)?LR=0:LR=LR;
  (LL<0)?LL=0:LL=LL;

  if(LR>0 && RU>0){
    frspin(LR);
  }else if(LL>0 && RU>0){
    flspin(LL);
  }else if(RU>0){
    forward(RU, RU);
  }else if(RD>0 && LR>0){
    brspin(LR);
  }else if(RD>0 && LL>0){
    blspin(LL);
  }else{
    backward(RD, RD);
  }

}
  
