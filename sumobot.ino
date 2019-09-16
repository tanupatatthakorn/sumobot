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
#define DIR3 9
#define DIR4 7


//RC
//#define CH1 9
#define CH2 8
//#define CH3 7
#define CH4 6

//const values
#define MAX_POWER 255
#define MIN_POWER 0
#define IR_THRESH 400
#define DIR_THRESH 350
//must be tuned later
#define FULLTURN 355 //ms

//MODES
//default
#define SEARCH 0
#define HUNT 1
#define ATTK 2
#define SURVIVE 3


int tmpCH1;
int tmpCH2;
int tmpCH3;
int tmpCH4;
int mode; 
int state;

struct sensVal {
  int detect_sensors[NUM_SENS];
  int line_sensors[2];
} sensors;

struct flagsVal {
  boolean leftEdge;
  boolean rightEdge;
  boolean Wflag;
  boolean NWflag;
  boolean Nflag;
  boolean NEflag;
  boolean Eflag;
} flags;

//struct flagsVal{
//  unsigned char leftEdge : 1,
//                rightEdge : 1,
//                Wflag : 1,
//                NWflag : 1,
//                Nflag : 1,
//                NEflag : 1,
//                Eflag : 1,
//                unused : 1;
//}flags;

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
  //pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  //pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);

  mode = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  //readSensors();
  debug();
  //Controller();
  fSTOP();

  if (digitalRead(SW) == LOW ) {
    mode = 1;
    delay(300);
  }

  if(digitalRead(SW) == LOW && (analogRead(DIR0)>DIR_THRESH)){
    mode = 2;
    delay(300);
  }

  if (mode > 0) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }

  state = SEARCH;
//---------------MODE 1----------------------
  //mode 1
  while (mode == 1) {
    debug();

    switch (state) {
      case SEARCH:
        forward(MAX_POWER*0.5, MAX_POWER*0.5);
        if(sensors.detect_sensors[0] > DIR_THRESH || sensors.detect_sensors[1] > DIR_THRESH || sensors.detect_sensors[2] > DIR_THRESH || sensors.detect_sensors[3] > DIR_THRESH || sensors.detect_sensors[4] > DIR_THRESH){
          state = HUNT;
        }else if (sensors.line_sensors[0] > IR_THRESH || sensors.line_sensors[1] > IR_THRESH) {
          state = SURVIVE;
        }else {
          state = SEARCH;
        }
        break;
      
      case HUNT:
        Search();
        if (sensors.line_sensors[0] > IR_THRESH || sensors.line_sensors[1] > IR_THRESH) {
          state = SURVIVE;
        }
        break;

      case SURVIVE:
        if (sensors.line_sensors[0] > IR_THRESH) {
          backward(255, 255);
          delay(150);

          rspin(255);
          delay(FULLTURN * 5 / 6);
          frspin(255);
          delay(250);
        }else if (sensors.line_sensors[1] > IR_THRESH) {
          backward(255, 255);
          delay(100);

          lspin(255);
          delay(FULLTURN * 5 / 6);
          flspin(255);
          delay(250);
        }else {
          state = SEARCH;
        }
    }

    if (digitalRead(SW) == LOW) {
      delay(300);
      mode = 0;
    }
  }
//------------------END OF MODE 1-----------------------

//----------------MODE 2------------------------
  //mode 2
  while(mode == 2){
    test();
   
    mode = 0;
    if (digitalRead(SW) == LOW){
      delay(300);
      mode = 0;
    }
  }
//-------------END OF MODE 2---------------------


}

