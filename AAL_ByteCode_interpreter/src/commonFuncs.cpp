#include "commonFuncs.h"

bool isRelativelyPrime(unsigned long long a, unsigned long long b)
{
   return (a<b) ? isRelativelyPrime(b,a) : !(a%b) ? (b==1LL) : isRelativelyPrime (b, a%b);
}

/**
* \brief This function checks if given type is some sort of variable (TYPE_PTR_VARIABLE or TYPE_TMP_VARIABLE or TYPE_VARIABLE).
*
* \param type Type to check
*
* \return true if variable, otherwise false
*/
bool isTypeVar(const char type)
{
    return type==TYPE_PTR_VARIABLE || type==TYPE_TMP_VARIABLE || type==TYPE_VARIABLE;
}

/**
* \brief This function generates a random string containing only characters "a-z" and "A-Z".
*
* \param len Length of string that should be generated
*
* \return generated string
*/
std::string generateRandomString(const int len)
{
    std::string str;
    const static char alpha[] ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < len; ++i)
    {
        str+=alpha[rand() % (sizeof(alpha) - 1)];
    }

    return str;
}

/**
* \brief This function checks if a string (which is a number) is a double number.
*
* \param number String to check
*
* \return true if number is a double number, otherwise false
*/
bool isNumberDouble(const std::string& number)
{
	if(number.find('.')!=std::string::npos)
	{
		return true;
	}
	return false;
}

/**
* \brief Helper-Function for stringFindIncase, lowercase compare.
*/
bool lowerTest (char l, char r)
{
	return (std::tolower(l) == std::tolower(r));
}

/**
* \brief This function searches given string for another string (incase sensitive).
*
* \param text String to search through
* \param toSearch String that should be searched for
*
* \return position of found text or -1 if text was not found
*/
int stringFindIncase(std::string& text, std::string& toSearch)
{
	std::string::iterator fpos = std::search(text.begin(), text.end(), toSearch.begin(), toSearch.end(), lowerTest);
	if (fpos != text.end())
	{
		return std::distance(text.begin(), fpos);
	}
	return -1;
}

/**
* \brief This function reads a file as string, if file does not exist it returns an empty string.
*
* \param fileName Name of file to read
*
* \return read text
*/
std::string readStringFile(const std::string& fileName)
{
	std::ifstream t(fileName);
	std::string str="";

	t.seekg(0, std::ios::end);
	int lRead=t.tellg();
	if(lRead>0)
	{
		str.reserve(t.tellg());
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
	}
	return str;
}

/**
* \brief This function converts a char vector to hex string.
*
* \param data Char vector to convert
*
* \return converted string
*/
std::string convertCharVectorToStringHex(const std::vector<char>& data)
{
	int lData=data.size();
	std::stringstream ss;
	for(int i=0; i<lData; ++i)
	{
        if(data[i]<16)
            ss << '0';
		ss << std::hex << (int)((unsigned char)data[i]);
	}
	return ss.str();
}

/**
* \brief This function converts a char array to hex string.
*
* \param data Char array to convert
* \param lData Length of char array
*
* \return converted string
*/
std::string convertCharArrayToStringHex(char* data, int lData)
{
	std::stringstream ss;
	for(int i=0; i<lData; ++i)
	{
	    if(data[i]<16)
            ss << '0';
		ss << std::hex << (int)((unsigned char)data[i]);
	}
	return ss.str();
}

/**
* \brief This function executes a program and waits until it finished.
*
* \param lpApplicationName name of application to start
* \param lpCommandLine Command-Line arguments
*/
void runWaitUntilFinished(LPCTSTR lpApplicationName,LPTSTR lpCommandLine)
{
#if ONLY_PARSE == 0
   // additional information
   STARTUPINFO si;
   PROCESS_INFORMATION pi;

   // set the size of the structures
   ZeroMemory( &si, sizeof(si) );
   si.cb = sizeof(si);
   ZeroMemory( &pi, sizeof(pi) );

  // start the program up
  CreateProcess( lpApplicationName,   // the path
    lpCommandLine,  // Command line
    NULL,           // Process handle not inheritable
    NULL,           // Thread handle not inheritable
    FALSE,          // Set handle inheritance to FALSE
    0,              // No creation flags
    NULL,           // Use parent's environment block
    NULL,           // Use parent's starting directory
    &si,            // Pointer to STARTUPINFO structure
    &pi );           // Pointer to PROCESS_INFORMATION structure

	// Wait for finish
	WaitForSingleObject( pi.hProcess, INFINITE );
    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
#endif
}

