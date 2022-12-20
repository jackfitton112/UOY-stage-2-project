#include "motors.h"
#include "pid.h"

//Date Created: 16/12/2022

// this is a test program for the ESP32 for the sensor to measure the distance to an obstacle and then drive 50mm from it

/////Motor A Pins/////

// Motor A PWM Pin
#define PWMA 13

// Motor A Encoder Pins
#define ENCA1 35
#define ENCA2 34

// Direction Control Pins
#define AIN1 25
#define AIN2 26

//////////////////////

/////Motor B Pins/////

// Motor B PWM Pin
#define PWMB 12

// Motor B Encoder Pins
#define ENCB1 33
#define ENCB2 32

// Direction Control Pins
#define BIN1 2
#define BIN2 4
//////////////////////


//init objects-
Motors motors(PWMA, AIN1, AIN2, ENCA1, ENCA2, PWMB, BIN1, BIN2, ENCB1, ENCB2);
PID PID_A (0,0,0);
PID PID_B (0,0,0);

void IRAM_ATTR encoderDataA()
{

    int ENCA2_DATA = digitalRead(ENCA2);

    if (ENCA2_DATA == HIGH){
        motors.encoderCountA++;
    }
    else if (ENCA2_DATA == LOW){
        motors.encoderCountA--;
    }

}

void IRAM_ATTR encoderDataB(){
    int ENCB2_DATA = digitalRead(ENCB2);

    if (ENCB2_DATA == HIGH){
        motors.encoderCountB++;
    }
    else if (ENCB2_DATA == LOW){
        motors.encoderCountB--;
    }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

    attachInterrupt(ENCA1, encoderDataA, RISING);
    attachInterrupt(ENCB1, encoderDataB, RISING);

}

void loop() {

    int val = random(1000);
    PID_A.setTarget(val);
    PID_B.setTarget(val);

    Serial.println(val);
    Serial.print("A");
    Serial.println(PID_A.update(motors.encoderCountA));
    Serial.println("B");
    Serial.println(PID_B.update(motors.encoderCountB));


    motors.setSpeedA(PID_A.update(motors.encoderCountA));
    motors.setSpeedB(PID_B.update(motors.encoderCountB));

    while (PID_A.update(motors.encoderCountA) != 0 && PID_B.update(motors.encoderCountB) != 0){

        motors.setSpeedA(PID_A.update(motors.encoderCountA));
        motors.setSpeedB(PID_B.update(motors.encoderCountB));
        Serial.println(motors.encoderCountA);
        Serial.println(motors.encoderCountB);
        Serial.println(PID_A.targetTicks);
        Serial.println(PID_B.targetTicks);
    }


    delay(10000);




}

