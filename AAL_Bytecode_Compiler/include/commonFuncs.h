#ifndef COMMONFUNCS_H_INCLUDED
#define COMMONFUNCS_H_INCLUDED

#include "defines.h"
#include <string.h>
#include "TokenTypes.h"
#include "SyntaxValidator.h"
#include "ProblemTypes.h"
#include <sstream>
#include <iomanip>
#include <fstream>

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
        #if defined(USE_ABS_LONG_LONG_PATCH)
        T result=quotient % ((T)62);
        buf += chars[ (unsigned int) (result < 0 ? -result : result) ];
        #else
		buf += chars[ (unsigned int) abs( quotient % ((T)62) ) ];
		#endif // defined
		quotient /=  (T)62;
	} while ( quotient );

    if ( n < 0) buf += '-';
	std::reverse( buf.begin(), buf.end() );

	return buf;
}

bool isRelativelyPrime(unsigned long long a, unsigned long long b);
unsigned long long log2(unsigned long long val);
std::string addQuotes(const std::string& str);
bool isNumberDouble(const std::string& number);
bool lowerTest (char l, char r);
std::string readStringFile(const std::string& fileName);
int stringFindIncase(std::string& text, std::string& toSearch);
std::string convertCharVectorToStringHex(const std::vector<char>&);
std::string convertCharArrayToStringHex(char* data, int lData);
std::string getProblemName(Problem prob);
bool iequals(const std::string& a, const std::string& b);
bool isValidWordString(const std::string& str);
std::string getTokenName(TokenTypes token);
bool isValidNumber(const std::string& str);
int getPrecedence(TokenTypes token);
std::string getCorrespondingToken(Token* tok);
std::string getProblemMessage(const Problem& prob);
void runWaitUntilFinished(LPCTSTR lpApplicationName,LPTSTR lpCommandLine);
void printSyntaxTree(Token* currToken, int depth);
void eraseChildTokens(Token* currToken);
std::string generateRandomString(const int len);

#endif // COMMONFUNCS_H_INCLUDED
