# Lawny Arduino Code

Arduino IDE Instructions:

Download the arduino IDE here: https://www.arduino.cc/en/software
Goto File->Preferences and change the "Sketchbook location" path to point to the "embedded/lawny" directory.
Which tells the arduino IDE where to look for 3rd party library files (which are included in this project)

VsCode:
Repeat the same steps to change "Sketchbook location" as Vscode just directly references arduino IDE's functionality.
Download the "C++ Extension pack" and "Auduino Extension" and open the "embedded folder", 
The c_cpp_properties.json file should already make the include paths work.

Easiest workflow IMO, is to use VsCode to develop, and the arduino IDE to program/debug.