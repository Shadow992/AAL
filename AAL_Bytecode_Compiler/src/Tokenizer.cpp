#include "Tokenizer.h"

Tokenizer::Tokenizer(const std::string& code)
{
    tokenizeString(code);
}

/** \brief Function which speeds up creating a new token by preallocation
 * normally you use something like thta
 * Token* newToken=new Token;
 * now you use
 * Token* newToken=createNewToken();
 *
 * \param
 * \param
 *
 */
Token* Tokenizer::createNewToken()
{
    currTokenBuffer++;
    if(currTokenBuffer<=0 || currTokenBuffer>=maximumPreAllocation)
    {
        tokenBuffer=new Token[maximumPreAllocation];
        if(tokenBuffer==nullptr)
        {
            std::cout<<"Internal Error: tokenBuffer is NULL"<<std::endl;
        }
        currTokenBuffer=0;
        tokensToFree.push_back(tokenBuffer);
    }
    return &(tokenBuffer[currTokenBuffer]);
}

/** \brief This function must be called to destroy Tokenizer correctly.
 *
 * \param
 * \param
 *
 */
void Tokenizer::cleanUp()
{
    for(auto it=tokensToFree.begin(); it!=tokensToFree.end(); ++it)
    {
        delete[] (*it);
    }
    currTokenBuffer=-1;
    tokensToFree.clear();
}

/** \brief This function splits the give code into tokens.
 *
 * \param
 * \param
 *
 */
bool Tokenizer::tokenizeString(const std::string& code)
{
    currTokenBuffer=-1;
    bool singleQuote=false;
    bool doubleQuote=false;
    bool inComment=false;
    int lastAllocated=8192;
    bool escapeCharFound=false;

    char currChar;
    char *currValue=(char*) malloc(lastAllocated);
    int idxChar=0;

    int lCode=code.length();
    Token* currToken=createNewToken();
    int i=0;

    for(; i<lCode; i++)
    {
        currChar=code[i];

        if(inComment==false)
        {

            if(currChar=='\\' && (singleQuote==true || doubleQuote==true))
            {
                escapeCharFound=true;
                currValue[idxChar]=currChar;
                idxChar++;
                if(idxChar>=lastAllocated)
                {
                    lastAllocated+=8192;
                    currValue=(char*) realloc(currValue,lastAllocated);
                }
            }
            else if(escapeCharFound==true)
            {
                escapeCharFound=false;
                currValue[idxChar]=currChar;

                idxChar++;
                if(idxChar>=lastAllocated)
                {
                    lastAllocated+=8192;
                    currValue=(char*) realloc(currValue,lastAllocated);
                }
            }
            else
            {

                if(currChar=='\'' && doubleQuote==false)
                {
                    if(singleQuote==true)
                    {
                        singleQuote=false;
                    }
                    else
                    {
                        singleQuote=true;
                    }
                }
                else if(currChar=='\"' && singleQuote==false)
                {
                    if(doubleQuote==true)
                    {
                        doubleQuote=false;
                    }
                    else
                    {
                        doubleQuote=true;
                    }
                }

                if(currChar == ';')
                {
                    inComment=true;
                }

                if(singleQuote==false && doubleQuote==false && isSplittingChar(currChar))
                {
                    if(idxChar!=0)
                    {
                        currValue[idxChar]='\0';
                        tokens.push_back(currToken);
                        currToken->setToken(currValue);
                        idxChar=0;
                        currToken=createNewToken();
                    }
                    if(!isWSChar(currChar) && inComment==false)
                    {
                        currValue[idxChar]=currChar;
                        currValue[idxChar+1]='\0';
                        tokens.push_back(currToken);
                        currToken->setToken(currValue);
                        idxChar=0;
                        currToken=createNewToken();
                    }
                }
                else
                {
                    currValue[idxChar]=currChar;
                    idxChar++;
                    if(idxChar>=lastAllocated)
                    {
                        lastAllocated+=8192;
                        currValue=(char*) realloc(currValue,lastAllocated);
                    }
                }

            }

        }
        else if(currChar == '\n' && inComment==true)
        {
            inComment=false;
            idxChar=0;
            i--;
        }
    }

    if(idxChar!=0)
    {
        currValue[idxChar]='\0';
        tokens.push_back(currToken);
        currToken->setToken(currValue);
        currToken=createNewToken();
    }

    // Add some End-Tokens to avoid ifs later on

    currToken->type=TOK_END;
    tokens.push_back(currToken);

    currToken=createNewToken();
    currToken->type=TOK_END;
    tokens.push_back(currToken);
    free(currValue);
    return true;
}

/** \brief This function checks if character is a WS-Char.
 *
 * \param
 * \param
 *
 */
bool Tokenizer::isWSChar(char chr)
{
    switch (chr)
    {
    case ' ':
    case '\t':
    case '\r':
        return true;
    default:
        return false;
    }
}

/** \brief This function checks if character is a delimiter to split at.
 *
 * \param
 * \param
 *
 */
bool Tokenizer::isSplittingChar(char chr)
{
    switch (chr)
    {
    case ' ':
    case '\t':
    case'\r':
    case'\n':
    case'?':
    case':':
    case';':
    case'.':
    case',':
    case'<':
    case'>':
    case'=':
    case'+':
    case'-':
    case'*':
    case'/':
    case'^':
    case'&':
    case'|':
    case'(':
    case')':
    case'[':
    case']':
    case'{':
    case'}':
    case'%':
    case'!':
        return true;
    default:
        return false;
    }
}
