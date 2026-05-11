#include <AFMotor.h>
#include <Servo.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define DHTPIN 10
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial bt(0, 1); // RX, TX
Servo pitServo;
Servo handServo;

// Motors
AF_DCMotor leftWheel(2, MOTOR12_1KHZ);
AF_DCMotor rightWheel(3, MOTOR12_1KHZ);
AF_DCMotor hand1(4, MOTOR12_1KHZ);
AF_DCMotor hand2(5, MOTOR12_1KHZ);

// Sensors
int backIR  = 6;
int pitIR   = 7;

// Heater relay
int heatPin = 8;
int fanPin = 9;

// Servo angles
int PIT_OPEN = 135;
int PIT_CLOSE = 0;

void setup() 
{
  pinMode(backIR, INPUT);
  pinMode(pitIR, INPUT);
  pinMode(heatPin, OUTPUT);
  pinMode(fanPin, OUTPUT);

  pitServo.attach(11);
  pitServo.write(PIT_CLOSE);
  handServo.attach(12);
  handServo.write(PIT_CLOSE);

  leftWheel.setSpeed(90);
  rightWheel.setSpeed(90);
  hand1.setSpeed(100);
  hand2.setSpeed(100);

  dht.begin();
  bt.begin(9600);
  Serial.begin(9600);

  Serial.println("Robot System Ready");
}

void loop() 
{

  // 🔍 OBSTACLE DETECTION
  if (digitalRead(backIR) == HIGH)
  {
    stopWheels();
    Serial.println("Gonna fall");
  }

  // 🗑️ PIT FULL
  if (digitalRead(pitIR) == LOW) 
  {
    stopWheels();
    Serial.println("Pit full");
  }

  // 🌡️ TEMPERATURE CONTROL
  float temp = dht.readTemperature();
  float hum  = dht.readHumidity();

  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("  Humidity: ");
  Serial.println(hum);

  if (temp < 40) 
  {
    digitalWrite(heatPin, HIGH);
    digitalWrite(fanPin, HIGH);
  }
  else 
  {
    digitalWrite(heatPin, LOW);
    digitalWrite(fanPin, LOW);
  }

  // 📱 BLUETOOTH COMMANDS
  if (bt.available()) 
  {
    char cmd = bt.read();

    if (cmd == 'F') moveForward();
    if (cmd == 'B') moveBackward();
    if (cmd == 'L') turnLeft();
    if (cmd == 'R') turnRight();
    if (cmd == 'S') stopWheels();
    if (cmd == 'U') handUp();
    if (cmd == 'T') handDown();
    if (cmd == 'X') limbUp();
    if (cmd == 'H') pitLift();
  }
}

// 🚗 MOVEMENT FUNCTIONS
void moveForward() {
  leftWheel.run(FORWARD);
  rightWheel.run(FORWARD);
}

void moveBackward() {
  leftWheel.run(BACKWARD);
  rightWheel.run(BACKWARD);
}

void turnLeft() {
  leftWheel.run(BACKWARD);
  rightWheel.run(FORWARD);
}

void turnRight() {
  leftWheel.run(FORWARD);
  rightWheel.run(BACKWARD);
}

void stopWheels() {
  leftWheel.run(RELEASE);
  rightWheel.run(RELEASE);
}

// 🦾 HAND COLLECTION
void handUp() {
  hand1.run(FORWARD);
  hand2.run(FORWARD);
}

void handDown(){
  hand1.run(RELEASE);
  hand2.run(RELEASE);
}

void limbUp(){
  handServo.write(PIT_OPEN);
  delay(2000);
  handServo.write(PIT_CLOSE);
}

void pitLift(){
  pitServo.write(PIT_OPEN);
  delay(2000);
  pitServo.write(PIT_CLOSE);
}
