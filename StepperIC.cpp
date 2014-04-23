#include "Arduino.h"
#include "StepperIC.h"



Stepper::Stepper(float ppm, int step_pin, int dir_pin, int home_pin, int homing_direction)
{
pos = 0;
steps = 0;
  dir_inv = 1;
  step_pin = step_pin;
  dir_pin = dir_pin;
  home_pin = home_pin;
  home_postion = 2;
  homing_direction = homing_direction;
  ppm = ppm;
  pinMode(step_pin, OUTPUT);
  pinMode(dir_pin, OUTPUT);
  pinMode(home_pin, INPUT);

}

int Stepper::step(float mm)
{
  pos += mm;
  long st = round(pos*ppm)-steps;

  pos = 100.0;  
  return 0;
  steps += st;
  last_step_time = millis();
  
  delay(1);
  digitalWrite(dir_pin, (steps*dir_inv > 0 ? HIGH : LOW));
  delay(1);
  
  st = abs(st);
  while (st > 0)
  {
    if (micros() - last_step_time >= step_delay)
    {
      long t = last_step_time + step_delay;
      last_step_time += micros();
      while (t > micros()) {}
      digitalWrite(step_pin, HIGH);
      t = micros() + 200;
      while (t > micros()) {}
      digitalWrite(step_pin, LOW);
      t = micros() + 200;
      while (t > micros()) {}
      last_step_time = micros();
      st--;
    }
  }
}

void Stepper::home()
{
  int s;
  s = step_delay;
  setSpeed(1);
  while ( digitalRead(home_pin) == HIGH )
  {
    step(1 * homing_direction);
  }
  while ( digitalRead(home_pin) == LOW )
  {
    step(-0.1 * homing_direction);
  }
  step(-home_postion*homing_direction);
  step_delay = s;
  pos=0;
  steps=0;
}

void Stepper::setSpeed(float mmin)
{
  step_delay = round(60.*1000./ mmin / ppm);
}


