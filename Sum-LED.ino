/*
  Emerson Leonardo Lucena
  03/03/2016
*/
  
#include <avr/wdt.h>

#define FRST 0
#define SCND 1
#define RSLT 2

const int buttonPin[4] = {2,3,4,5};
const int ledPin[7] = {6,7,8,9,10,11,12};

void soft_reset() {
    Serial.println("Reseting...");

    wdt_enable(WDTO_15MS);
    while(1);
}

int number[7], last[4];
int i;

void turnON(int Pin = 13) { 
  digitalWrite(Pin,HIGH);
}

void turnOFF(int Pin = 13) { 
  digitalWrite(Pin,LOW);
}

void updateLeds(int i) {
  number[i]?turnON(ledPin[i]):turnOFF(ledPin[i]);
}

void waitanybutton() {
  Serial.println("Press any button to continue...");
  int b[4];
  do {
    for(i = 0; i < 4; i++) {
      b[i] = digitalRead(buttonPin[i]);
    }
  } while(!(b[0] || b[1] || b[2] || b[3]));
  Serial.println("Button pressed.");
}

int upd_timer() {
  unsigned long time = millis();

  if(time > 10000) {
    turnOFF(ledPin[6]);
    return 1;
  }
  else if(time > 7500) {
    turnOFF(ledPin[5]);
  }

  else if(time > 5000) {
    turnOFF(ledPin[4]);
  }

  return 0;
}

void firula() { //just visual effects
  for(i = 0; i < 2; i++) {
    turnON(ledPin[4]);
    turnON(ledPin[5]);
    turnON(ledPin[6]);

    delay(500);

    turnOFF(ledPin[4]);
    turnOFF(ledPin[5]);
    turnOFF(ledPin[6]);

    delay(500);
  }

  turnON(ledPin[6]);
  delay(200);
  turnON(ledPin[5]);
  delay(200);
  turnON(ledPin[4]);
  delay(200);

  turnOFF(ledPin[6]);
  delay(200);
  turnOFF(ledPin[5]);
  delay(200);
  turnOFF(ledPin[4]);
  delay(200);

  turnON(ledPin[6]);
  delay(100);
  turnOFF(ledPin[6]);
  turnON(ledPin[5]);
  delay(100);
  turnOFF(ledPin[5]);
  turnON(ledPin[4]);
  delay(100);
  turnOFF(ledPin[4]);

  delay(200);
  turnON(ledPin[4]);
  turnON(ledPin[5]);
  turnON(ledPin[6]);
}

void setup() {
  MCUSR = 0;
  wdt_disable();

  Serial.begin(9600);

  for(i=0;i<7;i++) pinMode(ledPin[i], OUTPUT);
  //for(i=0;i<4;i++) pinMode(buttonPin[i], INPUT); // not necessary - pins are already defined as INPUT 

  firula();
}

void loop() {
  Serial.println("Starting");
  while(!upd_timer()) {
    for(i = 0; i < 4; i++) {

      int aux = last[i];
      int but = digitalRead(buttonPin[i]);
      if(but && !aux) {
        number[i] ^= 1;
        Serial.print(i);
        Serial.println(" has been pressed!");
      }
      last[i] = but;
      updateLeds(i);
    }
  }

  delay(300);

  int ans = 0;
  ans += (number[0] + number[2]) * 2 + number[1] + number[3];

  Serial.print("The result is ");
  Serial.println(ans);

  number[6] = ans%2;
  ans /= 2;
  updateLeds(6);

  number[5] = ans%2;
  ans /= 2;
  updateLeds(5);

  number[4] = ans%2;
  updateLeds(4);

  waitanybutton();
  soft_reset();
}
