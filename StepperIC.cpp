#include "Arduino.h"
#include "StepperIC.h"



Stepper::Stepper(int ppm, int step_pin, int dir_pin, int home_pin, int homing_direction)
{
  this->pos = 0;
  this->step_pin = step_pin;
  this->dir_pin = dir_pin;
  this->home_pin = home_pin;
  this->homing_direction = homing_direction;
  pinMode(this->step_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);
  pinMode(this->home_pin, INPUT);

}

void Stepper::step(int steps)
{
  this->last_step_time = millis();
  delay(1);
  digitalWrite(this->dir_pin,(steps>0?HIGH:LOW));
  delay(1);
  this->pos += steps;
  steps = abs(steps);
  while (steps>0)
  {
    if (micros() - this->last_step_time >= this->step_delay)
    {
      digitalWrite(this->step_pin,HIGH);
      this->last_step_time = micros()+200;
      while (this->last_step_time > micros()){}
      digitalWrite(this->step_pin,LOW);
      this->last_step_time = micros()+200;
      while (this->last_step_time > micros()){}      
      this->last_step_time = micros();
      steps--;
    }
  }  
}

void Stepper::home()
{
  while( digitalRead(this->home_pin) == HIGH )
  {this->step(100*this->homing_direction);}
  while ( digitalRead(this->home_pin) == LOW )
  {this->step(-10*this->homing_direction);}
  this->pos = 0;
}

void Stepper::setSpeed(long mmin)
{this->step_delay = 60L * 1000L / this->ppm / mmin;}


