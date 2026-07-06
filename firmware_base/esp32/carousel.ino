#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PWMServoDriver.h>
#define LED_PIN     48 
#define NUM_PIXELS   1 

#define SWITCH_PIN 47

Adafruit_NeoPixel rgbLed(NUM_PIXELS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

const int SDA_PIN = 8;
const int SCL_PIN = 9;
const int SERVO_FREQ = 50;
const int STOP_PULSE = 372;
const int MAX_CW = 205;
const int MAX_CCW = 540;
const int SERVOMIN = 150;
const int SERVOMAX = 600;
const int E_MIN = 100;//102
const int E_MAX = 520;//512

const int E_CACTUS = 102;
const int E_BIRD = 512;
const int E_ = 307;

const int AIN1_PIN = 4;
const int AIN2_PIN = 5;
const int SLEEP_PIN = 7;

int state = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); 
  }
  Serial.println("\nESP32 Serial Initialized.");
  Wire.begin(SDA_PIN, SCL_PIN);
  pca.begin();
  pca.setPWMFreq(SERVO_FREQ);
  delay(10);
  rgbLed.begin();
  randomSeed(analogRead(0));

  pinMode(AIN1_PIN, OUTPUT);
  pinMode(AIN2_PIN, OUTPUT);
  pinMode(SLEEP_PIN, OUTPUT);

  digitalWrite(AIN1_PIN, LOW);
  digitalWrite(AIN2_PIN, LOW);

  digitalWrite(SLEEP_PIN, HIGH);

  pinMode(SWITCH_PIN, INPUT_PULLUP);

}

const int DELAY = 500;
int startPos = 0;
int motorSpeed =85;
int pos = -1;
int perTime;
bool pressed;

void loop() {
  int curTime = millis();
  motorF(motorSpeed);
  Serial.println(digitalRead(SWITCH_PIN));
  if (!pressed && (curTime - perTime > 2700 || (digitalRead(SWITCH_PIN) == HIGH && curTime - perTime > 100))) {
    pressed = true;
    perTime = curTime;
  }
  if (pressed) {
    pos = startPos%8;
    int r = random(3);
    if (r == 0) {
      pca.setPWM(pos,0,E_CACTUS);
    }
    else if (r == 1) {
      pca.setPWM(pos,0,E_BIRD);
    }
    else {
      pca.setPWM(pos,0,E_);
    } 
    r = random(3);
    if (r == 0) {
      pca.setPWM((startPos+1)%8,0,E_CACTUS);
    }
    else if (r == 1) {
      pca.setPWM((startPos+1)%8,0,E_BIRD);
    }
    else {
      pca.setPWM((startPos+1)%8,0,E_);
    }
    startPos += 2;
    delay(450);
    pressed = false;
  }
}

void motorF(int speed) {
  analogWrite(AIN2_PIN, 0);
  analogWrite(AIN1_PIN, speed);
}

void motorR(int speed) {
  analogWrite(AIN1_PIN, speed);
  analogWrite(AIN2_PIN, 0);
}

void motorS() {
  analogWrite(AIN2_PIN, 0);
  analogWrite(AIN1_PIN, 0);
}