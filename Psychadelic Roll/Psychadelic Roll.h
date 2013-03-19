#pragma once

/********************************* Forward Declarations *************************************/
void ballUpdate(Object * const self);
void keyAct();
void keyPressed(unsigned char key, int x, int y);
void keyReleased(unsigned char key, int x, int y);
void initialize(int iArgc, char ** cppArgv);
int main(int iArgc, char ** cppArgv);
/******************************* End Forward Declarations ***********************************/

char * objectIDs = new char[stage.MAX_OBJECTS];

bool keys[256] = { false };
