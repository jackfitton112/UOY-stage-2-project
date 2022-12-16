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
#include "motor.h"

//init objects
VL53L1X sensor;
Motor motorA(PWMA, AIN1, AIN2, ENCA1, ENCA2);
Motor motorB(PWMB, BIN1, BIN2, ENCB1, ENCB2);



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

void IRAM_ATTR encoderData()
{
    int ENCA1_DATA = digitalRead(ENCA1);
    int ENCA2_DATA = digitalRead(ENCA2);
    int ENCB1_DATA = digitalRead(ENCB1);
    int ENCB2_DATA = digitalRead(ENCB2);

    if (ENCA1_DATA == HIGH && ENCA2_DATA == HIGH)
    {
        motorA.encoderCount++;
    }
    else if (ENCA1_DATA == HIGH && ENCA2_DATA == LOW)
    {
        motorA.encoderCount--;
    }

    if (ENCB1_DATA == HIGH && ENCB2_DATA == HIGH)
    {
        motorB.encoderCount++;
    }
    else if (ENCB1_DATA == HIGH && ENCB2_DATA == LOW)
    {
        motorB.encoderCount--;
    }

}

void setup(){

    //start serial communication
    Serial.begin(115200);

    //init sensor
    sensorInit();

    //declare interrupt for encoder
    attachInterrupt(ENCA1, encoderData, RISING);
    attachInterrupt(ENCB1, encoderData, RISING);

}

void drive(int distance, int speed){
    //reset encoder count
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;

    //calc target encoder count
    // wheel diameter = 32mm
    // wheel circumference = 100.53mm
    // 60 ticks per revolution
    // 100.53/60 = 1.6755mm per tick
    // so distance in mm / 1.6755 = ticks
    int ticks = round(distance / 1.6755);
    //print ticks
    Serial.print("ticks: ");
    Serial.println(ticks);
    //set encoder target
    motorA.encoderTarget = ticks;
    motorB.encoderTarget = ticks;

    motorA.changeSpeed(speed);
    motorB.changeSpeed(speed);

    while (true){

        //get the ticks remaining
        int ticksRemainingA = motorA.encoderTarget - motorA.encoderCount;
        int ticksRemainingB = motorB.encoderTarget - motorB.encoderCount;
        //Get the amount of ticks remaining as a percentage of the target
        float percentRemainingA = (float)ticksRemainingA / (float)ticks * 100;
        float percentRemainingB = (float)ticksRemainingB / (float)ticks * 100;

        if (percentRemainingA < 50 && percentRemainingA > 25){
            //if both motors are more than 50% of the way to the target, set speed to 20
            int newSpeedA = speed << 1;
            motorA.changeSpeed(newSpeedA);
        }

        else if (percentRemainingA < 25 && percentRemainingA > 15){
            //if both motors are more than 25% of the way to the target, set speed to 1/4 of initial speed
            int newSpeedA = speed << 2;
            motorA.changeSpeed(newSpeedA);
        }

        else if (percentRemainingA < 15 && percentRemainingA > 5){
            //if both motors are more than 10% of the way to the target, set speed to 1/8 of initial speed
            int newSpeedA = speed << 3;
            motorA.changeSpeed(newSpeedA);
        }

        else if (percentRemainingA < 5){
            //if both motors are more than 5% of the way to the target, set speed to 0
            motorA.stop();
        }

        //repeat for motor B

        if (percentRemainingB < 50 && percentRemainingB > 25){
            //if both motors are more than 50% of the way to the target, set speed to 20
            int newSpeedB = speed << 1;
            motorB.changeSpeed(newSpeedB);
        }

        else if (percentRemainingB < 25 && percentRemainingB > 15){
            //if both motors are more than 25% of the way to the target, set speed to 1/4 of initial speed
            int newSpeedB = speed << 2;
            motorB.changeSpeed(newSpeedB);
        }

        else if (percentRemainingB < 15 && percentRemainingB > 5){
            //if both motors are more than 10% of the way to the target, set speed to 1/8 of initial speed
            int newSpeedB = speed << 3;
            motorB.changeSpeed(newSpeedB);
        }

        else if (percentRemainingB < 5){
            //if both motors are more than 5% of the way to the target, set speed to 0
            motorB.stop();
        }

        Serial.print("percentRemainingA: ");
        Serial.print(percentRemainingA);
        Serial.print(" percentRemainingB: ");
        Serial.println(percentRemainingB);

        if (percentRemainingA < 5 && percentRemainingB < 5){
            break;
        }


   }


}

int counter = 0;

void loop(){

  if (counter < 1){
    if (sensor.read() > 0){
        Serial.println("Distance(mm): ");
        Serial.println(sensor.read());      
        drive(sensor.read(), 20);
        counter++;
    }
  }

  
  
}