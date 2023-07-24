#include <AccelStepper.h>

// Define the number of steps for a full rotation
const int SPR = 200;    //Steps per Revolution
const int DIST = 20;    //Number of Rotations per direction per cycle
const int MAXSPEED = 4;  //Rotations per second
const int ACCEL = 4;     //Rotations per second per second

const int interruptPin = 7;
const int EnablePin = 9;
const int StepPin = 11;	
const int DirPin = 10;

bool motor_run = false;

// Initialize the stepper motor object with the pins used
AccelStepper stepper(AccelStepper::DRIVER, StepPin, DirPin);

void Motor_Switch();

void setup() {
  // put your setup code here, to run once:
  pinMode(EnablePin, OUTPUT);																		//nnfjfjkjlfjkj fkj f
  pinMode(interruptPin, INPUT);																		//nnfjfjkjlfjkj fkj f
  digitalWrite(EnablePin, LOW);																		//nnfjfjkjlfjkj fkj f
  attachInterrupt(digitalPinToInterrupt(interruptPin), Motor_Switch, FALLING);
  stepper.setMaxSpeed(MAXSPEED * SPR);																//nnfjfjkjlfjkj fkj f
  stepper.setAcceleration(ACCEL * SPR);																//nnfjfjkjlfjkj fkj f
}				

void Motor_Switch() {
  motor_run = !motor_run;
}

void loop() {
  if (motor_run) {
    digitalWrite(EnablePin, LOW);
    // Rotate the motor all the way in the opposite direction
    stepper.move(SPR * DIST);
    stepper.runToPosition();
    
    // Pause for one second
    delay(500);
    
    // Rotate the motor all the way in the opposite direction
    stepper.move(-SPR * DIST);				//nnfjfjkjlfjkj fkj f
    stepper.runToPosition();				//nnfjfjkjlfjkj fkj f
    
    // Pause for one second
    delay(500);
  } else {
    digitalWrite(EnablePin, HIGH);
  }
}
