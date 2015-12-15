#include "BytecodeParser.h"

BytecodeParser::BytecodeParser()
{

}

BytecodeParser::~BytecodeParser()
{
    //dtor
}

void BytecodeParser::printJmpLabels()
{
	for(auto it=jmpLabels.begin();it!=jmpLabels.end();++it)
	{
		std::cout<<it->first<<": "<<it->second<<std::endl;
	}
}

void BytecodeParser::printParsedLines()
{
	BytecodeLine* currLine;
	for(auto it=parsedLines.begin();it!=parsedLines.end();++it)
	{
		currLine=*it;
		if(currLine->assignVar!="")
		{
			if(currLine->type!=LINE_ASSIGN)
			{
				std::cout<<currLine->type<<" "<<currLine->assignVar<<"="<<getTokenName(currLine->commandType)<<" "<<currLine->arg1<<" "<<currLine->arg2<<std::endl;
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

void BytecodeParser::convertToVector()
{
	parsedLinesVector.reserve(parsedLines.size());
	for(auto it=parsedLines.begin();it!=parsedLines.end();++it)
	{
		parsedLinesVector.push_back(parsedLines.front());
		parsedLines.pop_front();
	}
}

std::string BytecodeParser::generateBytecode(BytecodeLine* currLine)
{
	std::string str="";
	switch(currLine->type)
	{
		case LINE_JMP:
			str="j ";
			str+=currLine->arg1;
			break;
		case LINE_FALSE_JMP:
			str="jf ";
			str+=currLine->arg1;
			str+=" ";
			str+=currLine->arg2;
			break;
		case LINE_TRUE_JMP:
			str="jt ";
			str+=currLine->arg1;
			str+=" ";
			str+=currLine->arg2;
			break;
		case LINE_PUSH:
			str="^";
			break;
		case LINE_POP:
			str="!";
			break;
		case LINE_LABEL:
			str=currLine->arg1;
			str+=":";
			break;
		case LINE_ASSIGN:
			str=currLine->assignVar;
			str+="=";
			str+=currLine->arg1;
			break;
		case LINE_USING:
			str="using";
			if(currLine->arg1!="")
            {
                str+=" ";
                str+=currLine->arg1;
                if(currLine->arg2!="")
                {
                    str+=" ";
                    str+=currLine->arg2;
                }
            }
			break;
		case LINE_COMMAND:
			str=currLine->assignVar;
			str+="=";
			str+=toString(currLine->commandType);
			if(currLine->arg1!="")
			{
				str+=" ";
				str+=currLine->arg1;
				if(currLine->arg2!="")
				{
					str+=" ";
					str+=currLine->arg2;
				}
			}
			break;
		case LINE_ENDPARAM:
			str="ep";
			break;
        case LINE_RET:
            str="ret";
            if(currLine->arg1!="")
            {
                str+=" "+currLine->arg1;
            }
			break;
        default:
            break;
	}
	return str;
}

void BytecodeParser::writeToFile(const std::string& fileName,bool useVector)
{
	#if SHOW_TIMINGS == 1
		auto start = std::chrono::steady_clock::now();
	#endif
	remove(fileName.c_str());
    std::ofstream outFile(fileName, std::ios::app | std::ios::out);

	if(useVector==false)
	{
		for(auto it=parsedLines.begin();it!=parsedLines.end();++it)
		{
			outFile<<generateBytecode(*it)<<std::endl;
		}
	}
	else
	{
		for(auto it=parsedLinesVector.begin();it!=parsedLinesVector.end();++it)
		{
			outFile<<generateBytecode(*it)<<std::endl;
		}
	}

	outFile.close();
	#if SHOW_TIMINGS == 1
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Writing parsed Bytecode to file: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif
}

void BytecodeParser::parseBytecode(const std::string& code,bool useVector)
{
	#if SHOW_TIMINGS == 1
		auto start = std::chrono::steady_clock::now();
	#endif

	int lCode=code.length();
	char currChar;
	bool doubleQuote=false;
	bool singleQuote=false;
	std::string extractedValue;
	int numberOfCurrentToken=0;
	BytecodeLine* currLine=new BytecodeLine;
	int lineNumber=0;
	int i=0;

	for(;i<lCode;i++)
	{
		currChar=code[i];
		if(currChar=='"' && singleQuote==false)
		{
			doubleQuote=!doubleQuote;
			extractedValue+=currChar;
		}
		else if(currChar=='\'' && doubleQuote==false)
		{
			singleQuote=!singleQuote;
			extractedValue+=currChar;
		}
		else if(singleQuote==false && doubleQuote==false)
		{
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
						currLine->assignVar=extractedValue;
					}
				}
				else if(currLine->type==LINE_TRUE_JMP || currLine->type==LINE_FALSE_JMP)
				{
					if(numberOfCurrentToken==1)
					{
						// extract condition
						currLine->arg1=extractedValue;
					}
					else if(numberOfCurrentToken==2)
					{
						// extract label to jmp to
						currLine->arg2=extractedValue;
					}
				}
				else if(currLine->type==LINE_JMP)
				{
					if(numberOfCurrentToken==1)
					{
						// extract label to jmp to
						currLine->arg1=extractedValue;
					}
				}
                else if(currLine->type==LINE_RET)
				{
					if(numberOfCurrentToken==1)
					{
						// extract return value
						currLine->arg1=extractedValue;
					}
				}
				else if(currLine->type==LINE_USING)
				{
					if(numberOfCurrentToken==1)
					{
						// extract constant which value should be changed
						currLine->arg1=extractedValue;
					}
					else if(numberOfCurrentToken==2)
					{
						// extract new value of constant
						currLine->arg2=extractedValue;
					}
				}
				else
				{
					if(numberOfCurrentToken==1 && (currChar=='\n' || currChar=='\r'))
					{
						currLine->type=LINE_ASSIGN;
						currLine->arg1=extractedValue;
					}
					else if(numberOfCurrentToken==1)
					{
						currLine->type=LINE_COMMAND;
						currLine->commandType=(TokenTypes)std::stoi(extractedValue);
					}
					else if(numberOfCurrentToken==2)
					{
						currLine->arg1=extractedValue;
					}
					else if(numberOfCurrentToken==3)
					{
						currLine->arg2=extractedValue;
					}
				}

				extractedValue="";

				if(currChar=='\n')
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
		    extractedValue+=currChar;
			i++;
			if(i<lCode)
            {
                extractedValue+=code[i];
            }

		}
		else
		{
			extractedValue+=currChar;
		}
	}

	#if SHOW_TIMINGS == 1
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Parsing Bytecode: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif
}

