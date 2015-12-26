#ifndef STANDARDFUNCTIONS_H
#define STANDARDFUNCTIONS_H

#include "ClassPrototypes.h"
#include "AalVariable.h"
#include "BytecodeParser.h"
#include "Image.h"
#include "AllocationHelper.h"
#include "InternalClassWrapper.h"
#include "Interpreter.h"
#include "Control.h"
#include "Window.h"
#include "UserInterface.h"
#include <map>
#include <time.h>
#include <algorithm>
#include <chrono>


typedef int (*AalFunction)(AalVariable* parameterVar, AalVariable* returnVar, Interpreter* currInterpreter); // function pointer type

// AAL-Functions in C/C++
int clickLeftBackground(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int consoleRead(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int consoleWrite(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int timer(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int Ubound(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int aalSleep(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);

int searchForPixelNextToPosition(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int searchForImageNextToPosition(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int searchForSimilarImage(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int searchForImage(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int searchForPixel(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int setPixelColor(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int getPixelColor(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int loadImageFromBMP(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int captureWindowByName(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int saveImage(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int captureWindow(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int getWindowHandle(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);

int stringContains(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int charAt(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int stringSubstring(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int stringLength(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);

int guiCreateWindow(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiCreateButton(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiCreateCheckbox(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiCreateLabel(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiCreateInputbox(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiSetOnEvent(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiReadText(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiSetText(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int guiIsChecked(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);

// Debugging functions
int debug_PrintVariableRecursive(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);
int debug_ShowArrayInfo(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);

/**
* \brief This class is used for executing standard AAL-Functions.
*/
class StandardFunctions
{
public:
    StandardFunctions();
    virtual ~StandardFunctions();
    bool executeFunction(std::string functionName, AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter);


protected:
private:
    std::map<std::string,AalFunction> functionMap;

};

#endif // STANDARDFUNCTIONS_H
