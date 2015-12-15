#include "BytecodeParser.h"

BytecodeParser::BytecodeParser()
{

}

BytecodeParser::~BytecodeParser()
{
    //dtor
    for(auto it=parsedLinesVector.begin();it!=parsedLinesVector.end();++it)
    {
        delete (*it);
    }

    for(auto it=parsedLines.begin();it!=parsedLines.end();++it)
    {
        delete (*it);
    }

}

/**
* \brief  This method prints all found jump labels and the corresponding line (mainly used for debugging).
*/
void BytecodeParser::printJmpLabels()
{
    for(auto it=jmpLabels.begin(); it!=jmpLabels.end(); ++it)
    {
        std::cout<<it->first<<": "<<it->second<<std::endl;
    }
}

/**
* \brief  This method prints all bytecode lines parsed (mainly used for debugging, may be quite inaccurate).
*/
void BytecodeParser::printParsedLines()
{
    BytecodeLine* currLine;
    for(auto it=parsedLines.begin(); it!=parsedLines.end(); ++it)
    {
        currLine=*it;
        if(currLine->assignVar!="")
        {
            if(currLine->type!=LINE_ASSIGN)
            {
                std::cout<<currLine->type<<" "<<currLine->assignVar<<"="<<(currLine->commandType)<<" "<<currLine->arg1<<" "<<currLine->arg2<<std::endl;
            }
            else
            {
                std::cout<<currLine->type<<" "<<currLine->assignVar<<"="<<" "<<currLine->arg1<<std::endl;
            }
        }
        else
        {
            std::cout<<currLine->type<<" "<<currLine->arg1<<" "<<currLine->arg2<<std::endl;
        }
    }
}

/**
* \brief  This method converts the internal std::list<BytecodeLine*> to std::vector<BytecodeLine*>.
*/
void BytecodeParser::convertToVector()
{
    parsedLinesVector.reserve(parsedLines.size());
    for(auto it=parsedLines.begin(); it!=parsedLines.end(); ++it)
    {
        parsedLinesVector.push_back(parsedLines.front());
        parsedLines.pop_front();
    }
}

/**
* \brief This method sets the type and values corresponding to extractedValue (mainly used internal).
*
* \param extractedValue extracted value of Bytecode (in)
* \param isString determines if extracted value is a string (in)
* \param extractedType will hold the type which was extracted (e.g. TYPE_VARIABLE, TYPE_PTR_VARIABLE, TYPE_DOUBLE, ...) (out)
* \param extractedStr will hold the extracted value as string (mostly used when extractedValue could not bee converted)  (out)
* \param extractedLL will hold the extracted value as long long (out)
* \param extractedD will hold the extracted value as double (out)
*
*/
void BytecodeParser::setTypeSpecificValues(const std::string& extractedValue, bool isString, char& extractedType,std::string& extractedStr, long long& extractedLL, double& extractedD)
{
	if(isString==false && extractedValue[0]=='$')
	{
		extractedLL=stoll(extractedValue.substr(2));
		if(extractedValue[1]=='1')
		{
			extractedType=TYPE_VARIABLE;
		}
		else if(extractedValue[1]=='2')
		{
			extractedType=TYPE_TMP_VARIABLE;
		}
		else if(extractedValue[1]=='3')
		{
			extractedType=TYPE_PTR_VARIABLE;
		}
	}
	else if(isString==false && extractedValue[0]=='*')
	{
		extractedLL=stoll(extractedValue.substr(3));
		if(extractedValue[2]=='1')
		{
			extractedType=TYPE_PTR_VARIABLE;
		}
		else if(extractedValue[2]=='2')
		{
			extractedType=TYPE_PTR_VARIABLE;
		}
		else if(extractedValue[2]=='3')
		{
			extractedType=TYPE_PTR_VARIABLE;
		}
	}
	else if(isValidLong(extractedValue))
	{
		extractedType=TYPE_LONG;
		extractedLL=stoll(extractedValue);
		extractedD=extractedLL;
	}
	else if(isValidDouble(extractedValue))
	{
		extractedType=TYPE_DOUBLE;
		extractedD=stod(extractedValue);
		extractedLL=extractedD;
	}
	else if(extractedValue[0]=='@')
	{
        extractedStr=extractedValue;
        extractedType=TYPE_MACRO;
	}
	else
	{
		extractedStr=extractedValue;
		extractedType=TYPE_STRING;
	}
}

