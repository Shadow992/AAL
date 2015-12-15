#include "commonFuncs.h"


bool isRelativelyPrime(unsigned long long a, unsigned long long b)
{
   return (a<b) ? isRelativelyPrime(b,a) : !(a%b) ? (b==1LL) : isRelativelyPrime (b, a%b);
}

unsigned long long log2 (unsigned long long val)
{
    if (val <= 1)
        return 0;

    unsigned int ret = 0;
    while (val > 1)
    {
        val >>= 1;
        ret++;
    }
    return ret;
}

std::string addQuotes(const std::string& str)
{
    std::string retStr="\"";
    int lStr=str.length();
    char currChar;
    for(int i=0; i<lStr; i++)
    {
        currChar=str[i];
        if(currChar=='"')
        {
            retStr+='\\';
        }
        retStr+=currChar;
    }
    return retStr+"\"";
}

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

void eraseChildTokens(Token* currToken)
{
    Token* currChildToken;

    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        currChildToken=*it;
        eraseChildTokens(currChildToken);
        delete currChildToken;
    }
}

// this function prints the created syntax tree
void printSyntaxTree(Token* currToken, int depth)
{
    Token* currChildToken;

    for(int i=0; i<depth; i++)
    {
        std::cout<<"-";
    }
    std::cout<<" "<<currToken->tokenValue<<" ("<<currToken->childTokens.size()<<") ["<<getTokenName(currToken->type)<<"] "<<std::endl;

    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        currChildToken=*it;
        printSyntaxTree(currChildToken,depth+1);
    }
}

// checks if a string is a double number
bool isNumberDouble(const std::string& number)
{
    if(number.find('.')!=std::string::npos)
    {
        return true;
    }
    return false;
}

// helper function for stringFindIncase, lowercase compare
bool lowerTest (char l, char r)
{
    return (std::tolower(l) == std::tolower(r));
}

// searches given string text for another string toSearch (incase sensitive), returns position of found text or -1 if text was not found
int stringFindIncase(std::string& text, std::string& toSearch)
{
    std::string::iterator fpos = std::search(text.begin(), text.end(), toSearch.begin(), toSearch.end(), lowerTest);
    if (fpos != text.end())
    {
        return std::distance(text.begin(), fpos);
    }
    return -1;
}

// this function reads a file as string, if file does not exist it returns an empty string
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

// executes a program and waits until it finished
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

// this function returns the corresponding end/start token to a given token
std::string getCorrespondingToken(Token* tok)
{
    switch(tok->type)
    {
    case TOK_ENDWITH:
        return "With";
    case TOK_ENDIF:
        return "If";
    case TOK_ENDSELECT:
        return "Select";
    case TOK_ENDSWITCH:
        return "Switch";
    case TOK_NEXT:
        return "For";
    case TOK_WEND:
        return "While";
    case TOK_ENDFUNC:
        return "Func";
    case TOK_WITH:
        return "EndWith";
    case TOK_IF:
    case TOK_ELSE:
    case TOK_ELSEIF:
        return "EndIf";
    case TOK_SELECT:
        return "EndSelect";
    case TOK_SWITCH:
        return "EndSwitch";
    case TOK_FOR:
        return "Next";
    case TOK_WHILE:
        return "WEnd";
    case TOK_FUNC:
        return "EndFunc";
    case TOK_CLASS:
        return "EndClass";
    case TOK_ENDCLASS:
        return "Class";
    case TOK_BRACKET_ROUND_OPEN:
        return ")";
    case TOK_BRACKET_ANGLE_OPEN:
        return ">";
    case TOK_BRACKET_ARRAY_OPEN:
        return "]";
    case TOK_BRACKET_OPEN:
        return "}";
    case TOK_BRACKET_ROUND_CLOSE:
        return "(";
    case TOK_BRACKET_ANGLE_CLOSE:
        return "<";
    case TOK_BRACKET_ARRAY_CLOSE:
        return "[";
    case TOK_BRACKET_CLOSE:
        return "{";
    case TOK_PUNCT_QST:
        return ":";
    case TOK_PUNCT_DOUBLEDOT:
        return "?";
    default:
        return "";
    }
}

