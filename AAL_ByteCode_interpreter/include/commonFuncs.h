/**
* \brief Common function collection
*/

#ifndef COMMONFUNCS_H_INCLUDED
#define COMMONFUNCS_H_INCLUDED

#include "defines.h"
#include "TokenTypes.h"
#include "AalVariable.h"

#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#if USE_TO_STRING_PATCH == 1
    // may be necessary for old (buggy) mingw compiler
    template < typename T > std::string toString( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
#else
    template < typename T > std::string toString( const T& n )
    {
        return std::to_string(n);
    }
#endif

template < typename T > std::string toCompressedString( const T& n )
{
    const static char chars[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	std::string buf;
	T quotient = n;
	do
    {
		buf += chars[ (unsigned int) abs( quotient % 62 ) ];
		quotient /=  62;
	} while ( quotient );

    if ( n < 0) buf += '-';
	std::reverse( buf.begin(), buf.end() );

	return buf;
}

bool isTypeVar(const char type);
bool isValidDouble(const std::string& str);
bool isNumberDouble(const std::string& number);
bool lowerTest (char l, char r);
std::string readStringFile(const std::string& fileName);
int stringFindIncase(std::string& text, std::string& toSearch);
std::string convertCharVectorToStringHex(const std::vector<char>&);
std::string convertCharArrayToStringHex(char* data, int lData);
bool iequals(const std::string& a, const std::string& b);
bool isValidWordString(const std::string& str);
std::string getTokenName(TokenTypes token);
bool isValidLong(const std::string& str);
int getPrecedence(TokenTypes token);
void runWaitUntilFinished(LPCTSTR lpApplicationName,LPTSTR lpCommandLine);
std::string generateRandomString(const int len);

#endif // COMMONFUNCS_H_INCLUDED
