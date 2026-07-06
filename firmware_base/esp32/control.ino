#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <TM1637Display.h>


#define LIMIT_SWITCH 4

#define CLK 40
#define DIO 41
TM1637Display display(CLK, DIO);

Adafruit_PWMServoDriver pca = Adafruit_PWMServoDriver(0x40);

const int SERVO_FREQ = 50;

int SCORE = 0;

const int AXON_STANDING = 500;
const int AXON_JUMPING = 102;
const int AXON_SQUATTING = 602;
const int CONTINU = 1;

String state = "Standing";

void setup() {
  pinMode(LIMIT_SWITCH, INPUT_PULLUP);

  Serial.begin(115200);
  while (!Serial) {
    delay(10); 
  }
  Serial.println("\nESP32 Serial Initialized.");

  pca.begin();
  pca.setPWMFreq(SERVO_FREQ);
  delay(10);

  display.setBrightness(0x0f);

}

int stateTime;
double GRAVITY = 0.0044;
double VELOCITY = 1.43;
bool hit = false;

void loop() {
    // for (int i = 0; i < 10000; i++) {
  display.showNumberDec(SCORE, false); // (number, leading zeros)
    //   delay(100); // Wait 100 milliseconds between counts
    // }
  Serial.println(digitalRead(LIMIT_SWITCH));
  
  if (digitalRead(LIMIT_SWITCH)==CONTINU) {
    hit = false;
    String inputState;
    if (Serial.available() > 0) {
      String incomingMessage = Serial.readStringUntil('\n');
      incomingMessage.trim(); 
        
      Serial.print("Echo: ");
      Serial.println(incomingMessage);
      inputState = incomingMessage;
    }
    int curTime = millis();

    // pca.setPWM(4, 0, AXON_MIN); 
    // delay(2000);

    // pca.setPWM(4, 0, AXON_MAX); 
    // delay(2000);

    Serial.println(state);
    if (state == "Standing") {
      pca.setPWM(4,0,AXON_STANDING);
      // delay(100);
      if (inputState == "Jumping" || inputState == "Squatting") {
        state = inputState;
        stateTime = curTime;
      }
    }
    else if (state == "Jumping") {
      int jumpPos = (int) (AXON_STANDING + 0.5 * GRAVITY * (curTime - stateTime) * (curTime - stateTime) + -VELOCITY * (curTime - stateTime));
      if (jumpPos < AXON_JUMPING) {
        jumpPos = AXON_JUMPING;
      }
      if (jumpPos > AXON_STANDING) {
        jumpPos = AXON_STANDING;
        state = "Standing";
      }
      pca.setPWM(4,0,jumpPos);
      Serial.println(jumpPos);
      // delay(50);
    }
    else if (state == "Squatting") {
      pca.setPWM(4,0,AXON_SQUATTING);
      if (inputState == "Jumping" || inputState == "Standing") {
        state = inputState;
        stateTime = curTime;
      }
      // delay(100);
    }
  }
  else {
    Serial.println("Hit");
    if (!hit) {
      SCORE++;
      hit = true;
    }

  }


}