/**
* \brief This method parses given Bytecode and saves it into some member variables, also saves found jump-labels into jmpLabels-map.
*
* \param code Bytecode to parse
* \param useVector if true, parsed Bytecode will be saved in std::vector, otherwise it will be saved in std::list (for different algorithms you may need list or vector to speed code up)
*
*/
void BytecodeParser::parseBytecode(const std::string& code, bool useVector)
{
#if SHOW_TIMINGS == 1
    auto start = std::chrono::steady_clock::now();
#endif

    int lCode=code.length();
    char currChar;
    bool doubleQuote=false;
    bool singleQuote=false;
    bool isString=false;

    std::string extractedValue;
    int numberOfCurrentToken=0;
    BytecodeLine* currLine=new BytecodeLine;
    int lineNumber=0;
    int i=0;

    for(; i<lCode; i++)
    {
        //std::cout<<code[i];
        currChar=code[i];
        if(currChar=='"' && singleQuote==false)
		{
			doubleQuote=!doubleQuote;
			isString=true;
		}
		else if(currChar=='\'' && doubleQuote==false)
		{
			singleQuote=!singleQuote;
			isString=true;
		}
        else if(singleQuote==false && doubleQuote==false)
        {
            if(currChar=='\r')
                continue;
            if(currChar=='^')
            {
                currLine->type=LINE_PUSH;
            }
            else if(currChar=='!')
            {
                currLine->type=LINE_POP;
            }
            else if(currChar==' ' || currChar=='\n' || currChar=='\r' || currChar=='=')
            {
                if(numberOfCurrentToken==0)
                {
                    if(extractedValue.find(":")!=std::string::npos)
                    {
                        // we found jmp_label
                        currLine->type=LINE_LABEL;
                        extractedValue.pop_back();
                        currLine->arg1=extractedValue;
                        jmpLabels[extractedValue]=lineNumber;
                    }
                    else if(extractedValue=="j")
                    {
                        // we found jmp command
                        currLine->type=LINE_JMP;
                    }
                    else if(extractedValue=="jt")
                    {
                        // we found jmp on true command
                        currLine->type=LINE_TRUE_JMP;
                    }
                    else if(extractedValue=="jf")
                    {
                        // we found jmp on false command
                        currLine->type=LINE_FALSE_JMP;
                    }
                    else if(extractedValue=="ep")
                    {
                        currLine->type=LINE_ENDPARAM;
                    }
                    else if(extractedValue=="ret")
                    {
                        currLine->type=LINE_RET;
                    }
					else if(extractedValue=="using")
					{
						currLine->type=LINE_USING;
					}
                    else
                    {
						double filler;
						setTypeSpecificValues(extractedValue, isString, currLine->assignVarType,currLine->assignVar,currLine->assignVarLong,filler);
						if(extractedValue.length() > 0 && extractedValue[0]=='*')
                        {
                            currLine->assignVarIsPointer=true;
                        }
                        else
                        {
                            currLine->assignVarIsPointer=false;
                        }
                    }
                }
                else if(currLine->type==LINE_TRUE_JMP || currLine->type==LINE_FALSE_JMP)
                {
                    if(numberOfCurrentToken==1)
                    {
                        // extract condition
                        setTypeSpecificValues(extractedValue, isString, currLine->arg1Type,currLine->arg1,currLine->arg1Long,currLine->arg1Double);
                    }
                    else if(numberOfCurrentToken==2)
                    {
                        // extract label to jmp to
                        setTypeSpecificValues(extractedValue, isString, currLine->arg2Type,currLine->arg2,currLine->arg2Long,currLine->arg2Double);
                    }
                }
                else if(currLine->type==LINE_JMP)
                {
                    if(numberOfCurrentToken==1)
                    {
                        // extract label to jmp to
						setTypeSpecificValues(extractedValue, isString, currLine->arg1Type,currLine->arg1,currLine->arg1Long,currLine->arg1Double);
                    }
                }
                else if(currLine->type==LINE_RET)
                {
                    if(numberOfCurrentToken==1)
                    {
                        // extract return value
                        setTypeSpecificValues(extractedValue, isString, currLine->arg1Type,currLine->arg1,currLine->arg1Long,currLine->arg1Double);
                    }
                }
				else if(currLine->type==LINE_USING)
				{
					if(numberOfCurrentToken==1)
                    {
						currLine->arg1=extractedValue;
					}
					else if(numberOfCurrentToken==2)
                    {
						currLine->arg2=extractedValue;
					}
				}
                else
                {
                    if(numberOfCurrentToken==1 && (currChar=='\n' || currChar=='\r'))
                    {
                        currLine->type=LINE_ASSIGN;
                        setTypeSpecificValues(extractedValue, isString, currLine->arg1Type,currLine->arg1,currLine->arg1Long,currLine->arg1Double);
                    }
                    else if(numberOfCurrentToken==1)
                    {
                        currLine->type=LINE_COMMAND;
                        currLine->commandType=(TokenTypes)std::stoi(extractedValue);
                    }
                    else if(numberOfCurrentToken==2)
                    {
                        setTypeSpecificValues(extractedValue, isString, currLine->arg1Type,currLine->arg1,currLine->arg1Long,currLine->arg1Double);
                    }
                    else if(numberOfCurrentToken==3)
                    {
                        setTypeSpecificValues(extractedValue, isString, currLine->arg2Type,currLine->arg2,currLine->arg2Long,currLine->arg2Double);
                    }
                }

                extractedValue="";
                isString=false;

                if(currChar=='\n' || currChar=='\r')
                {
                    if(currLine->type!=LINE_LABEL || (currLine->arg1)[0]=='_')
                    {
                        lineNumber++;
                    }

                    if(useVector==true)
                    {
                        parsedLinesVector.push_back(currLine);
                    }
                    else
                    {
                        parsedLines.push_back(currLine);
                    }
                    currLine=new BytecodeLine;
                    numberOfCurrentToken=0;
                }
                else
                {
                    numberOfCurrentToken++;
                }
            }
            else
            {
                extractedValue+=currChar;
            }
        }
		else if(currChar=='\\')
		{
			i++;
			if(i<lCode)
            {
                if(code[i]=='n')
                {
                    extractedValue+='\n';
                }
                else if(code[i]=='t')
                {
                    extractedValue+='\t';
                }
                else if(code[i]=='r')
                {
                    extractedValue+='\r';
                }
                else
                {
                    extractedValue+=currChar;
                    extractedValue+=code[i];
                }

            }

		}
        else
        {
            extractedValue+=currChar;
        }
    }

    delete currLine;

#if SHOW_TIMINGS == 1
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Parsing Bytecode: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
    start = std::chrono::steady_clock::now();
#endif
}

