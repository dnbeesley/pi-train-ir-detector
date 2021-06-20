#include <Wire.h>
#define DURATION_ON 1000l
#define DURATION_TOGGLE 200l

unsigned long start;
unsigned long t1;
unsigned long t2;
int address;
int toogle;
int toogleLast;
int value;
uint8_t state[2] = { 0xFF, 0xFF };
uint8_t workingState;

void setup()
{
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(5, OUTPUT);
  Wire.begin(0x50);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(sendData);
}

void loop() {
  calculateState(0, A1);
  calculateState(1, A2);
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
    if(toogle == 0 && toogleLast == 1) {
      tone(5, 38000);
    } else if(toogleLast == 0) {
      noTone(5);
      digitalWrite(5, LOW);
    }

    toogleLast = toogle;
    value = analogRead(pin) / 4;
    if(value < workingState) {
      workingState = value;
    }
  } while (t2 < DURATION_ON);

  state[index] = workingState;
  noTone(5);
  digitalWrite(5, LOW);
  delayMicroseconds(1000);
}

void receiveEvent(size_t numBytes) {
  while(1 < numBytes--) {
    Wire.read();
  }

  address = Wire.read();
}

void sendData() {
  if (address == 0 || address == 1) {
    Wire.write(&state[address], 1);
  } else {
    Wire.write((uint8_t*)&address, 1);
  }
}
