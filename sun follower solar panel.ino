/* Get tilt angles on X and Y, and rotation angle on Z
   Angles are given in degrees
   https://github.com/elosine/MPU6050_accel
   License: MIT
*/
/*------------------------ (( Defintion )) ----------------------------*/
// MPU6050
#include "Wire.h"
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
unsigned long timer = 0;
// L298N
int motor1pin1 = 3;
int motor1pin2 = 2;
// LDR
const int LDR_EAST = A2;
const int LDR_WEST = A3;
int val_EAST = 0;
int val_WEST = 0;
int Actuator , Solar_Degree ;

/* --------------------------  (( Setup ))  ------------------------------*/
void setup() {
  Serial.begin(9600);
  Wire.begin();
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while (status != 0) { } // stop everything if could not connect to MPU6050
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");

  pinMode(motor1pin1, OUTPUT); // L298N
  pinMode(motor1pin2, OUTPUT);// L298N

}
// ***************************************************************//
void loop() {
  test();
  if (LDRW() < 5 &&  LDRE() == 5) {  // case 1 East highest analog 1024 sunrise
    while (Actuator < 0) {
      go2west();
      test();
    }
    while (Actuator > 1) {
      go2east();
      test();
    }
    STOP();
  }

  else if (LDRE() < 5 && LDRW() == 5) { // case 2 West highest analog 1024 sunset 
    while (Actuator < 86) {
      go2west();
      test();
    }
    while (Actuator > 90) {
      go2east();
      test();
    }
    STOP();
  }

  else if (LDRE() ==  LDRW() ) { // case 3  West and East are equal the sun is virtical (( south ))
    while (Actuator < 42) {
      go2west();
      test();
    }
    while (Actuator > 45) {
      go2east();
      test();
    }
    STOP();
  }

  else if (LDRE() > LDRW() && LDRE() == 4) { // case 4 the direction SE
    while (Actuator < 22) {
      go2east();
      test();
    }
    while (Actuator > 24) {
      go2west();
      test();
    }
    STOP();
  }
  else if (LDRW() > LDRE() && LDRW() == 4) { // case 5 the direction SW
    while (Actuator < 62) {
      go2west();
      test();
    }
    while (Actuator > 67) {
      go2east();
      test();
    }
    STOP();
  }
}

//************************************ vonction *********************
int LDRE  () {
  val_EAST = analogRead(LDR_EAST);
  val_EAST = map(val_EAST, 0, 700, 1, 6); // mapping val_EAST
  Serial.print("val_EAST = ");
  Serial.println(val_EAST);
  //delay(100);
  return val_EAST;
}
int LDRW () {
  val_WEST = analogRead(LDR_WEST);
  val_WEST = map(val_WEST, 0, 1024, 1, 7); // mapping val_WEST
  Serial.print("val_WEST = ");
  Serial.println(val_WEST);
  //delay(100);
  return val_WEST;
}

void STOP () {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
}

void go2west () {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);
}

void go2east () {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);
}

void test () {
  mpu.update();
  if ((millis() - timer) > 10) { // print data every 10ms
    //  Serial.print("X : ");
    //Serial.print(mpu.getAngleX());
    Serial.print("\tSolar Degree : ");
    Serial.print(mpu.getAngleY());
    Serial.print("\tActuator : ");
    Serial.println(mpu.getAngleZ());
    Actuator = mpu.getAngleZ();
    Solar_Degree = mpu.getAngleY();
    timer = millis();

  }
}
