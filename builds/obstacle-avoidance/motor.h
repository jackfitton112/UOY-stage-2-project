class Motor
{

private:
    // these are the private variables of the class Motor
    // these variables are only accessible within the class Motor
    // these variables are used to store the pin numbers of the motor driver and encoder
    int pwmPin;
    int inputAPin;
    int inputBPin;
    int encoderAPin;
    int encoderBPin;
    int previousSpeedPercentage = 0;
    int minSpeedCount = 0;

    //serves as a cache for the speed, so that it saves calculating the speed every time it is called
    int watchdogCount = 0;

public:
    int encoderCount = 0;
    int encoderTarget;


    // this is the constructor for the class Motor and it takes 5 arguments (int pwm, int inputA, int inputB, int encoderA, int encoderB)
    //  the constructor is called when you create an instance of the class

    Motor(int pwm, int inputA, int inputB, int encoderA, int encoderB)
    {
        pwmPin = pwm;
        inputAPin = inputA;
        inputBPin = inputB;
        encoderAPin = encoderA;
        encoderBPin = encoderB;
        pinMode(pwmPin, OUTPUT);
        pinMode(inputAPin, OUTPUT);
        pinMode(inputBPin, OUTPUT);
        pinMode(encoderAPin, INPUT);
        pinMode(encoderBPin, INPUT);
        setDirection(1);
    }

    // this is a method of the class Motor and it takes 1 argument (int speedPercentage)
    //  this method is used to set the speed of the motor as a percentage of the max speed

    void setSpeed(int speedPercentage)
    {
        // Set the PWM value to control the speed of the motor
        // 0 = stop, 100 = full speed
        // pwm is the value that is sent to the motor driver to control the speed of the motor. it maps the speedPercentage to a value between 0 and 255
        int pwm = map(speedPercentage, 0, 100, 0, 255);
        previousSpeedPercentage = speedPercentage;
        analogWrite(pwmPin, pwm);
    }

    void setDirection(int direction)
    {
        // Set the direction of the motor
        // 1 = forward, 0 = reverse
        digitalWrite(inputAPin, direction);
        digitalWrite(inputBPin, !direction);
    }

    // this is a method of the class Motor and it takes 1 argument (int speedPercentage)
    //  this method gradually changes the speed of the motor from the previous speed to the new speed
    //  this method is used to avoid sudden changes in speed which can cause the motor to stall
    void changeSpeed(int speedPercentage)
    {
        speedPercentage = max(0, speedPercentage); // Ensure that speedPercentage is never less than 0

        if (speedPercentage > previousSpeedPercentage)
        {
            for (int i = previousSpeedPercentage; i < speedPercentage; i++)
            {
                setSpeed(i);
                delay(25);
            }
        }
        else if (speedPercentage < previousSpeedPercentage)
        {
            for (int i = previousSpeedPercentage; i > speedPercentage; i--)
            {
                setSpeed(i);
                delay(25);
            }
        }
    }

    void stop()
    {
        setSpeed(0);
    }


    //this function kind of works, there are still some issue with one motor spinning fatseter than the other however it is a good start

    void minSpeed(){


        if (minSpeedCount != 0 || watchdogCount < 10)
        {
            setSpeed(minSpeedCount);
            Serial.print("minSpeedCount: ");
            Serial.println(minSpeedCount);
            watchdogCount++;
            return;

        }

        //syncs the encoder count to the previous encoder count
        int prevEncoderCount = encoderCount;

        for (int i = 1; i < 100; i++)
        {
            //sets the speed to the current speed
            setSpeed(i);
            //waits 25 milliseconds
            delay(10);
            //if the encoder count is the same as the previous encoder count
            if (encoderCount > prevEncoderCount)
            {
                //sets the minimum speed to the current speed
                minSpeedCount = i;
                //resets the watchdog count to 0
                watchdogCount = 0;
                //breaks out of the loop
                break;
            }
            
        }

        //add 2% to the minimum speed to ensure the motor is moving
        minSpeedCount += 2;
        //sets the speed to the minimum speed
        setSpeed(minSpeedCount);



    }



};