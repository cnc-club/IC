#include "Arduino.h"
#include "StepperIC.h"



Stepper::Stepper(float ppm, int step_pin, int dir_pin, int home_pin, int homing_direction)
{
  this->pos = 0;
  this->step_pin = step_pin;
  this->dir_pin = dir_pin;
  this->home_pin = home_pin;
  this->homing_direction = homing_direction;
  this->ppm = ppm;
  pinMode(this->step_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  pinMode(this->home_pin, INPUT);

}

void Stepper::step(int steps)
{
  long t;
  this->last_step_time = millis();
  delay(1);
  digitalWrite(this->dir_pin, (steps > 0 ? HIGH : LOW));
  delay(1);
  this->pos += steps;
  steps = abs(steps);
  while (steps > 0)
  {
    if (micros() - this->last_step_time >= this->step_delay)
    {
      t = this->last_step_time + this->step_delay;
      this->last_step_time += micros();
      while (t > micros()) {}
      digitalWrite(this->step_pin, HIGH);
      t = micros() + 200;
      while (t > micros()) {}
      digitalWrite(this->step_pin, LOW);
      t = micros() + 200;
      while (t > micros()) {}
      this->last_step_time = micros();
      steps--;
    }
  }
}

void Stepper::home()
{
  int s;
  s = this->step_delay;
  this->setSpeed(1);
  while ( digitalRead(this->home_pin) == HIGH )
  {
    this->step(10 * this->homing_direction);
  }
  while ( digitalRead(this->home_pin) == LOW )
  {
    this->step(-10 * this->homing_direction);
  }
  this->pos = 100*this->homing_direction;
  this->step_delay = s;
  this->step(-100*this->homing_direction);
}

void Stepper::setSpeed(float mmin)
{
  this->step_delay = round(60.*1000000./ mmin / this->ppm);
}


