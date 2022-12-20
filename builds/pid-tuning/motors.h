class Motors
{

private:
    // these are the private variables of the class Motor
    // these variables are only accessible within the class Motor
    // these variables are used to store the pin numbers of the motor driver and encoder
    int A_pwmPin;
    int A_inputAPin;
    int A_inputBPin;
    int A_encoderAPin;
    int A_encoderBPin;
    int A_previousSpeedPercentage = 0;
    int A_minSpeedCount = 0;

    int B_pwmPin;
    int B_inputAPin;
    int B_inputBPin;
    int B_encoderAPin;
    int B_encoderBPin;
    int B_previousSpeedPercentage = 0;
    int B_minSpeedCount = 0;

    int Motors_prevSpeed;
    int Motors_PWM;

    //serves as a cache for the speed, so that it saves calculating the speed every time it is called
    int watchdogCount = 0;

public:

    int encoderCountA = 0;
    int encoderTargetA;

    int encoderCountB = 0;
    int encoderTargetB;
    int PWM_Limit = 255;

    // this is the constructor for the class Motor and it takes 5 arguments (int pwm, int inputA, int inputB, int encoderA, int encoderB)
    //  the constructor is called when you create an instance of the class

    Motors(int pwmA, int inputA1, int inputA2, int encoderA1, int encoderA2, int pwmB, int inputB1, int inputB2, int encoderB1, int encoderB2)
    {

        //Motor A
        A_pwmPin = pwmA;
        A_inputAPin = inputA1;
        A_inputBPin = inputA2;
        A_encoderAPin = encoderA1;
        A_encoderBPin = encoderA2;

        pinMode(A_pwmPin, OUTPUT);
        pinMode(A_inputAPin, OUTPUT);
        pinMode(A_inputBPin, OUTPUT);
        pinMode(A_encoderAPin, INPUT);
        pinMode(A_encoderBPin, INPUT);

        //Motor B
        B_pwmPin = pwmB;
        B_inputAPin = inputB1;
        B_inputBPin = inputB2;
        B_encoderAPin = encoderB1;
        B_encoderBPin = encoderB2;

        pinMode(B_pwmPin, OUTPUT);
        pinMode(B_inputAPin, OUTPUT);
        pinMode(B_inputBPin, OUTPUT);
        pinMode(B_encoderAPin, INPUT);
        pinMode(B_encoderBPin, INPUT);

        setDirection(1);

    }


    void changeSpeed(int newSpeed){

        if (A_previousSpeedPercentage != B_previousSpeedPercentage){
            stop();
            Motors_prevSpeed = 0;
        } else {
            Motors_prevSpeed = A_previousSpeedPercentage;
        }


        if (newSpeed > Motors_prevSpeed){
            for (int i = Motors_prevSpeed; i <= newSpeed; i++){
                setSpeed(i);
                delay(10);
            }
        } else if (newSpeed < Motors_prevSpeed){
            for (int i = Motors_prevSpeed; i >= newSpeed; i--){
                setSpeed(i);
                delay(10);
            }
        } else {
            setSpeed(newSpeed);
        }

    }

    void setSpeed(int speedPercentage){

        int pwm = map(speedPercentage, 0, 100, 0, 255);

        analogWrite(A_pwmPin, pwm);
        analogWrite(B_pwmPin, pwm);

        A_previousSpeedPercentage = speedPercentage;
        B_previousSpeedPercentage = speedPercentage;

    }

    void setSpeedA(int speedPercentage){

        int pwm = map(speedPercentage, 0, 100, 0, PWM_Limit);

        analogWrite(A_pwmPin, pwm);

        A_previousSpeedPercentage = speedPercentage;

    }

    void setSpeedB(int speedPercentage){

        int pwm = map(speedPercentage, 0, 100, 0, PWM_Limit);

        analogWrite(B_pwmPin, pwm);

        B_previousSpeedPercentage = speedPercentage;

    }

    void stop(){
        setSpeed(0);
    }

    void stopA(){
        setSpeedA(0);
    }

    void stopB(){
        setSpeedB(0);
    }

    void resetEncoders(){
        encoderCountA = 0;
        encoderCountB = 0;
    }

    void setEncoderTarget(int target){
        encoderTargetA = target;
        encoderTargetB = target;
    }

    void setDirection(int direction){
        digitalWrite(A_inputAPin, direction);
        digitalWrite(A_inputBPin, !direction);
        digitalWrite(B_inputAPin, direction);
        digitalWrite(B_inputBPin, !direction);
    }

    void setDirectionA(int direction){
        digitalWrite(A_inputAPin, direction);
        digitalWrite(A_inputBPin, !direction);
    }

    void setDirectionB(int direction){
        digitalWrite(B_inputAPin, direction);
        digitalWrite(B_inputBPin, !direction);
    }


    void minSpeed(){

        int encoderAsnapshot = encoderCountA;
        int encoderBsnapshot = encoderCountB;
        bool A_flag = false;
        bool B_flag = false;

        int speedSnapshotA = 0;
        int speedSnapshotB = 0;


    

            for (int i; i <= 100; i++){
                
                if (encoderCountA > encoderAsnapshot){
                    speedSnapshotA = i;
                    A_flag = true;
                }

                if (encoderCountB > encoderBsnapshot){
                    speedSnapshotB = i;
                    B_flag = true;
                }

                if (A_flag && B_flag){
                    A_minSpeedCount = speedSnapshotA;
                    B_minSpeedCount = speedSnapshotB;
                    break;
                }

                if (i == 100){
                    A_minSpeedCount = 0;
                    B_minSpeedCount = 0;
                    break;
                }

                if (speedSnapshotA > 0){
                    setSpeedA(speedSnapshotA);
                } else {
                    setSpeedA(i);
                }

                if (speedSnapshotB > 0){
                    setSpeedB(speedSnapshotB);
                } else {
                    setSpeedB(i+0.9);
                }


                delay(10);

            }

            



    


    }

};