// this function returns a human readable text for a specific found problem
std::string getProblemMessage(const Problem& prob)
{
    std::string currTokenInfo="n.a.";
    std::string lastTokenInfo="n.a.";
    if(prob.currToken!=nullptr)
    {
        if(prob.currToken->tokenValue.length()>100)
        {
            std::string tmp=prob.currToken->tokenValue.substr(0,100)+"...";
            currTokenInfo="'"+tmp+std::string("' [Line ")+toString(prob.currToken->currLine)+std::string("]");
        }
        else
        {
            currTokenInfo="'"+(prob.currToken->tokenValue)+std::string("' [Line ")+toString(prob.currToken->currLine)+std::string("]");
        }
    }
    if(prob.lastToken!=nullptr)
    {
        if(prob.lastToken->tokenValue.length()>100)
        {
            std::string tmp=(prob.lastToken->tokenValue).substr(0,100)+"...";
            lastTokenInfo="'"+tmp+std::string("' [Line ")+toString(prob.currToken->currLine)+std::string("]");
        }
        else
        {
            lastTokenInfo="'"+(prob.lastToken->tokenValue)+std::string("' [Line ")+toString(prob.lastToken->currLine)+std::string("]");
        }

    }


    switch(prob.type)
    {
    case ERROR_UNKNOWN:
        return "An unknown error occured with "+currTokenInfo+" and "+lastTokenInfo;
    case ERROR_MISSING_CLOSING_BRACKETS:
        return "There is no closing bracket for "+currTokenInfo;
    case ERROR_MISSING_ENDKEYWORD:
        return "There is no closing '"+getCorrespondingToken(prob.currToken)+"' for "+currTokenInfo;
    case ERROR_UNUSED_ENDKEYWORD:
        return currTokenInfo+" does not have a corresponding '"+getCorrespondingToken(prob.currToken)+"'";
    case ERROR_NOT_MATCHING_ENDKEYWORD:
        return "Found "+lastTokenInfo+" but expected '"+getCorrespondingToken(prob.currToken)+"' (last found keyword was "+currTokenInfo+")";
    case ERROR_NOT_MATCHING_CLOSING_BRACKET:
        return "Found "+lastTokenInfo+" but expected '"+getCorrespondingToken(prob.currToken)+"' (last found bracket was "+currTokenInfo+")";
    case ERROR_MISSING_OPEN_BRACKETS:
        return lastTokenInfo+" does not have a corresponding '"+getCorrespondingToken(prob.lastToken)+"'";
    case ERROR_NOT_POSSIBLE_FUNCTIONNAME:
        return "Invalid functionname "+currTokenInfo+". Functionname must only consist of A-Z, a-z, 0-9 and _";
    case ERROR_NOT_POSSIBLE_CLASSNAME:
        return "Invalid classname "+currTokenInfo+". Classname must only consist of A-Z, a-z, 0-9 and _";
    case ERROR_TOKEN_OUTSIDE_OF_IF:
        return lastTokenInfo+" is not allowed outside of if.";
    case ERROR_MORE_THAN_ONE_VARDECL_NOT_ALLOWED:
        return lastTokenInfo+" is not allowed when variable already have got an other declaration keyword (problematic keywords are Global, Local, Dim and ReDim)";
    case ERROR_ROUND_OPEN_BRACKET_OR_DOT_EXPECTED:
        return "There must be a '(' or '.' after "+lastTokenInfo+" but found "+currTokenInfo;
    case ERROR_TOKEN_OUTSIDE_OF_FUNC:
        return lastTokenInfo+" is not allowed outside of 'Func'";
    case ERROR_TOKEN_OUTSIDE_OF_LOOP:
        return lastTokenInfo+" is not allowed outside of a loop (loops are While, For and Do)";
    case ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN:
        return lastTokenInfo+" can not be followed by "+currTokenInfo;
    case ERROR_TOKEN_NOT_ALLOWED_HERE:
        switch(prob.lastToken->type)
        {
        case TOK_CASE:
            return lastTokenInfo+" is only allowed in Switch/Select but found in "+currTokenInfo;
        case TOK_ELSEIF:
            return lastTokenInfo+" is only allowed after If/ElseIf but found after "+currTokenInfo;
        case TOK_THEN:
            return lastTokenInfo+" is only allowed after If/ElseIf but found after "+currTokenInfo;
        default:
            return lastTokenInfo+" is not allowed in this line/at this position";
        }
    case WARNING_REMOVABLE_STATEMENT:
        return "At least one "+lastTokenInfo+" can be removed without any problems";
    case ERROR_UNKNOWN_CLASS:
        return "The class "+currTokenInfo+" were not defined";
    case ERROR_UNKNOWN_FUNCTION:
        return "The function "+lastTokenInfo+" were not defined";
    case ERROR_TOO_MANY_CLOSING_BRACKETS:
    case ERROR_MISSING_ENDIF:
    case ERROR_MISSING_WEND:
    case ERROR_MISSING_ENDSWITCH:
    case ERROR_MISSING_ENDSELECT:
    case ERROR_MISSING_ENDWITH:
    case ERROR_MISSING_NEXT:
    case ERROR_MISSING_UNTIL:
    case ERROR_MISSING_ENDFUNC:
        return "n.a";
    }
    return "Unknown Error";
}

