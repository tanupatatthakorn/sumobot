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


void setup() {
  // put your setup code here, to run once:
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
  

}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(SW)==LOW){
      test();

      STOP();
      while(1);
    }
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
