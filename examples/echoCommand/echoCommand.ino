/******************************************************************
  @file       echoCommand.ino
  @brief      Send commands from the x-IMU3 GUI back as Notifications
  @author     David Such
  @copyright  Please see the accompanying LICENSE file

  Code:        David Such
  Version:     1.0.0
  Date:        13/04/23

  1.0.0     Original Release.       13/04/23

  Dependency - Requires that the Reefwing_imuTypes Library is also
               installed.

******************************************************************/

#include "ThetisAPI.h"

void setup() {
  //  Start Serial and wait for connection
  Serial.begin(115200);
  while (!Serial);
  api.begin(&Serial);

  api.sendNotification("Arduino Connected");
}

void loop() {
  api.checkForCommand();
  if (api.newCommand()) {
    char *cmdPtr = api.getCommand();

    //  Echo Command to x-IMU3 GUI
    api.sendNotification(cmdPtr);
  }
}