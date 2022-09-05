#include <Servo.h> 
class JoyStick  { 
  private:
    int xPos;
    int yPos;
    int vrx;
    int vry;
    int sw;
    int swState;
    int mapX;
    int mapY;
  public:
    JoyStick(int xPos, int yPos, int vrx, int vry, int sw, int swState, int mapX, int mapY){
      this->xPos = xPos;
      this->yPos = yPos;
      this->vrx = vrx;
      this->vry = vry;
      this->sw = sw;
      this->swState = swState;
      this->mapX = mapX;
      this->mapY = mapY;
      init();
    }

    void init() {          
      pinMode(vrx, INPUT);
      pinMode(vry, INPUT);
      pinMode(sw, INPUT_PULLUP); 
    }
}

JoyStick j1(0,0,A0,A1,2,0,0,0);

class ServoMotor {
  private:
    Servo servo;
    int servoPin;
    int limitPlus;
    int limitMinus;
    int velPlus;
    int velMinus;
  public:
    ServoMotor(Servo servo, int servoPin, int limitPlus, int limitMinus, int velPlus, int velMinus) {
        this->servo = servo;
        this->servoPin = servoPin;
        this->limitPlus = limitPlus;
        this->limitMinus = limitMinus;
        this->velPlus = velPlus;
        this->velMinus = velMinus;
        init();
    }
    void init() {
       servo.attach(servoPin); 
    }
}

Servo Servo1; 
ServoMotor s1(Servo1, 3,138,0,1,5);

void setup() {
  Serial.begin(9600);   
}

void confPins() {
  j1.xPosition = analogRead(VRx);
  j1.yPosition = analogRead(VRy);
  j1.swSstate = digitalRead(SW);
  j1.mapX = map(xPosition, 0, 1023, -512, 512);
  j1.mapY = map(yPosition, 0, 1023, -512, 512);
}

void monitorJoySitck() {
  Serial.print("X: ");
  Serial.print(j1.mapX);
  Serial.print(" | Y: ");
  Serial.print(j1.mapY);
  Serial.print(" | Button: ");
  Serial.println(j1.swState);
}

void controlServo(Servo servo, int mapServo, int limit1, int limit2, int vel1, int vel2) {
  if (mapServo >= 500 and servo.read() < limit1) {
      Servo1.write(servo.read() + vel1);
  } else if (mapServo<= -500 and servo.read() > limit2) {
      Servo1.write(Servo1.read() - vel2);
  } 
}

void controlRobotArm() {
    controlServo(s1.servo, j1.mapY, s1.limitPlus, s1.limitMinus, s1.velPlus, s1.velMinus);
}

void loop() {
  confPins();
  monitorJoySitck();
  Serial.println(s1.servo.read());
  controlRobotArm();
}
