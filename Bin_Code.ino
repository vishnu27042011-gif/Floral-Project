#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD setup (address may be 0x27 or 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo binServo;

// Pin definitions
const int robotIR = 2;
const int humanIR = 3;
const int servoPin = 6;

// Servo angles
const int CLOSED = 0;
const int OPEN = 90;

void setup() {
  pinMode(robotIR, INPUT);
  pinMode(humanIR, INPUT);

  binServo.attach(servoPin);
  binServo.write(CLOSED);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("SMART FLORAL");
  lcd.setCursor(0, 1);
  lcd.print("WASTE BIN");

  delay(2000);
  lcd.clear();
}

void loop() {

  // Read IR sensors
  bool robotDetected = (digitalRead(robotIR) == LOW);
  bool humanDetected = (digitalRead(humanIR) == LOW);

  // ROBOT HAS PRIORITY
  if (robotDetected) {
    openBin();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ROBOT");
    lcd.setCursor(0, 1);
    lcd.print("UNLOADING...");
    delay(3000);
    closeBin();
  }

  // HUMAN DISPOSAL
  else if (humanDetected) {
    openBin();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("THANK YOU");
    lcd.setCursor(0, 1);
    lcd.print("KEEP CLEAN");
    delay(3000);
    closeBin();
  }

  else {
    closeBin();
  }
}

void openBin() {
  binServo.write(OPEN);
}

void closeBin() {
  binServo.write(CLOSED);
}