void debug() {

  readSensors();
  
//      Serial.print("Channel Cooedinates: ");
//      Serial.print(tmpCH2);
//      Serial.print("\t");
//      Serial.print(tmpCH4);
//      Serial.print("\n");

  //    Serial.print("Line sensors: Left = ");
  //    Serial.print(analogRead(LIR));
  //    Serial.print(" Right = ");
  //    Serial.print(analogRead(RIR));
  //    Serial.print("\n");

        Serial.print(sensors.detect_sensors[0]);
        Serial.print("\t");
        Serial.print(sensors.detect_sensors[1]);
        Serial.print("\t");
        Serial.print(sensors.detect_sensors[2]);
        Serial.print("\n");

  //----------------read flag--------------------
//  Serial.print(flags.leftEdge);
//  Serial.print("\t");
//  Serial.print(flags.rightEdge);
//  Serial.print("\t");
//  Serial.print(flags.Wflag);
//  Serial.print("\t");
//  Serial.print(flags.NWflag);
//  Serial.print("\t");
//  Serial.print(flags.Nflag);
//  Serial.print("\t");
//  Serial.print(flags.NEflag);
//  Serial.print("\t");
//  Serial.print(flags.Eflag);
//  Serial.print("\n");

//    Serial.print("MODE: ");
//    Serial.print(mode);
//    Serial.println(state);
    
}

void test() {
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
//  blspin(255);
//  delay(600);

  //forward(200, 200);
  //delay(350);
  flspin(255);
  delay(FULLTURN*1/4);
  larc(255, MAX_POWER*0.2);
  delay(750);
  pSTOP();
}

void forward(int lspd, int rspd) {
  digitalWrite(LINA, LOW);
  digitalWrite(LINB, HIGH);
  digitalWrite(RINA, LOW);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, lspd);
  analogWrite(RPWM, rspd);

  //Serial.println("forward called");

}

void backward(int lspd, int rspd) {
  digitalWrite(LINA, HIGH);
  digitalWrite(LINB, LOW);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, LOW);

  analogWrite(LPWM, lspd);
  analogWrite(RPWM, rspd);

  //Serial.println("backward called");
}

void fSTOP() {
  digitalWrite(LINA, HIGH);
  digitalWrite(LINB, HIGH);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, 0);
  analogWrite(LPWM, 0);

  //Serial.println("fSTOP called");
}

void pSTOP() {
  digitalWrite(LINA, LOW);
  digitalWrite(LINB, LOW);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, 0);
  analogWrite(LPWM, 0);

  //Serial.println("pSTOP called");
}

void flspin(int spd) {

  forward(0, spd);

  //Serial.println("flspin called");

}

void frspin(int spd) {

  forward(spd, 0);

  //Serial.println("frspin called");

}

void blspin(int spd) {

  backward(0, spd);

  //Serial.println("blspin called");

}

void brspin(int spd) {

  backward(spd, 0);

  //Serial.println("brspin called");

}

void larc(int spd, int factor){

  forward(spd-factor, spd);
}

void rarc(int spd, int factor){

  forward(spd, spd-factor);
}

void lspin(int spd) {
  //left back
  digitalWrite(LINA, HIGH);
  digitalWrite(LINB, LOW);
  //right for
  digitalWrite(RINA, LOW);
  digitalWrite(RINB, HIGH);

  analogWrite(LPWM, spd);
  analogWrite(RPWM, spd);

  //Serial.println("lspin called");

}

void rspin(int spd) {
  //left for
  digitalWrite(LINA, LOW);
  digitalWrite(LINB, HIGH);
  //right back
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, LOW);

  analogWrite(LPWM, spd);
  analogWrite(RPWM, spd);

  //Serial.println("rspin called");

}

void readSensors() {
  sensors.detect_sensors[0] = analogRead(DIR0);
  sensors.detect_sensors[1] = analogRead(DIR1);
  sensors.detect_sensors[2] = analogRead(DIR2);

  sensors.line_sensors[0] = analogRead(LIR);
  sensors.line_sensors[1] = analogRead(RIR);
}

void clearflags() {
  flags.leftEdge = false;
  flags.rightEdge = false;
  flags.Wflag = false;
  flags.NWflag = false;
  flags.Nflag = false;
  flags.NEflag = false;
  flags.Eflag = false;
}

