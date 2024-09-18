#include <Wire.h>
#include <rgb_lcd.h>
#define NOTE_C4 262
#define NOTE_G3 196
#define NOTE_A3 220
#define NOTE_B3 247

rgb_lcd lcd;

const int button1 = A0;     // Button 1
const int button2 = A1;     // Button 2
const int touchSensor1 = 4; // Touch Sensor 1
const int touchSensor2 = 2; // Touch Sensor 2
const int touchSensor3 = 6; // Touch Sensor 3
const int touchSensor4 = 5; // Touch Sensor 4
const int Noise1 = 8;       // Noise for Winning


// States
int buttonState1 = HIGH;
int lastButtonState1 = HIGH;
int buttonState2 = HIGH;
int lastButtonState2 = HIGH;
int touchSensor1State = 0;
int touchSensor2State = 0;
int touchSensor3State = 0;
int touchSensor4State = 0;
int totalTouches = 0;
int WinningMelody[] = {
   NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

int WinningNoteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
// Messages
String tryMessage = "Try first, ";
String tryMessage2 = "then get hints!";
String hint1 = "Hint #1";
String hint2 = "Hint #2";
String hint3 = "Hint #3 Call IDs";
String hint1Row2 = "Volumes together";
String hint2Row2 = "Notice Call ID?";
String hint3Row2 = "odd vs even...";

String currentText = tryMessage;

void setup() {
  Serial.begin(9600);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(touchSensor1, INPUT);
  pinMode(touchSensor2, INPUT);
  pinMode(touchSensor3, INPUT);
  pinMode(touchSensor4, INPUT);
  lcd.begin(16, 2);
  lcd.setRGB(255, 125, 100);
  lcd.setCursor(0, 0);
  lcd.print(currentText);
  lcd.setCursor(0, 1);
  lcd.print(tryMessage2);
  pinMode(Noise1, OUTPUT);
}

void loop() {
  buttonState1 = analogRead(button1);
  buttonState2 = analogRead(button2);
  touchSensor1State = digitalRead(touchSensor1);
  touchSensor2State = digitalRead(touchSensor2);
  touchSensor3State = digitalRead(touchSensor3);
  touchSensor4State = digitalRead(touchSensor4);

  if (buttonState1 < 10 && lastButtonState1 > 100) {
    ButtonPress(1);
  }

  if (buttonState2 < 10 && lastButtonState2 > 100) {
    ButtonPress(2);
  }

  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;

  delay(50); // Add a small delay to debounce the buttons and touch sensors
}

void ButtonPress(int buttonNumber) {
  lcd.clear();
  lcd.setCursor(0, 0);

  switch (buttonNumber) {
    case 1:
      // Code for handling button 1 press
      if (currentText == tryMessage ) {
        currentText = hint1;
        lcd.print(currentText);
        lcd.setCursor(0, 1);
        lcd.print(hint1Row2);
      } else if (currentText == hint1) {
        currentText = hint2;
        lcd.print(currentText);
        lcd.setCursor(0, 1);
        lcd.print(hint2Row2);
      } else if (currentText == hint2) {
        currentText = hint3;
        lcd.print(currentText);
        lcd.setCursor(0, 1);
        lcd.print(hint3Row2);
      } else {
        currentText = tryMessage ;
        lcd.print(currentText);
        lcd.setCursor(0, 1);
        lcd.print(tryMessage2);
      }
      break;

     case 2:
      // Code for handling button 2 press
      totalTouches = touchSensor1State + touchSensor2State + touchSensor3State + touchSensor4State;
      if (totalTouches == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Correct books: 0");
        lcd.setCursor(0, 1);
        lcd.print("Use the hints!");      
      } else if (totalTouches == 1) {
        lcd.setCursor(0, 0);
        lcd.print("Correct books: 1");
        lcd.setCursor(0, 1);
        lcd.print("Use the hints!");
      } else if (totalTouches == 2) {
        lcd.setCursor(0, 0);
        lcd.print("Correct books: 2");
        lcd.setCursor(0, 1);
        lcd.print("Keep using hints");
      } else if (totalTouches == 3) {
        lcd.setCursor(0, 0);
        lcd.print("Correct books: 3");
        lcd.setCursor(0, 1);
        lcd.print("so close!");
      } else if (totalTouches == 4) {
        lcd.setCursor(0, 0);
        lcd.print("Correct books: 4");
        lcd.setCursor(0, 1);
        lcd.print("Congrats!!");
        playWinningSound();
      }

      break;
  }
}


void playWinningSound() {
  // Play the short winning melody on the buzzer
  for (int thisNote = 0; thisNote < 8; thisNote++) { // Changed to play only 2 notes
    int noteDuration = 1000 / WinningNoteDurations[thisNote];
    tone(Noise1, WinningMelody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(Noise1);
  }
}