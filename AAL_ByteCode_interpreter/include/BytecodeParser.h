#ifndef BYTECODEPARSER_H
#define BYTECODEPARSER_H

#include "TokenTypes.h"
#include "commonFuncs.h"
#include "defines.h"
#include "AalVariable.h"

#include <map>
#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>

enum LineTypes
{
    LINE_ASSIGN, LINE_PUSH, LINE_POP, LINE_LABEL, LINE_JMP,
    LINE_TRUE_JMP, LINE_FALSE_JMP, LINE_COMMAND, LINE_ENDPARAM,
    LINE_RET, LINE_USING, LINE_UNKNOWN
};

/**
* \brief This struct is holding information of exactly one line of bytecode
*/
struct BytecodeLine
{
	LineTypes type=LINE_UNKNOWN; /**< This variable holds type of bytecode line */
	std::string assignVar=""; /**< This variable holds the assign variable (so the left side of "=") as string (only filled if variable/value can not be converted to long or double) */
	TokenTypes commandType=TOK_UNKNOWN; /**< This variable holds the type of command if line type is LINE_COMMAND (e.g. TOK_OP_MOD) */
	std::string arg1=""; /**< This variable holds the first argument as string (only filled if variable/value can not be converted to long or double) */
	std::string arg2=""; /**< This variable holds the first argument as string (only filled if variable/value can not be converted to long or double) */
	long long arg1Long; /**< This variable holds arg1 as long */
	long long arg2Long; /**< This variable holds arg2 as long */
    double arg1Double; /**< This variable holds arg1 as double */
	double arg2Double; /**< This variable holds arg2 as double */
	long long assignVarLong; /**< This variable holds the assign variable (so the left side of "=") as long */
	char arg1Type=TYPE_UNKNOWN; /**< This variable holds the type of arg1 (e.g. TYPE_VARIABLE, TYPE_LONG, ...) */
	char arg2Type=TYPE_UNKNOWN; /**< This variable holds the type of arg2 (e.g. TYPE_VARIABLE, TYPE_LONG, ...) */
	char assignVarType=TYPE_UNKNOWN; /**< This variable holds the type of assignVar (e.g. TYPE_VARIABLE, TYPE_LONG, ...) */
	bool assignVarIsPointer=false; /**< This variable determines if the assign var is a pointer (=true) (e.g. *$123=... or *$32=... etc.) or not (=false) */
};

/**
* \brief Class to parse Bytecode, this class assumes that there are no errors in Bytecode (no syntax/semantic validation)
*/
class BytecodeParser
{
    public:
        BytecodeParser();
        virtual ~BytecodeParser();

        void parseBytecode(const std::string& code,bool useVector=true);
		void printParsedLines();
		void printJmpLabels();
		void convertToVector();
        void setTypeSpecificValues(const std::string& extractedValue, bool isString, char& extractedType,std::string& extractedStr, long long& extractedLL, double& extractedD);

		std::list<BytecodeLine*> parsedLines; /**< Holding all parsed lines as list of BytecodeLine-Pointers */
		std::vector<BytecodeLine*> parsedLinesVector; /**< Holding all parsed lines as vector of BytecodeLine-Pointers */
		std::map<std::string, int> jmpLabels; /**< Holding all found jmpLabels */
    private:
};

#endif // BYTECODEPARSER_H
