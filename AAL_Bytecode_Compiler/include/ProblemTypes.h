#ifndef PROBLEM_TYPES_H_INCLUDED
#define PROBLEM_TYPES_H_INCLUDED

#include "TokenTypes.h"
#include "Token.h"
#include <map>
#include <list>

enum ProblemTypes
{
	//Errors
	ERROR_UNKNOWN, ERROR_MISSING_CLOSING_BRACKETS, ERROR_TOO_MANY_CLOSING_BRACKETS, ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN, ERROR_TOKEN_NOT_ALLOWED_HERE,
	ERROR_MISSING_ENDIF,ERROR_MISSING_WEND,ERROR_MISSING_ENDSWITCH,ERROR_MISSING_ENDSELECT,ERROR_MISSING_ENDWITH,ERROR_MISSING_NEXT, ERROR_MISSING_ENDFUNC,
	ERROR_MISSING_ENDKEYWORD, ERROR_UNUSED_ENDKEYWORD, ERROR_NOT_MATCHING_ENDKEYWORD, ERROR_NOT_MATCHING_CLOSING_BRACKET, ERROR_MISSING_OPEN_BRACKETS,
	ERROR_MISSING_UNTIL,ERROR_NOT_POSSIBLE_FUNCTIONNAME,ERROR_TOKEN_OUTSIDE_OF_IF, ERROR_MORE_THAN_ONE_VARDECL_NOT_ALLOWED,ERROR_ROUND_OPEN_BRACKET_OR_DOT_EXPECTED,
	ERROR_TOKEN_OUTSIDE_OF_FUNC,ERROR_TOKEN_OUTSIDE_OF_LOOP,ERROR_NOT_POSSIBLE_CLASSNAME,ERROR_UNKNOWN_CLASS, ERROR_UNKNOWN_FUNCTION,



	//Warnings
	WARNING_REMOVABLE_STATEMENT
};

enum ImportanceType{IMPORTANCE_INFO, IMPORTANCE_WARNING, IMPORTANCE_ERROR};

struct Problem
{
	ProblemTypes type;
	Token* currToken;
	Token* lastToken;
    ImportanceType importance;

	Problem(ProblemTypes typeIn,Token* currTokenIn,Token* lastTokenIn,ImportanceType importanceIn=IMPORTANCE_ERROR)
	{
		type=typeIn;
		currToken=currTokenIn;
		lastToken=lastTokenIn;
		importance=importanceIn;
	}
};

#endif // PROBLEM_TYPES_H_INCLUDED
