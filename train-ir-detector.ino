#include <Wire.h>
#define DURATION_ON 1000l
#define DURATION_TOGGLE 200l

unsigned long start;
unsigned long t1;
unsigned long t2;
int toogle;
int toogleLast;
int value;
uint8_t state[3] = { 0xFF, 0xFF, 0xFF };
uint8_t workingState;

void setup()
{
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(5, OUTPUT);
  Wire.begin(0x51);
  Wire.onRequest(sendData);
}

void loop() {
  calculateState(0, A1);
  calculateState(1, A2);
  calculateState(2, A3);
}

void calculateState(int index, uint8_t pin) {
  workingState = 0xFF;
  start = micros();
  toogleLast = 0;
  tone(5, 38000);
  do {
    t1 = micros();
    t2 = t1 - start;
    toogle = (t2 / DURATION_TOGGLE) % 2;
    if (toogle == 0 && toogleLast == 1) {
      tone(5, 38000);
    } else if (toogleLast == 0) {
      noTone(5);
      digitalWrite(5, LOW);
    }

    toogleLast = toogle;
    value = analogRead(pin) / 4;
    if (value < workingState) {
      workingState = value;
    }
  } while (t2 < DURATION_ON);

  state[index] = workingState;
  noTone(5);
  digitalWrite(5, LOW);
  delayMicroseconds(1000);
}

void sendData() {
  Wire.write(state, 3);
}