// function to check if two strings are equal (incasesensitive compare)
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

// this function checks if a given string is a valid word string, so if string consists only of A-Z, a-z, 0-9 and _
bool isValidWordString(const std::string& str)
{
    int lStr=str.length();
    if(lStr==0)
        return false;

    char currChar;

    for(int i=0; i<lStr; i++)
    {
        currChar=str[i];
        if((currChar<'a' || currChar>'z') && (currChar<'A' || currChar>'Z') &&
                (currChar<'0' || currChar>'9') && currChar != '_')
            return false;
    }
    return true;
}

// this function checks if string is a valid number (can be binary, hexadecimal or decimal number)
// Binary numbers (e.g.): 0b11111001010
// Hexadecimal numbers (e.g.): 0xA276BF
bool isValidNumber(const std::string& str)
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
            for(int i=2; i<lStr; i++)
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
            for(int i=2; i<lStr; i++)
            {
                currChar=str[i];
                if(currChar<'0' || currChar>'1')
                    return false;
            }
        }
        else
        {
            for(int i=0; i<lStr; i++)
            {
                currChar=str[i];
                if(currChar<'0' || currChar>'9')
                    return false;
            }
        }
    }
    else
    {
        for(int i=0; i<lStr; i++)
        {
            currChar=str[i];
            if(currChar<'0' || currChar>'9')
                return false;
        }
    }
    return true;
}

// function returns precedence of a given token
// precedence is used to determine which token must be calculated first
// the higher the precedence is the earlier it must be evaluated
int getPrecedence(TokenTypes token)
{
    switch(token)
    {
    case TOK_OP_NOT:
        return 14;
    case TOK_PUNCT_NOT:
        return 17;
    case TOK_OP_MOD:
        return 17;
    case TOK_OP_ADD:
        return 16;
    case TOK_OP_SUB:
        return 16;
    case TOK_OP_MUL:
        return 17;
    case TOK_OP_DIV:
        return 17;
    case TOK_OP_SHL:
        return 17;
    case TOK_OP_SHR:
        return 17;
    case TOK_OP_JOIN:
        return 17;
    case TOK_OP_POW:
        return 18;
    case TOK_OP_INC:
        return 18;
    case TOK_OP_DEC:
        return 18;
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
    case TOK_BRACKET_ARRAY_APPEND:
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

    case TOK_LOGIC_AND:
        return 13;
    case TOK_LOGIC_OR:
        return 12;

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

    case TOK_PUNCT_COMMENT:
        return 15;
    case TOK_PUNCT_DOT:
        return 19;
    case TOK_PUNCT_COMMA:
        return 8;
    case TOK_PUNCT_QST:
        return 10;
    case TOK_PUNCT_DOUBLEDOT:
        return 11;

    case TOK_NEWLINE:
        return 0;
    case TOK_END:
        return 0;
    default:
        return -1;
    }

    return -1;
}

