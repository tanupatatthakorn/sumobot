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

//RC
#define CH1 9
#define CH2 8
#define CH3 7
#define CH4 6

int tmpCH1;
int tmpCH2;
int tmpCH3;
int tmpCH4;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
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

  //RC
  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  tmpCH1 = pulseIn(CH1, HIGH);
  tmpCH2 = pulseIn(CH2, HIGH);
  tmpCH3 = pulseIn(CH3, HIGH);
  tmpCH4 = pulseIn(CH4, HIGH);

  debug();

 Controller(tmpCH1, tmpCH2, tmpCH3, tmpCH4);
  
  if(digitalRead(SW)==LOW){
      //test();
      
      STOP();
      while(1);
    }
  }

void debug(){
    Serial.print(tmpCH1);
    Serial.print("\t");
    Serial.print(tmpCH2);
    Serial.print("\t");
    Serial.print(tmpCH3);
    Serial.print("\t");
    Serial.print(tmpCH4);
    Serial.print("\n");
    
//    Serial.println(digitalRead(CH2));
//    Serial.println(digitalRead(CH3));
//    Serial.println(digitalRead(CH4));
}

void test(){
      //forward(100, 100);
      //backward(100, 100);
//      flspin(100);
//      delay(2000);
//      frspin(100);
//      delay(2000);
      lspin(100);
      delay(2000);
      rspin(100);
      delay(2000);
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

void STOP(){
  digitalWrite(LINA, HIGH);  
  digitalWrite(LINB, HIGH);
  digitalWrite(RINA, HIGH);
  digitalWrite(RINB, HIGH);
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

void Controller(int tmpCH1, int tmpCH2, int tmpC3, int tmpCH4){
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
    blspin(LR);
  }else if(RD>0 && LL>0){
    brspin(LL);
  }else{
    backward(RD, RD);
  }

}
  
