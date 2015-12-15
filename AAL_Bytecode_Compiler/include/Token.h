#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED

#include "TokenTypes.h"
#include <map>
#include <list>
#include <string>

class Token
{
	public:
		TokenTypes type=TOK_UNKNOWN;
		std::string tokenValue;
		std::list<Token*> childTokens;
		int currLine;
		bool processed=false;

		Token(TokenTypes,const std::string&);
		Token(){};
		void setToken(const std::string& value);
		void setToken(TokenTypes,const std::string& value);
		void updateGeneralTokenType(const std::string& value);
		TokenTypes getKeyword(const std::string& value);
};


#endif // TOKEN_H_INCLUDED