// get TokenType token as string (only used for debugging)
std::string getTokenName(TokenTypes token)
{
    switch(token)
    {
    case TOK_IF:
        return "TOK_IF";
    case TOK_SWITCH:
        return "TOK_SWITCH";
    case TOK_SELECT:
        return "TOK_SELECT";
    case TOK_ELSE:
        return "TOK_ELSE";
    case TOK_ELSEIF:
        return "TOK_ELSEIF";
    case TOK_THEN:
        return "TOK_THEN";
    case TOK_FOR:
        return "TOK_FOR";
    case TOK_WHILE:
        return "TOK_WHILE";
    case TOK_DO:
        return "TOK_DO";
    case TOK_FUNCDECL:
        return "TOK_FUNCDECL";
    case TOK_FUNC:
        return "TOK_FUNC";
    case TOK_PREPROCESS:
        return "TOK_PREPROCESS";
    case TOK_VARDECL:
        return "TOK_VARDECL";
    case TOK_VAR:
        return "TOK_VAR";
    case TOK_LITERAL:
        return "TOK_LITERAL";
    case TOK_PREDEFINEDVAR:
        return "TOK_PREDEFINEDVAR";
    case TOK_RETURN:
        return "TOK_RETURN";
    case TOK_CONTINUE:
        return "TOK_CONTINUE";
    case TOK_DEFAULT:
        return "TOK_DEFAULT";
    case TOK_GEN_EXIT:
        return "TOK_GEN_EXIT";
    case TOK_CASE:
        return "TOK_CASE";
    case TOK_STATIC:
        return "TOK_STATIC";
    case TOK_VOLATILE:
        return "TOK_VOLATILE";
    case TOK_CONST:
        return "TOK_CONST";
    case TOK_WITH:
        return "TOK_WITH";
    case TOK_ENDWITH:
        return "TOK_ENDWITH";
    case TOK_ENDIF:
        return "TOK_ENDIF";
    case TOK_ENDSELECT:
        return "TOK_ENDSELECT";
    case TOK_ENDSWITCH:
        return "TOK_ENDSWITCH";
    case TOK_TO:
        return "TOK_TO";
    case TOK_STEP:
        return "TOK_STEP";
    case TOK_NULL:
        return "TOK_NULL";
    case TOK_UNTIL:
        return "TOK_UNTIL";
    case TOK_NEXT:
        return "TOK_NEXT";
    case TOK_WEND:
        return "TOK_WEND";
    case TOK_BRACKET_OPEN:
        return "TOK_BRACKET_OPEN";
    case TOK_BRACKET_CLOSE:
        return "TOK_BRACKET_CLOSE";
    case TOK_ENDFUNC:
        return "TOK_ENDFUNC";
    case TOK_UNKNOWN:
        return "TOK_UNKNOWN";
    case TOK_OP_NOT:
        return "TOK_OP_NOT";
    case TOK_OP_MOD:
        return "TOK_OP_MOD";
    case TOK_OP_ADD:
        return "TOK_OP_ADD";
    case TOK_OP_SUB:
        return "TOK_OP_SUB";
    case TOK_OP_MUL:
        return "TOK_OP_MUL";
    case TOK_OP_DIV:
        return "TOK_OP_DIV";
    case TOK_OP_SHL:
        return "TOK_OP_SHL";
    case TOK_OP_SHR:
        return "TOK_OP_SHR";
    case TOK_OP_JOIN:
        return "TOK_OP_JOIN";
    case TOK_OP_POW:
        return "TOK_OP_POW";
    case TOK_OP_INC:
        return "TOK_OP_INC";
    case TOK_OP_DEC:
        return "TOK_OP_DEC";
    case TOK_ASSIGN:
        return "TOK_ASSIGN";
    case TOK_ASSIGN_MOD:
        return "TOK_ASSIGN_MOD";
    case TOK_ASSIGN_ADD:
        return "TOK_ASSIGN_ADD";
    case TOK_ASSIGN_SUB:
        return "TOK_ASSIGN_SUB";
    case TOK_ASSIGN_MUL:
        return "TOK_ASSIGN_MUL";
    case TOK_ASSIGN_DIV:
        return "TOK_ASSIGN_DIV";
    case TOK_ASSIGN_SHL:
        return "TOK_ASSIGN_SHL";
    case TOK_ASSIGN_SHR:
        return "TOK_ASSIGN_SHR";
    case TOK_ASSIGN_JOIN:
        return "TOK_ASSIGN_JOIN";
    case TOK_ASSIGN_POW:
        return "TOK_ASSIGN_POW";
    case TOK_COMP_EQCASE:
        return "TOK_COMP_EQCASE";
    case TOK_COMP_EQINCASE:
        return "TOK_COMP_EQINCASE";
    case TOK_COMP_LOWEREQ:
        return "TOK_COMP_LOWEREQ";
    case TOK_COMP_GREATEREQ:
        return "TOK_COMP_GREATEREQ";
    case TOK_COMP_NOTEQ:
        return "TOK_COMP_NOTEQ";
    case TOK_COMP_LESS:
        return "TOK_COMP_LESS";
    case TOK_COMP_GREATER:
        return "TOK_COMP_GREATER";
    case TOK_LOGIC_AND:
        return "TOK_LOGIC_AND";
    case TOK_LOGIC_OR:
        return "TOK_LOGIC_OR";
    case TOK_PUNCT_COMMENT:
        return "TOK_PUNCT_COMMENT";
    case TOK_PUNCT_DOT:
        return "TOK_PUNCT_DOT";
    case TOK_PUNCT_COMMA:
        return "TOK_PUNCT_COMMA";
    case TOK_PUNCT_QST:
        return "TOK_PUNCT_QST";
    case TOK_PUNCT_DOUBLEDOT:
        return "TOK_PUNCT_DOUBLEDOT";
    case TOK_NUMBER_HEX:
        return "TOK_NUMBER_HEX";
    case TOK_NUMBER_DEC:
        return "TOK_NUMBER_DEC";
    case TOK_NUMBER_BIN:
        return "TOK_NUMBER_BIN";
    case TOK_EXIT_LOOP:
        return "TOK_EXIT_LOOP";
    case TOK_TRUE:
        return "TOK_TRUE";
    case TOK_FALSE:
        return "TOK_FALSE";
    case TOK_BRACKET_ROUND_OPEN:
        return "TOK_BRACKET_ROUND_OPEN";
    case TOK_BRACKET_ANGLE_OPEN:
        return "TOK_BRACKET_ANGLE_OPEN";
    case TOK_BRACKET_ARRAY_OPEN:
        return "TOK_BRACKET_ARRAY_OPEN";
    case TOK_BRACKET_ROUND_CLOSE:
        return "TOK_BRACKET_ROUND_CLOSE";
    case TOK_PUNCT_NOT:
        return "TOK_PUNCT_NOT";
    case TOK_BRACKET_ANGLE_CLOSE:
        return "TOK_BRACKET_ANGLE_CLOSE";
    case TOK_BRACKET_ARRAY_CLOSE:
        return "TOK_BRACKET_ARRAY_CLOSE";
    case TOK_NEWLINE:
        return "TOK_NEWLINE";
    case TOK_END:
        return "TOK_END";
    case TOK_DEFAULT_ASSIGN:
        return "TOK_DEFAULT_ASSIGN";
    case TOK_ROOT:
        return "TOK_ROOT";
    case TOK_BRACKET_ARRAY_ACCESS:
        return "TOK_BRACKET_ARRAY_ACCESS";
    case TOK_SYNCHRONIZED:
        return "TOK_SYNCHRONIZED";
    case TOK_NEW:
        return "TOK_NEW";
    case TOK_CLASS:
        return "TOK_CLASS";
    case TOK_ENDCLASS:
        return "TOK_ENDCLASS";
    case TOK_BRACKET_ARRAY_CREATION:
        return "TOK_BRACKET_ARRAY_CREATION";
    case TOK_BRACKET_ARRAY_APPEND:
        return "TOK_BRACKET_ARRAY_APPEND";
    default:
        return "";
    }

    return "";
}

