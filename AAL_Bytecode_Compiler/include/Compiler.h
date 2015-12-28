#ifndef COMPILER_H
#define COMPILER_H

#include <string.h>
#include <vector>
#include <list>
#include <iostream>
#include <stack>
#include <algorithm>

#include "commonFuncs.h"
#include "TokenTypes.h"
#include "SyntaxValidator.h"
#include "Token.h"
#include "Tokenizer.h"
#include "PreProcessor.h"
#include "Parser.h"
#include "Optimizer.h"
#include "OneWayHasher.h"

class Compiler
{
    public:
        Compiler();
        virtual ~Compiler();
        bool compile(std::string code,const std::string& destFile, const std::string& includeDir);
        void renameVars(Token* currToken,std::map<std::string,int>& renamedVars,int& cRenamedVars);
        std::string getNextTmpVar();
        void compileTokenRecursive(Token* currToken,std::ofstream& outFile,std::vector<std::string>& compiledLines, bool useVector,const std::string& parentAssignVar,int& funcDeclParam);
        bool validArgumentCount(Token* currToken);
        std::string getAssignVarForToken(Token* currToken,const std::string& str);
        std::string getRightSideForToken(Token* currToken);
        void addLine(const std::string& str,std::ofstream& outFile, std::vector<std::string>& compiledLines, bool useVector);
        void generateTmpVars();
        void setVarUnused(const std::string& var);
        void generateTmpPointer();
        void setPointerUnused(const std::string& var);
        void addSavedLines(std::ofstream& outFile,std::vector<std::string>& compiledLines, bool useVector);
		bool isCompileable(Token* currToken);
        std::string getNextTmpPointer();
		void extractFunctions(Token* currToken,std::map<std::string,std::string>& renamedFuncs);
		void renameFunctions(Token* currToken,std::map<std::string,std::string>& renamedFuncs);
		void writeUsingDirectives(std::ofstream& outFile);

        Parser parser;
        std::vector<std::string> jmpLabels;
		std::string jmpEndLoopLabel;
        std::vector<std::vector<std::string>> savedLines;
    private:
        std::list<std::string> unusedVars;
        std::list<std::string> unusedPointer;
        std::vector<std::vector<std::string>> varsToFree;

        int lastTmpVar=0;
        int lastTmpPointer=0;
        long long lastKeywordNum=0;

        std::string STR_JUMP_ON_TRUE="jt";
        std::string STR_JUMP_ON_FALSE="jf";
        std::string STR_JUMP="j";

        std::string STR_END_WHILE_LABEL;
        std::string STR_START_WHILE_LABEL;
        std::string STR_END_AND_FALSE_LABEL;
        std::string STR_END_AND_TRUE_LABEL;
        std::string STR_END_OR_FALSE_LABEL;
        std::string STR_END_OR_TRUE_LABEL;
        std::string STR_END_TERNARY_LABEL;
        std::string STR_TERNARY_FALSE_LABEL;
        std::string STR_START_DO_LABEL;
        std::string STR_END_IF_LABEL;
        std::string STR_NEXT_IF_LABEL;
		std::string STR_END_LOOP_LABEL;
		std::string STR_END_FUNC_LABEL;

		const std::string STR_USING_DIRCETIVE="using";

		OneWayHasher hasher;
		FunctionAndClassChecker definedStandardFunctions;
};

#endif // COMPILER_H
