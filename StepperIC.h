// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h
class Stepper {
  public:
    Stepper(float ppm, int step_pin, int dir_pin, int home_pin, int homing_direction);
    void home();
    void setSpeed(float mmin);
    void step(int number_of_steps);
    long pos;
    float ppm;
  private:
    unsigned long step_delay;    // delay between steps, in ms, based on speed
    unsigned long last_step_time;
    int dir_pin;
    int step_pin;
    int home_pin;    
    int homing_direction;
};

#endif