/**
* \brief Function to check if two strings are equal (incase-sensitive).
*
* \param a first string to check
* \param b second string to check
*
* \return true if equal, false otherwise
*/
bool iequals(const std::string& a, const std::string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (std::tolower(a[i]) != std::tolower(b[i]))
            return false;
    return true;
}

/**
* \brief This function checks if a given string is a valid word string, so if string consists only of "A-Z", "a-z", "0-9" and "_".
*
* \param str string to check
*
* \return true if it is a valid word string, otherwise false
*/
bool isValidWordString(const std::string& str)
{
    int lStr=str.length();
    if(lStr==0)
        return false;

	char currChar;

    for(int i=0;i<lStr;i++)
    {
        currChar=str[i];
        if((currChar<'a' || currChar>'z') && (currChar<'A' || currChar>'Z') &&
            (currChar<'0' || currChar>'9') && currChar != '_')
            return false;
    }
    return true;
}

/**
* \brief This function checks if string is a valid long number (can be binary, hexadecimal or decimal long number).
*        Binary numbers (e.g.): 0b11111001010
*        Hexadecimal numbers (e.g.): 0xA276BF
*
* \param str string to check
*
* \return true if it is a valid long, otherwise false
*/
bool isValidLong(const std::string& str)
{
    int lStr=str.length();
    if(lStr==0)
        return false;

	char currChar;

	 if(lStr>1)
	 {
		if(str[1]=='x' || str[1]=='X')
		{
			if(str[0]!='0')
				return false;
			for(int i=2;i<lStr;i++)
			{
				currChar=str[i];
				if((currChar<'0' || currChar>'9') && (currChar<'A' || currChar>'F') && (currChar<'a' || currChar>'f') )
					return false;
			}
		}
		else if(str[1]=='b' || str[1]=='B')
		{
			if(str[0]!='0')
				return false;
			for(int i=2;i<lStr;i++)
			{
				currChar=str[i];
				if(currChar<'0' || currChar>'1')
					return false;
			}
		}
		else
		{
			for(int i=0;i<lStr;i++)
			{
				currChar=str[i];
				if(currChar<'0' || currChar>'9')
					return false;
			}
		}
	}
	else
	{
		for(int i=0;i<lStr;i++)
		{
			currChar=str[i];
			if(currChar<'0' || currChar>'9')
				return false;
		}
	}
    return true;
}

