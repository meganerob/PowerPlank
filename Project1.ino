//      ******************************************************************
//      *                                                                *
//      *                            Project 1                           *
//      *      Project name: ___Robot Arm______________________          *
//      *      Student name: __________________________________          *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Academy, 2017       *
//      *                                                                *
//      ******************************************************************


//
// declare IO pins below, ie: const byte LIMIT_SWITCH_1_PIN = 21;
//

const byte armStepperPin = 1;
const byte ARM_HOMING_PROX = 23; //proximity sensor
const byte HIGH_TOWER_PROX = 25;
const byte LOW_TOWER_PROX = 24;
const byte electromagPin = 26;
const byte airPin = 4;

const byte multiStep = 4; // 010 , # of multi steps
const byte transmission = 2; //little gear : big gear

const float acceleration = 0.5;
const float velocity = 0.5; //same as speed

//
// declare objects below, ie:  RCServo servo1;  or  SpeedyStepper stepper1;
//

SpeedyStepper armStepper;
RCServo electromag;

byte project1WorkingFlag = true;
bool project1InitializedFlag;

void moveToHighTower() {
  armStepper.moveToPositionInRevolutions(0.25);
}

void moveToLowTower() {
  armStepper.moveToPositionInRevolutions(0.415);
}

void electromagnetOn() {
  electromag.setServoPosition(1);
}

void electromagnetOff() {
  electromag.setServoPosition(0);
}

void armUp() {
  digitalWrite(airPin, LOW); //air going in to lower tube, raises arm
}

void armDown() {
  digitalWrite(airPin, HIGH);//air going in to higher tube, lowers arm
}


// ---------------------------------------------------------------------------------

//
// setup for the project, typically used to configure its IO pins, this is only called 
// once when the arduino is first powered on
// Note: Things done here should take no time.  Setup procedures that take time (such as
// homing stepper motors) should be done in initializeProject1()
//
void setupProject1()
{ 


  //
  // start with flags indicate the project is working, but not yet initialized
  //
  project1WorkingFlag = true;
  project1InitializedFlag = false;


  //
  // configure IO pins below, ie: pinMode(24, OUTPUT);  or  pinMode(25, INPUT_PULLUP);
  //

  
  
  //
  // connect objects to pins below, ie: stepper1.connectToPort(1);  or  servo1.connectToPin(8);) 
  //

  armStepper.connectToPort(armStepperPin);
  armStepper.setStepsPerRevolution(200 * multiStep * transmission);
  armStepper.setAccelerationInRevolutionsPerSecondPerSecond(acceleration);
  armStepper.setSpeedInRevolutionsPerSecond(velocity);

  armStepper.moveToHomeInRevolutions(-1, velocity, 1, ARM_HOMING_PROX);

  electromag.connectToPin(electromagPin, 1000, 2000, 0);

  //
  // set to true to indicate that this project is working
  //
  project1WorkingFlag = true;
}



//
// initialize the project, this includes setup steps that take time, such as homing a
// stepper motor.
//
void initializeProject1(void)
{
  //
  // return if not already initialized
  //
  if (project1InitializedFlag ==  true)
    return;


  //
  // initialize things that take time here
  //


  project1InitializedFlag = true;
}



//
// enable the project, this is a good place to "enable" stepper motors or other high
// current devices, it is called everytime before the project runs
//
void enableProject1(void)
{
  //
  // enable high current devices below, ie: stepper1.enableStepper();
  //
  armStepper.enableStepper();
}



//
// disable the project, this is a good place to "disable" stepper motors or other high
// current devices, it is called after the project runs
//
void disableProject1(void)
{
  //
  // disable high current devices below, ie: stepper1.disableStepper();
  //
  armStepper.disableStepper();
}



//
// run the project, return when the project completes
//
void runProject1(byte databyte)
{ 
 if(digitalRead(HIGH_TOWER_PROX) == 0) {        //0 = metal detected, 1 = no metal
    moveToHighTower();
    armDown();
    electromagnetOn();
    delay(1000);
    armUp();
    delay(1000);

    moveToLowTower();
    armDown();
    delay(1000);
    electromagnetOff();
    delay(1000);
    armUp();
    delay(1000);
  }
  
  if(digitalRead(LOW_TOWER_PROX) == 0) {
    moveToLowTower();
    armDown();
    delay(2000);
    electromagnetOn();
    delay(1000);
    armUp();
    delay(1000);

    moveToHighTower();
    armDown();
    delay(1500);
    electromagnetOff();
    armUp();
    delay(1000);
  }
  
}



//
// call to deterime if this project is working
//    Exit:  true returned if working
//
bool isWorkingProject1(void)
{
  return(project1WorkingFlag);
}


// -------------------------------------- End --------------------------------------


