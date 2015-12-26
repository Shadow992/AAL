#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ClassPrototypes.h"
#include "TokenTypes.h"
#include "commonFuncs.h"
#include "defines.h"
#include "BytecodeParser.h"
#include "VariableManagement.h"
#include "UserInterface.h"
#include "StandardFunctions.h"

#include <map>
#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include <map>

#include "AalVariable.h"
#include "AllocationHelper.h"
#include "OneWayHasher.h"

#include <chrono>



const std::string STR_PARAM="@param";
const std::string STR_FUNC_PREFIX="_func_";
const std::string STR_E_ONEWAYHASHER="e";
const std::string STR_N_ONEWAYHASHER="n";

/**
* \brief Class used for interpreting AAL bytecode
*/
class Interpreter
{
public:
    Interpreter(const std::string&);
    virtual ~Interpreter();

    void interpretCommand(BytecodeLine* bytecode);
    void interpretLine(BytecodeLine* bytecode);
    void interpretAllLines();
    void extractValuesForOperations(BytecodeLine* bytecode, char& type, double& tmpDouble1, double& tmpDouble2, long long& tmpLong1, long long& tmpLong2);
    void extractLongsForOperations(BytecodeLine* bytecode, long long& tmpLong1, long long& tmpLong2);
    void extractStringsForOperations(BytecodeLine* bytecode, std::string& tmpString1, std::string& tmpString2);
    void extractDoublesForOperations(BytecodeLine* bytecode, double& tmpDouble1, double& tmpDouble2);
	void extractValues(BytecodeLine* bytecode,char& type,double& tmpDouble1, double& tmpDouble2,long long& tmpLong1, long long& tmpLong2, std::string& tmpString1, std::string& tmpString2);
    void extractLongsForOperations(BytecodeLine* bytecode, long long& tmpLong1, const bool getArg1=true);
	void addToVariableAsParameter(AalVariable* assignVar, BytecodeLine* bytecode);
    bool callUserFunction(AalVariable* assignVar, BytecodeLine* bytecode);
    bool callStandardFunction(AalVariable* returnVar, BytecodeLine* bytecode);

    UserInterface* ui;
private:
    VariableManagement varManager;
    StandardFunctions* aalStandardFunctions;

    std::vector<AalVariable*> returnValues;

    std::string fileName;
    BytecodeParser parser;

    int currLine=0;
    AalVariable* tmpArg1Var;
    AalVariable* tmpArg2Var;
    int currHierarchyLevel=-1;

    char privateClassType=TYPE_LONG;
    double privateClassTmpDouble1;
    double privateClassTmpDouble2;
    long long privateClassTmpLong1;
    long long privateClassTmpLong2;
    std::string privateClassTmpString1;
    std::string privateClassTmpString2;

    OneWayHasher hasher;
};



#endif // INTERPRETER_H
