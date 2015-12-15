#ifndef SYNTAXVALIDATOR_INCLUDED
#define SYNTAXVALIDATOR_INCLUDED

#include "FunctionAndClassChecker.h"
#include "Token.h"
#include "TokenTypes.h"
#include "ProblemTypes.h"
#include "commonFuncs.h"
#include <map>
#include <vector>
#include <list>
#include <iostream>

class SyntaxValidator
{
	public:
		SyntaxValidator(){};
        std::vector<Problem> checkSyntax(const std::list<Token*>& tokenList);

};


#endif // SYNTAXVALIDATOR_INCLUDED
