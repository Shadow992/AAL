#ifndef STANDARDFUNCTIONS_H
#define STANDARDFUNCTIONS_H

#include "AalVariable.h"
#include "BytecodeParser.h"
#include "Image.h"
#include "AllocationHelper.h"
#include <map>
#include <time.h>
#include <algorithm>
#include <chrono>

typedef int (*AalFunction)(AalVariable* parameterVar, AalVariable* returnVar); // function pointer type

// AAL-Functions in C/C++
int clickLeftBackground(AalVariable* parameterVar, AalVariable* returnVar);
int consoleRead(AalVariable* parameterVar, AalVariable* returnVar);
int consoleWrite(AalVariable* parameterVar, AalVariable* returnVar);
int timer(AalVariable* parameterVar, AalVariable* returnVar);
int Ubound(AalVariable* parameterVar, AalVariable* returnVar);
int searchForPixelNextToPosition(AalVariable* parameterVar, AalVariable* returnVar);
int searchForImageNextToPosition(AalVariable* parameterVar, AalVariable* returnVar);
int searchForSimilarImage(AalVariable* parameterVar, AalVariable* returnVar);
int searchForImage(AalVariable* parameterVar, AalVariable* returnVar);
int searchForPixel(AalVariable* parameterVar, AalVariable* returnVar);
int setPixelColor(AalVariable* parameterVar, AalVariable* returnVar);
int getPixelColor(AalVariable* parameterVar, AalVariable* returnVar);
int loadImageFromBMP(AalVariable* parameterVar, AalVariable* returnVar);
int captureWindowByName(AalVariable* parameterVar, AalVariable* returnVar);
int charAt(AalVariable* parameterVar, AalVariable* returnVar);
int stringSubstring(AalVariable* parameterVar, AalVariable* returnVar);
int stringLength(AalVariable* parameterVar, AalVariable* returnVar);
int saveImage(AalVariable* parameterVar, AalVariable* returnVar);
int captureWindow(AalVariable* parameterVar, AalVariable* returnVar);
int getWindowHandle(AalVariable* parameterVar, AalVariable* returnVar);
int stringContains(AalVariable* parameterVar, AalVariable* returnVar);

// Debugging functions
int debug_PrintVariableRecursive(AalVariable* parameterVar, AalVariable* returnVar);
int debug_ShowArrayInfo(AalVariable* parameterVar, AalVariable* returnVar);

/**
* \brief This class is used for executing standard AAL-Functions.
*/
class StandardFunctions
{
    public:
        StandardFunctions();
        virtual ~StandardFunctions();
        bool executeFunction(std::string functionName, AalVariable* parameterVar, AalVariable* returnVar);
    protected:
    private:
        std::map<std::string,AalFunction> functionMap;

};

#endif // STANDARDFUNCTIONS_H
