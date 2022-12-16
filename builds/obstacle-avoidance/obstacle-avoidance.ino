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


//call libraries
#include <Wire.h>
#include <VL53L1X.h>
#include <cmath>
#include "motors.h"
#include "pid.h"

//init objects-
VL53L1X sensor;
//Motor motorA(PWMA, AIN1, AIN2, ENCA1, ENCA2);
//Motor motorB(PWMB, BIN1, BIN2, ENCB1, ENCB2);

Motors motors(PWMA, AIN1, AIN2, ENCA1, ENCA2, PWMB, BIN1, BIN2, ENCB1, ENCB2);
PID PID_A (0.95,0.9,0.5);
PID PID_B (1,0.11,0.52);

void sensorInit(){
    Wire.begin();
    Wire.setClock(400000); // use 400 kHz I2C
    sensor.setTimeout(500);
    if (!sensor.init()){
        Serial.println("Failed to detect and initialize sensor!");
        while (1);
    }

    sensor.setDistanceMode(VL53L1X::Short);
    sensor.setMeasurementTimingBudget(20000);
    sensor.startContinuous(25);
}

/*

void IRAM_ATTR encoderDataA()
{

    int ENCA2_DATA = digitalRead(ENCA2);

    if (ENCA2_DATA == HIGH){
        motorA.encoderCount++;
    }
    else if (ENCA2_DATA == LOW){
        motorA.encoderCount--;
    }

}

void IRAM_ATTR encoderDataB(){
    int ENCB2_DATA = digitalRead(ENCB2);

    if (ENCB2_DATA == HIGH){
        motorB.encoderCount++;
    }
    else if (ENCB2_DATA == LOW){
        motorB.encoderCount--;
    }
}

*/

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

void setup(){

    //start serial communication
    Serial.begin(115200);

    //init sensor
    sensorInit();

    //gives time to put the robot on the ground before it starts calibrating
    delay(1000);

    //declare interrupt for encoder
    attachInterrupt(ENCA1, encoderDataA, RISING);
    attachInterrupt(ENCB1, encoderDataB, RISING);

}

void drive(int distance){
    //reset encoder count
    motors.resetEncoders();

    //calc target encoder count
    // wheel diameter = 32mm
    // wheel circumference = 100.53mm
    // 60 ticks per revolution
    // 100.53/60 = 1.6755mm per tick
    // so distance in mm / 1.6755 = ticks
    int ticks = round(distance / 1.6755);
    //subtract 30 ticks to account for the distance the robot will travel before the sensor detects the obstacle and stops the robot from moving forward
    ticks = ticks - 30;
    //print ticks
    Serial.print("ticks: ");
    Serial.println(ticks);
    //set encoder target
    PID_A.setTarget(ticks);
    PID_B.setTarget(ticks);

    motors.encoderTargetA = ticks;
    motors.encoderTargetB = ticks;
   
   motors.setSpeedA(PID_A.update(motors.encoderCountA));
    motors.setSpeedB(PID_B.update(motors.encoderCountB));

    while (true){
        //get the ticks remaining
        int ticksRemainingA = motors.encoderTargetA - motors.encoderCountA;
        int ticksRemainingB = motors.encoderTargetB - motors.encoderCountB;
        //Get the amount of ticks remaining as a percentage of the target
        float percentRemainingA = (float)ticksRemainingA / (float)motors.encoderTargetA * 100;
        float percentRemainingB = (float)ticksRemainingB / (float)motors.encoderTargetB * 100;

        Serial.print("percentRemainingA: ");
        Serial.print(percentRemainingA);
        Serial.print(" percentRemainingB: ");
        Serial.println(percentRemainingB);

        if (percentRemainingA < 3 ){
            motors.stopA();
        }

        if (percentRemainingB < 3){
            motors.stopB();
        }

        if (percentRemainingA < 1 && percentRemainingB < 1){
            Serial.print("Target(A): ");
            Serial.print(motors.encoderTargetA);
            Serial.print(" Result(A): ");
            Serial.println(motors.encoderCountA);
            Serial.print("Target(B): ");
            Serial.print(motors.encoderTargetB);
            Serial.print(" Result(B): ");
            Serial.println(motors.encoderCountB);         
            break;
        }

        motors.setSpeedA(PID_A.update(motors.encoderCountA));
        motors.setSpeedB(PID_B.update(motors.encoderCountB));




    }


    
}



int counter = 0;

void loop(){

  if (counter < 1){
        if (sensor.read() > 0){
            Serial.println("Distance(mm): ");
            Serial.println(sensor.read());      
            drive(sensor.read());
            counter++;
        }
  }

    
}