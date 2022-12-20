#define TICKS_PER_ROTATION 20

class PID {
  public:
    PID(float p, float i, float d) {
      // Set PID coefficients
      kp = p;
      ki = i;
      kd = d;
    }

    void setTarget(int target) {
      // Set target tick count
      targetTicks = target;
    }


    int update(int currentTicks) {
      // Calculate error
      int error = targetTicks - currentTicks;

      // Calculate proportional term
      float pTerm = kp * error;

      // Update integral term
      iTerm += ki * error;

      // Constrain integral term to prevent windup
      iTerm = constrain(iTerm, -100, 100);

      // Calculate derivative term
      float dTerm = kd * (error - prevError);

      // Calculate total output
      output = pTerm + iTerm + dTerm;

      // Constrain output to 0-100%
      output = constrain(output, 0, 100);

      // Save error for next iteration
      prevError = error;

      // Return output as integer
      return (int)output;
    }

    int targetTicks; // Target tick count

  private:
    float kp; // Proportional coefficient
    float ki; // Integral coefficient
    float kd; // Derivative coefficient

    float iTerm; // Integral term
    float prevError; // Previous error
    float output; // PID output
};