#ifndef VARIABLEMANAGEMENT_H
#define VARIABLEMANAGEMENT_H

#include "TokenTypes.h"
#include "commonFuncs.h"
#include "defines.h"
#include "BytecodeParser.h"
#include "Hashtable.h"
#include "AalVariable.h"
#include "AllocationHelper.h"

#include <map>
#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <chrono>

/**
* \brief Class which handles hierarchy changes and variable creation/deletion
*/
class VariableManagement
{
public:
    VariableManagement();
    virtual ~VariableManagement();

    AalVariable* getVar(const std::string& str, const char type, bool createNewVar=false);
    AalVariable* getVar(unsigned long long tmp, const char type, bool createNewVar=false);
    void enterNextHierarchyLevel();
    void leaveCurrentHierarchyLevel();
    void enterFunction(const int currLine);
    void leaveFunction(int& currLine);
    void printVars();

private:
    std::vector<AalVariable*> globalVars;
	std::vector<AalVariable*> generatedPtrVars;
	std::vector<AalVariable*> generatedTmpVars;

    std::vector<std::vector<Hashtable<AalVariable>*>*> hierachyVars;
    std::vector<long long> informationStack;

    int currFunctionDepth=0;
    int currHierarchyLevel=-1;
};



#endif // VARIABLEMANAGEMENT_H
