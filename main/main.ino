#include <arduino-timer.h>
auto timer = timer_create_default(); // create a timer with default settings

//this script already contains the correct pin mapping for the pcb
#define DATA_PIN 12
#define OUTPUT_ENABLE_PIN 9
#define LATCH_CLOCK_PIN 11
#define SHIFT_CLOCK_PIN 10
#define RESET_PIN 8
#define BRIGHTNESS_KEY_PIN 2
#define HOUR_KEY_PIN 3
#define MINUTES_KEY_PIN 4
#define SECONDS_KEY_PIN 5
#define DS_PIN A3
#define SQ_PIN A2
byte numbers[] = {125, 24, 110, 62, 27, 55, 119, 28, 127, 63};

int counter;
float brightness, goalBrightness;
#define BRIGHTNESS_STEPS 10 //amount of steps from 0 to 100%

#define DEBOUNCE_DELAY 25  //the delay that the buttons are again read after sensing a pushed state to check for bouncing


// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS1307 rtc;

void setup() {
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, HIGH);

  pinMode(DS_PIN, OUTPUT);
  digitalWrite(DS_PIN, LOW);
  pinMode(SQ_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("HELLO WORLD");
  // put your setup code here, to run once:
  pinMode(DATA_PIN, OUTPUT);
  setBrightness(255.0);
  pinMode(LATCH_CLOCK_PIN, OUTPUT);
  pinMode(SHIFT_CLOCK_PIN, OUTPUT);
  digitalWrite(RESET_PIN, HIGH);
  Serial.println("HELLO WORLD");
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  Serial.println("HELLO WORLD");
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  Serial.println("HELLO WORLD");
  pinMode(BRIGHTNESS_KEY_PIN, INPUT_PULLUP); //brightness key
  pinMode(HOUR_KEY_PIN, INPUT_PULLUP); //hour key
  pinMode(MINUTES_KEY_PIN , INPUT_PULLUP); //minute key
  pinMode(SECONDS_KEY_PIN , INPUT_PULLUP); //seconds key
  timer.every(100, run100ms, 0);
  sendSingleByte(24);
  sendSingleByte(91);
  refreshScreen();
  delay(100);
}
void loop() {
  // put your main code here, to run repeatedly:
  /*sendSingleByte(B00000001);
    sendSingleByte(B00000010);
    sendSingleByte(B00000100);
    sendSingleByte(B00001000);
    sendSingleByte(B00010000);
    sendSingleByte(B00100000);
    sendSingleByte(B01000000);
    sendSingleByte(B00000000);

    sendNumber(0);
    sendNumber(1);
    sendNumber(2);
    sendNumber(3);
    sendNumber(4);
    sendNumber(5);
    sendNumber(6);
    sendNumber(7);
    sendNumber(8);
    sendNumber(9);

    sendSingleByte(0);
    sendSingleByte(125);
    sendSingleByte(24);
    sendSingleByte(110);
    sendSingleByte(62);
    sendSingleByte(27);
    sendSingleByte(55);
    sendSingleByte(119);
    sendSingleByte(28);
    sendSingleByte(127);
    sendSingleByte(63);*/
  checkButtons();
  timer.tick();
}
void run100ms() {
  //counter++;
  //if (counter == 100) counter = 0;
  //send2DigitNumber(counter);
  showTime();
  printTime();
}

