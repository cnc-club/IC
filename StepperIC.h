// ensure this library description is only included once
#ifndef Stepper_h
#define Stepper_h
class Stepper {
  public:
    Stepper(float ppm, int step_pin, int dir_pin, int home_pin, int homing_direction);
    int dir_inv;
    void home();
    void setSpeed(float mmin);
    int step(float mm);
    float pos;
    long steps;
    float ppm;
    float home_postion;
    int dir_pin;
    int step_pin;
    int home_pin;    
    int homing_direction;    
  private:
    unsigned long step_delay;    // delay between steps, in ms, based on speed
    unsigned long last_step_time;

};

#endif