std::string getProblemName(Problem prob)
{
    switch(prob.type)
    {
    case ERROR_UNKNOWN:
        return "ERROR_UNKNOWN";
    case ERROR_MISSING_CLOSING_BRACKETS:
        return "ERROR_MISSING_CLOSING_BRACKETS";
    case ERROR_TOO_MANY_CLOSING_BRACKETS:
        return "ERROR_TOO_MANY_CLOSING_BRACKETS";
    case ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN:
        return "ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN";
    case ERROR_TOKEN_NOT_ALLOWED_HERE:
        return "ERROR_TOKEN_NOT_ALLOWED_HERE";
    case ERROR_MISSING_ENDIF:
        return "ERROR_MISSING_ENDIF";
    case ERROR_MISSING_WEND:
        return "ERROR_MISSING_WEND";
    case ERROR_MISSING_ENDSWITCH:
        return "ERROR_MISSING_ENDSWITCH";
    case ERROR_MISSING_ENDSELECT:
        return "ERROR_MISSING_ENDSELECT";
    case ERROR_MISSING_ENDWITH:
        return "ERROR_MISSING_ENDWITH";
    case ERROR_MISSING_NEXT:
        return "ERROR_MISSING_NEXT";
    case ERROR_MISSING_ENDFUNC:
        return "ERROR_MISSING_ENDFUNC";
    case ERROR_MISSING_ENDKEYWORD:
        return "ERROR_MISSING_ENDKEYWORD";
    case ERROR_UNUSED_ENDKEYWORD:
        return "ERROR_UNUSED_ENDKEYWORD";
    case ERROR_NOT_MATCHING_ENDKEYWORD:
        return "ERROR_NOT_MATCHING_ENDKEYWORD";
    case ERROR_NOT_MATCHING_CLOSING_BRACKET:
        return "ERROR_NOT_MATCHING_CLOSING_BRACKET";
    case ERROR_MISSING_OPEN_BRACKETS:
        return "ERROR_MISSING_OPEN_BRACKETS";
    case ERROR_MISSING_UNTIL:
        return "ERROR_MISSING_UNTIL";
    case ERROR_NOT_POSSIBLE_FUNCTIONNAME:
        return "ERROR_NOT_POSSIBLE_FUNCTIONNAME";
    case ERROR_TOKEN_OUTSIDE_OF_IF:
        return "ERROR_TOKEN_OUTSIDE_OF_IF";
    case ERROR_MORE_THAN_ONE_VARDECL_NOT_ALLOWED:
        return "ERROR_MORE_THAN_ONE_VARDECL_NOT_ALLOWED";
    case ERROR_ROUND_OPEN_BRACKET_OR_DOT_EXPECTED:
        return "ERROR_ROUND_OPEN_BRACKET_OR_DOT_EXPECTED";
    case ERROR_TOKEN_OUTSIDE_OF_FUNC:
        return "ERROR_TOKEN_OUTSIDE_OF_FUNC";
    case ERROR_TOKEN_OUTSIDE_OF_LOOP:
        return "ERROR_TOKEN_OUTSIDE_OF_LOOP";
    case WARNING_REMOVABLE_STATEMENT:
        return "WARNING_REMOVABLE_STATEMENT";
    default:
        return "";
    }
}
