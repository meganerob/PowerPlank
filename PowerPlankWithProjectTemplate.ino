//      ******************************************************************
//      *                                                                *
//      *                        Project Controller                      *
//      *                                                                *
//      *     Template                                                   *
//      *     Students:                                                  *
//      *                                                                *
//      *      Copyright (c) Dos Pueblos Engineering Academy, 2017       *
//      *                                                                *
//      ******************************************************************

#include <Arduino.h>
#include <Wire.h>
#include <EEPROM.h>
#include <MultiInterfaceBoard.h>
#include <RCServo.h>
#include <FlexyStepper.h>
#include <I2CServoController.h>
#include <SerialDebug.h>
#include <SpeedyStepper.h>
#include <UserInterface.h>
#include <EEPROMConfig.h>
#include <RCServoTuner.h>


//
// constants
//
#define AMIB_NAME "-POWER PLANK-" // name of this AMIB project

//
// select which project is started by pressing the button on the project board
//

#define BUTTON_STARTS_PROJECT_NUMBER 3

//
// display version number for architecture AMIB / Power Plank functionality
//
#define REV_NUMBER_MAJOR 2
#define REV_NUMBER_MINOR 1


//
// flags and values with information sent from the master;
//
bool projectCommandInitializeFlag = false;
bool projectCommandRunProject1 = false;
bool projectCommandRunProject2 = false;
bool projectCommandRunProject3 = false;
bool projectCommandRunProject4 = false;
byte projectDatabyte = 0;
byte enabledProjectsToInitialize= 0;





// ---------------------------------------------------------------------------------
//                             Hardware and software setup
// ---------------------------------------------------------------------------------

//
// top level setup function
//
void setup()
{   
  //
  // configure the Multi Interface Board I/O pins
  //
  setupMultiInterfaceBoard();   

  //
  // open communication with the host
  //

  //
  // setup the projects
  //
  setupProject1();
  disableProject1();

  setupProject2();
  disableProject2();

  setupProject3();
  disableProject3();

  setupProject4();
  disableProject4();

}



////
//// setup the Multi Interface Board
////
//void setupMultiInterfaceBoard()
//{
//  //
//  // configure the LED and button
//  //
//  pinMode(LED_PIN, OUTPUT);
//  digitalWrite(LED_PIN, LOW);
//  pinMode(BUTTON_PIN, INPUT_PULLUP);
//
//
//  //
//  // configure the Driver bits
//  //
//  pinMode(DRIVER_1_PIN, OUTPUT);
//  digitalWrite(DRIVER_1_PIN, LOW);
//
//  pinMode(DRIVER_2_PIN, OUTPUT);
//  digitalWrite(DRIVER_2_PIN, LOW);
//
//  pinMode(DRIVER_3_PIN, OUTPUT);
//  digitalWrite(DRIVER_3_PIN, LOW);
//
//  pinMode(DRIVER_4_PIN, OUTPUT);
//  digitalWrite(DRIVER_4_PIN, LOW);
//
//
//  //
//  // configure the stepper bits
//  //
//  pinMode(STEPPER_1_ENABLE_PIN, OUTPUT);
//  digitalWrite(STEPPER_1_ENABLE_PIN, STEPPER_ENABLE_DISABLED);
//  pinMode(STEPPER_1_DIRECTION_PIN, OUTPUT);
//  digitalWrite(STEPPER_1_DIRECTION_PIN, LOW);
//  pinMode(STEPPER_1_STEP_PIN, OUTPUT);
//  digitalWrite(STEPPER_1_STEP_PIN, LOW);
//
//  pinMode(STEPPER_2_ENABLE_PIN, OUTPUT);
//  digitalWrite(STEPPER_2_ENABLE_PIN, STEPPER_ENABLE_DISABLED);
//  pinMode(STEPPER_2_DIRECTION_PIN, OUTPUT);
//  digitalWrite(STEPPER_2_DIRECTION_PIN, LOW);
//  pinMode(STEPPER_2_STEP_PIN, OUTPUT);
//  digitalWrite(STEPPER_2_STEP_PIN, LOW);
//
//}


// ---------------------------------------------------------------------------------
//                   The main loop, with one button user interface
// ---------------------------------------------------------------------------------

