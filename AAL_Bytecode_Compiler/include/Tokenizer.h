#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include "TokenTypes.h"
#include "commonFuncs.h"
#include "Token.h"
#include <list>
#include <stdlib.h>

class Tokenizer
{
	private:
        const int maximumPreAllocation=4096;

	public:
		Tokenizer(const std::string& code);
		Tokenizer(){};
		~Tokenizer(){cleanUp();};
		bool tokenizeString(const std::string&);
		bool isSplittingToken(char chr);
		bool isWSChar(char chr);
        bool isSplittingChar(char chr);
        Token* createNewToken();
        void cleanUp();

        Token* tokenBuffer;
        int currTokenBuffer=-1;
		std::list<Token*> tokens;
		std::vector<Token*> tokensToFree;
};

#endif // TOKENIZER_H_INCLUDED
