#ifndef PARSER_H
#define PARSER_H

#include <string.h>
#include <vector>
#include <list>
#include <iostream>
#include <stack>

#include "defines.h"
#include "commonFuncs.h"
#include "TokenTypes.h"
#include "SyntaxValidator.h"
#include "Token.h"
#include "Tokenizer.h"
#include "PreProcessor.h"

class Parser
{
    public:
        Parser(){};
        Parser(const std::string& code);
        virtual ~Parser();
        void updateSpecificTokenType(std::list<Token*>& );
        bool parse(const std::string& code);
		void createSyntaxTree(Token* currToken);
		int getChildTokenPrecedence(Token* currChildToken);
        void createSyntaxTreeThreaded(std::list<Token*>::iterator, int cThreads);

		Token rootToken;
		Tokenizer tokenizer;
    private:
        int const THREAD_COUNT=4;
};

#endif // PARSER_H
