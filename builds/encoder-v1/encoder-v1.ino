
//Date Created: 15/12/2022
// this is a test program for the ESP32
// it will use encoder to control the motor
// and take data from the TOF sensor to create a closed loop system

#include "motor.h"

#include "BluetoothSerial.h"


//Notes
// a 90 degree turn is about 45 ticks
// this needs confirming but it seems to be the case

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

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

Motor motorA(PWMA, AIN1, AIN2, ENCA1, ENCA2);
Motor motorB(PWMB, BIN1, BIN2, ENCB1, ENCB2);

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

BluetoothSerial SerialBT;


void setup()
{
    Serial.begin(9600);
    SerialBT.begin("ESP32test"); // Bluetooth device name
    Serial.println("The device started, now you can pair it with bluetooth!");

    // starts the motor
    // Direction is default to 1 (forward)
    //motorA.changeSpeed(10);
    //motorB.changeSpeed(10);

    // Attach the encoderData function to both ENCA and ENCB using the RISING mode
    attachInterrupt(ENCA1, encoderData, RISING);
    attachInterrupt(ENCB1, encoderData, RISING);
}

void loop()
{
    
    // put your main code here, to run repeatedly:
    if (SerialBT.available())
    {
        // read the incoming byte:
        //int incomingByte = SerialBT.read();
        // say what you got:
        int val = SerialBT.parseInt();
        Serial.print("Spinning: ");
        motorA.encoderCount = 0;
        motorB.encoderCount = 0;
        square(val);
    
    }

    delay(20);

    /*

    //send serial data to bluetooth
    if (Serial.available()) {
    SerialBT.write(Serial.read());
    }

    delay(20);
    */

}

void spin (int ticks){
    String msg = "";  
    motorA.setDirection(1);
    motorB.setDirection(0);

    int motorACount = motorA.encoderCount + ticks;
    int motorBCount = motorB.encoderCount - ticks;

    // while the encoder count is less than the target count
    // keep turning the motors

    while(true){
        if (motorA.encoderCount < motorACount){
            motorA.changeSpeed(10);
        } else {
            motorA.stop();
        }

        if (motorB.encoderCount > motorBCount){
            motorB.changeSpeed(10);
        } else {
            motorB.stop();
        }

        if (abs(motorA.encoderCount - motorACount) < 3 && abs(motorB.encoderCount - motorBCount) < 3){
            motorA.stop();
            motorB.stop();
            //msg = "SB MA: " + motorA.encoderCount + ":" + motorACount + " MB: " + motorB.encoderCount + ":" + motorBCount;
            SerialBT.write(motorA.encoderCount);
            SerialBT.write(motorACount);
            SerialBT.write(11111);
            SerialBT.write(motorB.encoderCount);
            SerialBT.write(motorBCount);          
            break;
        }

        if (motorA.encoderCount > motorACount || motorB.encoderCount < motorBCount){
            motorA.stop();
            motorB.stop();
           // msg = "SB MA: " + motorA.encoderCount + ":" + motorACount + " MB: " + motorB.encoderCount + ":" + motorBCount;            
            //SerialBT.write(msg)
            break;
        }
    }
}

void square(int size)
{

    forward(size);
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
    delay(1000);
    turnLeft();
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
    delay(1000);
    forward(size);
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
    delay(1000);
    turnLeft();
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
    delay(1000);
    forward(size);
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
    delay(1000);
    turnLeft();
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
    delay(1000);
    forward(size);
    motorA.encoderCount = 0;
    motorB.encoderCount = 0;
}

void forward(int noOfRotations)
{

    Serial.print("Forward: ");
    Serial.print(noOfRotations);
    Serial.println("Rotations");
    int ticks = noOfRotations * 60;
    // set both motors to forward
    motorA.setDirection(1);
    motorB.setDirection(1);

    int motorACount = motorA.encoderCount + ticks;
    int motorBCount = motorB.encoderCount + ticks;

    // while the encoder count is less than the target count
    // keep turning the motors
    while (true)
    {
        if (motorA.encoderCount < motorACount)
        {
            motorA.changeSpeed(10);
        }
        else
        {
            motorA.stop();
        }

        if (motorB.encoderCount < motorBCount)
        {
            motorB.changeSpeed(10);
        }
        else
        {
            motorB.stop();
        }

        if (abs(motorA.encoderCount - motorACount) < 3 && abs(motorB.encoderCount - motorBCount) < 3)
        {
            motorA.stop();
            motorB.stop();
            Serial.print("FWD BREAK - Motor A val ");
            Serial.print(motorA.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorACount);
            Serial.print("FWD BREAK - Motor B val ");
            Serial.print(motorB.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorBCount);
            break;
        }

        if (motorA.encoderCount > motorACount && motorB.encoderCount > motorBCount)
        {
            motorA.stop();
            motorB.stop();
            Serial.print("FWD BREAK - Motor A val ");
            Serial.print(motorA.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorACount);
            Serial.print("FWD BREAK - Motor B val ");
            Serial.print(motorB.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorBCount);
            break;
        }
    }
}

void turnLeft()
{
    // requires some work
    Serial.println("Turning Left 90 degrees");
    // set motor A to reverse
    motorA.setDirection(0);
    // set motor B to forward
    motorB.setDirection(1);

    // turning left requires 30 ticks of the encoder
    // this is the number of ticks the motors need to turn
    // we store the current encoder count in a variable

    int motorACount = motorA.encoderCount - 45;
    int motorBCount = motorB.encoderCount + 45;

    // while the encoder count is less than the target count
    // keep turning the motors
    while (true)
    {
        if (motorA.encoderCount > motorACount)
        {
            motorA.changeSpeed(10);
        }
        else
        {
            motorA.stop();
        }

        if (motorB.encoderCount < motorBCount)
        {
            motorB.changeSpeed(10);
        }
        else
        {
            motorB.stop();
        }

        if (abs(motorA.encoderCount - motorACount) < 3 && abs(motorB.encoderCount - motorBCount) < 3)
        {
            motorA.stop();
            motorB.stop();
            Serial.println("LEFT BREAK - Motor A val ");
            Serial.print(motorA.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorACount);
            Serial.println("LEFT BREAK - Motor B val ");
            Serial.print(motorB.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorBCount);
            break;
        }

        // if the motors have gone past the target count, break the loop and stop the motors from turning
        if (motorA.encoderCount < motorACount && motorB.encoderCount > motorBCount)
        {
            motorA.stop();
            motorB.stop();
            Serial.println("LEFT BREAK - Motor A val ");
            Serial.print(motorA.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorACount);
            Serial.println("LEFT BREAK - Motor B val ");
            Serial.print(motorB.encoderCount);
            Serial.print(" target val ");
            Serial.println(motorBCount);
            break;
        }
    }
}