void checkButtons() {
  if (digitalRead(BRIGHTNESS_KEY_PIN) == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(BRIGHTNESS_KEY_PIN) == LOW) {
      //if the button has been read without bouncing do this
      changeBrightness();
      //while the button is being held do this
      while (digitalRead(BRIGHTNESS_KEY_PIN) == LOW) delay(10);
      //after its released do this
    }
  }
  if (digitalRead(HOUR_KEY_PIN) == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(HOUR_KEY_PIN) == LOW) {
      //if the button has been read without bouncing do this
      addHour();
      showTime();
      //while the button is being held do this
      while (digitalRead(HOUR_KEY_PIN) == LOW) delay(10);
      //after its released do this
    }
  }
  if (digitalRead(MINUTES_KEY_PIN) == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(MINUTES_KEY_PIN) == LOW) {
      //if the button has been read without bouncing do this
      addMinute();
      showTime();
      //while the button is being held do this
      while (digitalRead(MINUTES_KEY_PIN) == LOW) delay(10);
      //after its released do this
    }
  }
  if (digitalRead(SECONDS_KEY_PIN) == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(SECONDS_KEY_PIN) == LOW) {
      //if the button has been read without bouncing do this
      addSecond();
      showTime();
      //while the button is being held do this
      
      //after its released do this
    }
  }
}
void addHour() {
  DateTime now = rtc.now();
  int newYearValue = now.year();
  int newMonthValue = now.month();
  int newDayValue = now.day();
  int newHourValue = now.hour() + 1;
  if (newHourValue > 23) newHourValue = 0;
  int newMinuteValue = now.minute();
  int newSecondValue = now.second();
  rtc.adjust(DateTime(newYearValue, newMonthValue, newDayValue, newHourValue, newMinuteValue, newSecondValue));
}
void addMinute() {
  DateTime now = rtc.now();
  int newYearValue = now.year();
  int newMonthValue = now.month();
  int newDayValue = now.day();
  int newHourValue = now.hour();
  int newMinuteValue = now.minute() + 1;
  if (newMinuteValue > 59) newMinuteValue = 0;
  int newSecondValue = now.second();
  rtc.adjust(DateTime(newYearValue, newMonthValue, newDayValue, newHourValue, newMinuteValue, newSecondValue));
}
void addSecond() {
  DateTime now = rtc.now();
  int newYearValue = now.year();
  int newMonthValue = now.month();
  int newDayValue = now.day();
  int newHourValue = now.hour();
  int newMinuteValue = now.minute();
  int newSecondValue = now.second() + 1;
  if (newSecondValue > 59) newSecondValue = 0;
  while (digitalRead(SECONDS_KEY_PIN) == LOW) delay(10);
  rtc.adjust(DateTime(newYearValue, newMonthValue, newDayValue, newHourValue, newMinuteValue, newSecondValue));
}
void showTime() {
  DateTime now = rtc.now();
  send2DigitNumber(now.second());
  //refreshScreen();
  //delay(4000/3);
  send2DigitNumber(now.minute());
  //refreshScreen();
  //delay(4000/3);
  send2DigitNumber(now.hour());
  refreshScreen();
  //delay(4000/3);
}
void printTime() {
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

}
void changeBrightness() {
  goalBrightness = brightness + (255 / float(BRIGHTNESS_STEPS));
  if (goalBrightness > 255) goalBrightness = 0;
  setBrightness(goalBrightness);

}
void setBrightness(float val) {
  brightness = val;
  analogWrite(OUTPUT_ENABLE_PIN, map(val, 0, 255, 255, 0));
  Serial.println("helligkeit auf: " + String(goalBrightness) + " gesetzt");
}
void sendNumber(int number) {
  sendSingleByte(numbers[number]);
}
void send2DigitNumber(int number) {
  int firstDigit = number / 10;
  int secondDigit = number % 10;
  sendDoubleByte(numbers[secondDigit], numbers[firstDigit]);
}
void sendSingleByte(byte val) {
  shiftDataOut(val);
  delay(100);
}
void sendDoubleByte(byte val1, byte val2) {
  shiftDataOut(val1);
  shiftDataOut(val2);
  delay(100);
}
void refreshScreen() {
  digitalWrite(LATCH_CLOCK_PIN, LOW);
  digitalWrite(LATCH_CLOCK_PIN, HIGH);
}
void shiftDataOut(byte dataOut) {
  // Shift out 8 bits LSB first, clocking each with a rising edge of the clock line
  boolean pinState;
  for (int i = 0; i <= 7; i++) { // for each bit in dataOut send out a bit
    digitalWrite(SHIFT_CLOCK_PIN, LOW); //set SHIFT_CLOCK_PIN to LOW prior to sending bit
    // if the value of DataOut and (logical AND) a bitmask
    // are true, set pinState to 1 (HIGH)
    if ( dataOut & (1 << i) ) {
      pinState = HIGH;
    }
    else {
      pinState = LOW;
    }
    //sets DATA_PIN to HIGH or LOW depending on pinState
    digitalWrite(DATA_PIN, pinState); //send bit out before rising edge of clock
    digitalWrite(SHIFT_CLOCK_PIN, HIGH);
  }
  digitalWrite(SHIFT_CLOCK_PIN, LOW); //stop shifting out data
}
/*
  void sendOut(byte val) {
  sendOut(00000001);
  sendOut(00000010);
  sendOut(00000100);
  sendOut(00001000);
  sendOut(00010000);
  sendOut(00100000);
  sendOut(01000000);
  sendOut(10000000);
  sendOut(11111111);
  shiftOut(DATA_PIN, SHIFT_CLOCK_PIN, MSBFIRST, val);
  digitalWrite(LATCH_CLOCK_PIN, LOW);
  digitalWrite(LATCH_CLOCK_PIN, HIGH);
  delay(500);
  }
*/
