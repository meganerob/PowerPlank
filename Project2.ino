//      ******************************************************************
//      *                                                                *
//      *                            Project 2                           *
//      *      Project name: __Perpetual Motion________________          *
//      *      Student name: __________________________________          *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Academy, 2017       *
//      *                                                                *
//      ******************************************************************


//
// declare IO pins below, ie: const byte LIMIT_SWITCH_1_PIN = 22;
//

const int RAMP_HOME_SENSOR_PIN = A3;
const int RAMP_LOWER_SENSOR_PIN = A1;
const int RAMP_UPPER_SENSOR_PIN = A2;
const int RAMP_STEPPER_PIN = 1;
const int SERVO_PIN = A0;
const int MOTOR = 10;

const int microStep = 4;
const int speedInMMPSPS = 100;
const int accelerationInMMPS = 100;

//
// declare objects below, ie:  RCServo servo1;  or  SpeedyStepper stepper1;
//

SpeedyStepper rampStepper;
RCServo servo;


byte project2WorkingFlag = true;
bool project2InitializedFlag;

void goHome() {
  rampStepper.moveToHomeInMillimeters(-1, 60, 250, RAMP_HOME_SENSOR_PIN);
}

void stairsUp() {
  digitalWrite(MOTOR, HIGH);
}

void stairsStop() {
  digitalWrite(MOTOR, LOW);
}

void openDoor() {
  servo.setServoPosition(0);
}

void closeDoor() {
  servo.setServoPosition(-1);
}

void goUp (int mm) {
  rampStepper.moveRelativeInMillimeters(mm);
}


// ---------------------------------------------------------------------------------

//
// setup for the project, typically used to configure its IO pins, this is only called 
// once when the arduino is first powered on
// Note: Things done here should take no time.  Setup procedures that take time (such as
// homing stepper motors) should be done in initializeProject2()
//
void setupProject2()
{ 


  //
  // start with flags indicate the project is working, but not yet initialized
  //
  project2WorkingFlag = true;
  project2InitializedFlag = false;


  //
  // configure IO pins below, ie: pinMode(24, OUTPUT);  or  pinMode(25, INPUT_PULLUP);
  //

  pinMode(RAMP_HOME_SENSOR_PIN, INPUT_PULLUP);
  pinMode(RAMP_LOWER_SENSOR_PIN, INPUT_PULLUP);
  pinMode(RAMP_UPPER_SENSOR_PIN, INPUT_PULLUP);
  pinMode(RAMP_STEPPER_PIN, OUTPUT);
  pinMode(MOTOR, OUTPUT);
  
  //
  // connect objects to pins below, ie: stepper1.connectToPort(1);  or  servo1.connectToPin(8);) 
  //

  servo.connectToPin(SERVO_PIN);

  rampStepper.setStepsPerMillimeter(25 * microStep);
  rampStepper.connectToPort(RAMP_STEPPER_PIN);
  rampStepper.setSpeedInMillimetersPerSecond(speedInMMPSPS);
  rampStepper.setAccelerationInMillimetersPerSecondPerSecond(accelerationInMMPS);

  //
  // set to true to indicate that this project is working
  //
  project2WorkingFlag = true;
}



//
// initialize the project, this includes setup steps that take time, such as homing a
// stepper motor.
//
void initializeProject2(void)
{
  //
  // return if not already initialized
  //
  if (project2InitializedFlag ==  true)
    return;


  //
  // initialize things that take time here
  //
  goHome();
  closeDoor();
  delay(1000);

  project2InitializedFlag = true;
}



//
// enable the project, this is a good place to "enable" stepper motors or other high
// current devices, it is called everytime before the project runs
//
void enableProject2(void)
{
  //
  // enable high current devices below, ie: stepper1.enableStepper();
  //
  rampStepper.enableStepper();
}



//
// disable the project, this is a good place to "disable" stepper motors or other high
// current devices, it is called after the project runs
//
void disableProject2(void)
{
  //
  // disable high current devices below, ie: stepper1.disableStepper();
  //
  rampStepper.disableStepper();
}



//
// run the project, return when the project completes
//
void runProject2(byte databyte)
{
  delay(1500);
  if(digitalRead(RAMP_LOWER_SENSOR_PIN) == 0) {
    goUp(225);
  }
  delay(1000);
  stairsUp();
  delay(3500);
  stairsStop();
  goHome();
  openDoor();
  delay(1000);
  closeDoor();
}



//
// call to deterime if this project is working
//    Exit:  true returned if working
//
bool isWorkingProject2(void)
{
  return(project2WorkingFlag);
}


// -------------------------------------- End --------------------------------------