void linedetect(int LIR, int RIR) {

  if (LIR > IR_THRESH) {
    backward(255, 255);
    delay(150);

    lspin(255);
    delay(FULLTURN * 3 / 4);

  }

  if (RIR > IR_THRESH) {
    backward(255, 255);
    delay(100);

    rspin(255);
    delay(FULLTURN * 3 / 4);

  }


}

void Search() {

  readSensors();

  if (sensors.detect_sensors[0] < DIR_THRESH && sensors.detect_sensors[1] < DIR_THRESH && sensors.detect_sensors[2] > DIR_THRESH) { //001
    rspin(255);
    flags.NEflag = true;
  }
  else if (sensors.detect_sensors[0] < DIR_THRESH && sensors.detect_sensors[1] > DIR_THRESH && sensors.detect_sensors[2] < DIR_THRESH) { //010
    forward(255, 255);
    flags.Nflag = true;
  }
  else if (sensors.detect_sensors[0] < DIR_THRESH && sensors.detect_sensors[1] > DIR_THRESH && sensors.detect_sensors[2] > DIR_THRESH) { //011
    frspin(255);
    flags.NEflag = true;
  }
  else if (sensors.detect_sensors[0] > DIR_THRESH && sensors.detect_sensors[1] < DIR_THRESH && sensors.detect_sensors[2] < DIR_THRESH) { //100
    lspin(255);
    flags.NWflag = true;
  }
  else if (sensors.detect_sensors[0] > DIR_THRESH && sensors.detect_sensors[1] < DIR_THRESH && sensors.detect_sensors[2] > DIR_THRESH) { //101
    pSTOP();
  }
  else if (sensors.detect_sensors[0] > DIR_THRESH && sensors.detect_sensors[1] > DIR_THRESH && sensors.detect_sensors[2] < DIR_THRESH) { //110
    flspin(255);
    flags.NWflag = true;
  }
  else if (sensors.detect_sensors[0] > DIR_THRESH && sensors.detect_sensors[1] > DIR_THRESH && sensors.detect_sensors[2] > DIR_THRESH) { //111
    forward(255, 255);
    flags.Nflag = true;
    flags.NEflag = true;
    flags.NWflag = true;
  }
  else {
    fSTOP();
    clearflags();
  }

}

void Controller() {

  //tmpCH1 = pulseIn(CH1, HIGH);
  tmpCH2 = pulseIn(CH2, HIGH);
  //tmpCH3 = pulseIn(CH3, HIGH);
  tmpCH4 = pulseIn(CH4, HIGH);

  //for/back
  int RU = map(tmpCH2, 1600, 1974, 0, 255);
  int RD = map(tmpCH2, 1400, 993, 0, 255);
  if (tmpCH2 > 1400 && tmpCH2 < 1600) {
    RU = 0;
    RD = 0;
  }else if(tmpCH2 > 1974){
    RU = 255;
  }else if(tmpCH2 < 993){
    RD = 255;
  }
  if (tmpCH2 == 0) {
    RU = 0;
    RD = 0;
  }
  //left/right
  int LR = map(tmpCH4, 1550, 1974, 0, 255);
  int LL = map(tmpCH4, 1450, 993, 0, 255);
  if (tmpCH4 > 1400 && tmpCH4 < 1600) {
    LR = 0;
    LL = 0;
  }else if(tmpCH4 > 1974){
    LR = 255;
  }else if(tmpCH4 < 993){
    LL = 255;
  }

  (RU < 0) ? RU = 0 : RU = RU;
  (RD < 0) ? RD = 0 : RD = RD;

  (LR < 0) ? LR = 0 : LR = LR;
  (LL < 0) ? LL = 0 : LL = LL;

  if (LR > 0 && RU > 0) {
    frspin(LR);
  } else if (LL > 0 && RU > 0) {
    flspin(LL);
  } else if (RU > 0) {
    forward(RU, RU);
  } else if (RD > 0 && LR > 0) {
    brspin(LR);
  } else if (RD > 0 && LL > 0) {
    blspin(LL);
  } else {
    backward(RD, RD);
  }

}