/**
* \brief This function checks if a given string is a valid double.
*
* \param str string to check
*
* \return true if it is a valid double, otherwise false
*
*/
bool isValidDouble(const std::string& str)
{
    int lStr=str.length();
    if(lStr==0 || lStr==1)
        return false;

	char currChar;
    bool dotNotFound=true;

    for(int i=0;i<lStr;i++)
    {
        currChar=str[i];
        if(currChar<'0' || currChar>'9')
        {
            if(currChar=='.' && dotNotFound==true)
            {
                dotNotFound=false;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

/**
* \brief This function returns precedence of a given token. Precedence is used to determine which token must be calculated first.
*        The higher the precedence is the earlier it must be evaluated.
*
* \param token token get precedence for
*
* \return Precedence-Number
*
*/
int getPrecedence(TokenTypes token)
{
    switch(token)
    {
        case TOK_OP_NOT: return 14;
        case TOK_PUNCT_NOT:  return 17;
        case TOK_OP_MOD: return 17;
        case TOK_OP_ADD: return 16;
        case TOK_OP_SUB: return 16;
        case TOK_OP_MUL: return 17;
        case TOK_OP_DIV: return 17;
        case TOK_OP_SHL: return 17;
        case TOK_OP_SHR: return 17;
        case TOK_OP_JOIN: return 17;
        case TOK_OP_POW: return 18;
        case TOK_OP_INC: return 18;
        case TOK_OP_DEC: return 18;
        case TOK_ASSIGN:
        case TOK_ASSIGN_MOD:
        case TOK_ASSIGN_ADD:
        case TOK_ASSIGN_SUB:
        case TOK_ASSIGN_MUL:
        case TOK_ASSIGN_DIV:
        case TOK_ASSIGN_SHL:
        case TOK_ASSIGN_SHR:
        case TOK_ASSIGN_JOIN:
        case TOK_ASSIGN_POW:
        case TOK_DEFAULT_ASSIGN:
            return 9;

		case TOK_BRACKET_ARRAY_CREATION:
		case TOK_BRACKET_ARRAY_ACCESS:
		case TOK_BYREF:
			return 19;
		case TOK_FUNC:
			return 20;


        case TOK_COMP_EQCASE:
        case TOK_COMP_EQINCASE:
        case TOK_COMP_LOWEREQ:
        case TOK_COMP_GREATEREQ:
        case TOK_COMP_NOTEQ:
        case TOK_COMP_LESS:
        case TOK_COMP_GREATER:
            return 15;

        case TOK_CASE:
        case TOK_IF:
        case TOK_WHILE:
        case TOK_UNTIL:
        case TOK_FOR:
        case TOK_TO:
        case TOK_CLASS:
        case TOK_STEP:
        case TOK_ELSEIF:
            return 9;

        case TOK_LOGIC_AND:  return 13;
        case TOK_LOGIC_OR:  return 12;

        case TOK_VARDECL:
        case TOK_FUNCDECL:
        case TOK_CONST:
        case TOK_SYNCHRONIZED:
        case TOK_ENUM:
        case TOK_STATIC:
        case TOK_VOLATILE:
            return 8;

        case TOK_RETURN:
            return 9;

        case TOK_NEW:
            return 19;

        case TOK_PREPROCESS:
            return 19;

        case TOK_PUNCT_COMMENT:  return 15;
        case TOK_PUNCT_DOT:  return 19;
        case TOK_PUNCT_COMMA:  return 8;
        case TOK_PUNCT_QST:  return 10;
        case TOK_PUNCT_DOUBLEDOT:  return 11;

        case TOK_NEWLINE: return 0;
        case TOK_END: return 0;
		default: return -1;
    }

    return -1;
}

/**
* \brief get TokenType token as string (only used for debugging, may not contain all tokens)
*
* \param token token to return as string
*
* \return token as string
*
*/
std::string getTokenName(TokenTypes token)
{
    switch(token)
    {
        case TOK_IF: return "TOK_IF";
        case TOK_SWITCH: return "TOK_SWITCH";
        case TOK_SELECT: return "TOK_SELECT";
        case TOK_ELSE: return "TOK_ELSE";
        case TOK_ELSEIF: return "TOK_ELSEIF";
        case TOK_THEN: return "TOK_THEN";
        case TOK_FOR: return "TOK_FOR";
        case TOK_WHILE: return "TOK_WHILE";
        case TOK_DO: return "TOK_DO";
        case TOK_FUNCDECL: return "TOK_FUNCDECL";
        case TOK_FUNC: return "TOK_FUNC";
        case TOK_PREPROCESS: return "TOK_PREPROCESS";
        case TOK_VARDECL: return "TOK_VARDECL";
        case TOK_VAR: return "TOK_VAR";
        case TOK_LITERAL: return "TOK_LITERAL";
        case TOK_PREDEFINEDVAR: return "TOK_PREDEFINEDVAR";
        case TOK_RETURN: return "TOK_RETURN";
        case TOK_CONTINUE: return "TOK_CONTINUE";
        case TOK_DEFAULT: return "TOK_DEFAULT";
        case TOK_GEN_EXIT: return "TOK_GEN_EXIT";
        case TOK_CASE: return "TOK_CASE";
        case TOK_STATIC: return "TOK_STATIC";
        case TOK_VOLATILE: return "TOK_VOLATILE";
        case TOK_CONST: return "TOK_CONST";
        case TOK_WITH: return "TOK_WITH";
        case TOK_ENDWITH: return "TOK_ENDWITH";
        case TOK_ENDIF: return "TOK_ENDIF";
        case TOK_ENDSELECT: return "TOK_ENDSELECT";
        case TOK_ENDSWITCH: return "TOK_ENDSWITCH";
        case TOK_TO: return "TOK_TO";
        case TOK_STEP: return "TOK_STEP";
        case TOK_NULL: return "TOK_NULL";
        case TOK_UNTIL: return "TOK_UNTIL";
        case TOK_NEXT: return "TOK_NEXT";
        case TOK_WEND: return "TOK_WEND";
        case TOK_BRACKET_OPEN: return "TOK_BRACKET_OPEN";
        case TOK_BRACKET_CLOSE: return "TOK_BRACKET_CLOSE";
        case TOK_ENDFUNC: return "TOK_ENDFUNC";
        case TOK_UNKNOWN: return "TOK_UNKNOWN";
        case TOK_OP_NOT: return "TOK_OP_NOT";
        case TOK_OP_MOD: return "TOK_OP_MOD";
        case TOK_OP_ADD: return "TOK_OP_ADD";
        case TOK_OP_SUB: return "TOK_OP_SUB";
        case TOK_OP_MUL: return "TOK_OP_MUL";
        case TOK_OP_DIV: return "TOK_OP_DIV";
        case TOK_OP_SHL: return "TOK_OP_SHL";
        case TOK_OP_SHR: return "TOK_OP_SHR";
        case TOK_OP_JOIN: return "TOK_OP_JOIN";
        case TOK_OP_POW: return "TOK_OP_POW";
        case TOK_OP_INC: return "TOK_OP_INC";
        case TOK_OP_DEC: return "TOK_OP_DEC";
        case TOK_ASSIGN: return "TOK_ASSIGN";
        case TOK_ASSIGN_MOD: return "TOK_ASSIGN_MOD";
        case TOK_ASSIGN_ADD: return "TOK_ASSIGN_ADD";
        case TOK_ASSIGN_SUB: return "TOK_ASSIGN_SUB";
        case TOK_ASSIGN_MUL: return "TOK_ASSIGN_MUL";
        case TOK_ASSIGN_DIV: return "TOK_ASSIGN_DIV";
        case TOK_ASSIGN_SHL: return "TOK_ASSIGN_SHL";
        case TOK_ASSIGN_SHR:  return "TOK_ASSIGN_SHR";
        case TOK_ASSIGN_JOIN:  return "TOK_ASSIGN_JOIN";
        case TOK_ASSIGN_POW:  return "TOK_ASSIGN_POW";
        case TOK_COMP_EQCASE:  return "TOK_COMP_EQCASE";
        case TOK_COMP_EQINCASE:  return "TOK_COMP_EQINCASE";
        case TOK_COMP_LOWEREQ:  return "TOK_COMP_LOWEREQ";
        case TOK_COMP_GREATEREQ:  return "TOK_COMP_GREATEREQ";
        case TOK_COMP_NOTEQ:  return "TOK_COMP_NOTEQ";
        case TOK_COMP_LESS:  return "TOK_COMP_LESS";
        case TOK_COMP_GREATER:  return "TOK_COMP_GREATER";
        case TOK_LOGIC_AND:  return "TOK_LOGIC_AND";
        case TOK_LOGIC_OR:  return "TOK_LOGIC_OR";
        case TOK_PUNCT_COMMENT:  return "TOK_PUNCT_COMMENT";
        case TOK_PUNCT_DOT:  return "TOK_PUNCT_DOT";
        case TOK_PUNCT_COMMA:  return "TOK_PUNCT_COMMA";
        case TOK_PUNCT_QST:  return "TOK_PUNCT_QST";
        case TOK_PUNCT_DOUBLEDOT:  return "TOK_PUNCT_DOUBLEDOT";
        case TOK_NUMBER_HEX:  return "TOK_NUMBER_HEX";
        case TOK_NUMBER_DEC:  return "TOK_NUMBER_DEC";
        case TOK_NUMBER_BIN:  return "TOK_NUMBER_BIN";
        case TOK_EXIT_LOOP:  return "TOK_EXIT_LOOP";
        case TOK_TRUE:  return "TOK_TRUE";
        case TOK_FALSE:  return "TOK_FALSE";
        case TOK_BRACKET_ROUND_OPEN: return "TOK_BRACKET_ROUND_OPEN";
        case TOK_BRACKET_ANGLE_OPEN:  return "TOK_BRACKET_ANGLE_OPEN";
        case TOK_BRACKET_ARRAY_OPEN:  return "TOK_BRACKET_ARRAY_OPEN";
        case TOK_BRACKET_ROUND_CLOSE:  return "TOK_BRACKET_ROUND_CLOSE";
        case TOK_PUNCT_NOT:  return "TOK_PUNCT_NOT";
        case TOK_BRACKET_ANGLE_CLOSE:  return "TOK_BRACKET_ANGLE_CLOSE";
        case TOK_BRACKET_ARRAY_CLOSE:  return "TOK_BRACKET_ARRAY_CLOSE";
        case TOK_NEWLINE: return "TOK_NEWLINE";
        case TOK_END: return "TOK_END";
		case TOK_DEFAULT_ASSIGN: return "TOK_DEFAULT_ASSIGN";
		case TOK_ROOT: return "TOK_ROOT";
		case TOK_BRACKET_ARRAY_ACCESS: return "TOK_BRACKET_ARRAY_ACCESS";
		case TOK_SYNCHRONIZED: return "TOK_SYNCHRONIZED";
		case TOK_NEW: return "TOK_NEW";
		case TOK_CLASS: return "TOK_CLASS";
		case TOK_ENDCLASS: return "TOK_ENDCLASS";
		case TOK_BRACKET_ARRAY_CREATION: return "TOK_BRACKET_ARRAY_CREATION";
		default: return "";
    }

    return "";
}