//
// main loop
//
void loop()
{
  unsigned long pulseLastBlinkTimeInMS;
  unsigned long pulsePeriodInMS;
  byte pulseState;
  byte buttonEvent;

  //
  // loop until there is something to do
  //

  showMainDisplay();

  pulseLastBlinkTimeInMS = millis();
  while(true)
  {
    //
    // has the Switch 1 button been pressed?  If so run the assigned project
    //
    if (digitalRead(BUTTON_PIN) == LOW)
    {    
      //
      // the button has been pressed, turn off the LED and run the assigned project
      //
      digitalWrite(LED_PIN, LOW);
      runProject(BUTTON_STARTS_PROJECT_NUMBER, 0);
      showMainDisplay();
      digitalWrite(LED_PIN, HIGH);
    }
    
    //
    // check if the user has press the Menu button
    //
    buttonEvent = ui.getButtonEvent();
    if (buttonEvent == PUSH_BUTTON_SELECT + BUTTON_PUSHED)
    {
      digitalWrite(LED_PIN, LOW);
      showMainMenu();
      showMainDisplay();
      digitalWrite(LED_PIN, HIGH);
    }

    //
    // blink the LED indicate waiting
    //
    pulsePeriodInMS = millis() - pulseLastBlinkTimeInMS;
    if (pulsePeriodInMS >= 80)
    {
      if((pulseState == 1) || (pulseState == 3))
        digitalWrite(LED_PIN, HIGH);
      else if((pulseState == 2) || (pulseState == 4))
        digitalWrite(LED_PIN, LOW);
      else if(pulseState > 8)
        pulseState = 0;

      pulseLastBlinkTimeInMS = millis();
      pulseState++;
    }
  }
}

//
// update the top level display
//
  
void showMainDisplay(void)
{  
  ui.lcdClearDisplay();

  ui.lcdSetCursorXY(LCD_WIDTH_IN_PIXELS/2, 0);
  ui.lcdPrintStringCentered(AMIB_NAME, 0);

  ui.lcdSetCursorXY(0, 1);
  ui.lcdPrintString(" AMIB Rev ");
  ui.lcdPrintInt(REV_NUMBER_MAJOR);
  ui.lcdPrintString(".");
  ui.lcdPrintInt(REV_NUMBER_MINOR);


  ui.lcdSetCursorXY(0, 2);
  ui.lcdPrintStringCentered("Switch 1 runs", 0);

  ui.lcdSetCursorXY(0, 3);
  ui.lcdPrintString("Project: ");
  ui.lcdPrintInt(BUTTON_STARTS_PROJECT_NUMBER);

 
  //
  // wait for the user to press OK
  //
  ui.drawButtonBar("Menu", "");
}


//
// initialize all the projects
//  Enter:  projectsToInitialize bit 0 = 1 if project 1 should be initialized
//          projectsToInitialize bit 1 = 1 if project 2 should be initialized
//          projectsToInitialize bit 2 = 1 if project 3 should be initialized
//          projectsToInitialize bit 3 = 1 if project 4 should be initialized
//          projectsToInitialize bit 4 = 1 if project 5 should be initialized
//
void initializeProjects(byte projectsToInitialize)
{

  //
  // initialize project 1
  //
  if (projectsToInitialize & 0x01)
  {
    enableProject1();
    delay(150);
    initializeProject1();
    disableProject1();
  }

  //
  // initialize project 2
  //
  if (projectsToInitialize & 0x02)
  {
    enableProject2();
    delay(150);
    initializeProject2();
    disableProject2();
  }

  //
  // initialize project 3
  //
  if (projectsToInitialize & 0x04)
  {
    enableProject3();
    delay(150);
    initializeProject3();
    disableProject3();
  }

  //
  // initialize project 4
  //
  if (projectsToInitialize & 0x08)
  {
    enableProject4();
    delay(150);
    initializeProject4();
    disableProject4();
  }

}



//
// run a project given the project number
//  Enter:  projectNumber = the number of the project to run (1 to 5)
//          projectDatabyte = optional data sent to project, typically 0
//
void runProject(byte projectNumber, byte projectDatabyte)
{
  //
  // check if running project 1
  //
  if (projectNumber == 1)
  {
    enableProject1();
    delay(150);
    initializeProject1();
    runProject1(projectDatabyte);
    disableProject1();
  }

  //
  // check if running project 2
  //
  if (projectNumber == 2)
  {
    enableProject2();
    delay(150);
    initializeProject2();
    runProject2(projectDatabyte);
    disableProject2();
  }

  //
  // check if running project 3
  //
  if (projectNumber == 3)
  {
    enableProject3();
    delay(150);
    initializeProject3();
    runProject3(projectDatabyte);
    disableProject3();
  }

  //
  // check if running project 4
  //
  if (projectNumber == 4)
  {
    enableProject4();
    delay(150);
    initializeProject4();
    runProject4(projectDatabyte);
    disableProject4();
  }
}




// -------------------------------------- End --------------------------------------







