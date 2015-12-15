// class to parse Bytecode, this class assumes that there are no errors in Bytecode (no syntax/semantic validation)

#ifndef BYTECODEPARSER_H
#define BYTECODEPARSER_H

#include "TokenTypes.h"
#include "commonFuncs.h"
#include <stdlib.h>
#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "defines.h"

enum LineTypes
{
    LINE_ASSIGN, LINE_PUSH, LINE_POP, LINE_LABEL, LINE_JMP,
    LINE_TRUE_JMP, LINE_FALSE_JMP, LINE_COMMAND, LINE_ENDPARAM,
    LINE_RET,LINE_USING, LINE_UNKNOWN
};

struct BytecodeLine
{
	LineTypes type=LINE_UNKNOWN;
	std::string assignVar="";
	TokenTypes commandType=TOK_UNKNOWN;
	std::string arg1="";
	std::string arg2="";
};

class BytecodeParser
{
    public:
        BytecodeParser();
        virtual ~BytecodeParser();

        void parseBytecode(const std::string& code,bool useVector=true);
		void printParsedLines();
		void printJmpLabels();
		void convertToVector();
		void writeToFile(const std::string& fileName,bool useVector=true);
		std::string generateBytecode(BytecodeLine* currLine);

		std::list<BytecodeLine*> parsedLines;
		std::vector<BytecodeLine*> parsedLinesVector;
		std::map<std::string, int> jmpLabels;
    private:


};

#endif // BYTECODEPARSER_H
