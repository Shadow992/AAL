#include "Optimizer.h"


void Optimizer::optimize(Token* rootToken, int level)
{
    #if SHOW_TIMINGS == 1
		auto start = std::chrono::steady_clock::now();
    #endif

	#if USE_MULTITHREADING == 1
	    auto it=rootToken->childTokens.begin();
		std::thread *threads=new std::thread[THREAD_COUNT];
		int usedThreads=0;
		for(int i=0;i<THREAD_COUNT && it!=rootToken->childTokens.end();i++)
		{
			threads[i]=std::thread(&Optimizer::doBasicCalculationsThreaded, this,rootToken,it,THREAD_COUNT);
			++it;
			usedThreads++;
		}

		for(int i=0;i<usedThreads;i++)
		{
			threads[i].join();
		}
		delete[] threads;
	#else
        doBasicCalculations(rootToken);
	#endif

	#if DEBUG>1
		std::cout<<"Optimized Syntaxtree: "<<std::endl;
		printSyntaxTree(rootToken,1);
	#endif

	#if SHOW_TIMINGS == 1
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Optimizing Syntaxtree: "<< std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
    #endif
}

/** \brief This method iterates over syntax tree and does basic calculations multithreaded
 *
 * \param
 * \param
 * \return
 *
 */
void Optimizer::doBasicCalculationsThreaded(Token* rootToken, std::list<Token*>::iterator it, int cThreads)
{
    while(it!=rootToken->childTokens.end())
    {
        doBasicCalculations(*it);
        for(int i=0;i<cThreads;i++)
        {
            ++it;
            if(it==rootToken->childTokens.end())
                return;
        }
    }
}

// this function extracts all variable assignments
/*void Optimizer::extractVariableAssignments(Token* currToken, std::vector<Token*>& extracted, bool isConst)
{
	Token* currChildToken;
	for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
	{
		currChildToken=*it;
		if(currChildToken->type==TOK_ASSIGN)
		{
		    if(currChildToken->childTokens.size()>0 && currChildToken->childTokens.begin()->type==TOK_VAR)
            {
                extracted.push_back(currChildToken);
            }
		}
		extractVariableAssignments(currChildToken,extracted);
	}
}*/

void Optimizer::doBasicCalculations(Token* currToken)
{
	Token* currChildToken;
	bool calculateAble=true;
	bool useDouble=false;

	for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
	{
		currChildToken=*it;
		doBasicCalculations(currChildToken);

		if(calculateAble==true)
		{
			switch(currChildToken->type)
			{
				case TOK_NUMBER_DEC:
					if(useDouble==false && isNumberDouble(currChildToken->tokenValue))
					{
						useDouble=true;
					}
					break;
				case TOK_LITERAL:
				case TOK_NULL:
				case TOK_NUMBER_HEX:
				case TOK_NUMBER_BIN:
				case TOK_TRUE:
				case TOK_FALSE:
					// still calculateable
					break;
				default:
					calculateAble=false;
					break;
			}
		}
	}

	if(currToken->childTokens.size()==0)
	{
		switch(currToken->type)
		{
			case TOK_NULL:
				currToken->setToken(TOK_NUMBER_DEC,"0");
				break;
			case TOK_NUMBER_BIN:
				currToken->setToken(TOK_NUMBER_DEC,toString(std::stoll(currToken->tokenValue.substr(2),nullptr,2)));
				break;
			case TOK_TRUE:
				currToken->setToken(TOK_NUMBER_DEC,"1");
				break;
			case TOK_FALSE:
				currToken->setToken(TOK_NUMBER_DEC,"0");
				break;
			default:
				break;
		}
	}
	else if(calculateAble==true && currToken->childTokens.size()==2)
	{
		auto it=currToken->childTokens.begin();
		std::string argument1String=(*it)->tokenValue;
		++it;
		std::string argument2String=(*it)->tokenValue;
		long long argument1;
		long long argument2;
		double argument1Double;
		double argument2Double;
		switch(currToken->type)
		{
			case TOK_OP_SHL:
				argument1=std::stoll(argument1String,nullptr,0);
				argument2=std::stoll(argument2String,nullptr,0);
				argument1<<=argument2;
				currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				goto clear_token_list_label;
			case TOK_OP_SHR:
				argument1=std::stoll(argument1String,nullptr,0);
				argument2=std::stoll(argument2String,nullptr,0);
				argument1>>=argument2;
				currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				goto clear_token_list_label;
			case TOK_OP_MOD:
				argument1=std::stoll(argument1String,nullptr,0);
				argument2=std::stoll(argument2String,nullptr,0);
				argument1%=argument2;
				currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				goto clear_token_list_label;

			case TOK_OP_MUL:
				if(useDouble==true)
				{
					argument1Double=std::stod(argument1String,nullptr);
					argument2Double=std::stod(argument2String,nullptr);
					argument1Double*=argument2Double;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1Double));
				}
				else
				{
					argument1=std::stoll(argument1String,nullptr,0);
					argument2=std::stoll(argument2String,nullptr,0);
					argument1*=argument2;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				}
				goto clear_token_list_label;
			case TOK_OP_DIV:
				if(useDouble==true)
				{
					argument1Double=std::stod(argument1String,nullptr);
					argument2Double=std::stod(argument2String,nullptr);
					argument1Double/=argument2Double;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1Double));
				}
				else
				{
					argument1=std::stoll(argument1String,nullptr,0);
					argument2=std::stoll(argument2String,nullptr,0);
					if(argument1%argument2==0)
					{
						argument1/=argument2;
						currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
					}
					else
					{
						argument1Double=argument1;
						argument2Double=argument2;
						argument1Double/=argument2Double;
						currToken->setToken(TOK_NUMBER_DEC,toString(argument1Double));
					}
				}
				goto clear_token_list_label;
			case TOK_OP_ADD:
				if(useDouble==true)
				{
					argument1Double=std::stod(argument1String,nullptr);
					argument2Double=std::stod(argument2String,nullptr);
					argument1Double+=argument2Double;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1Double));
				}
				else
				{
					argument1=std::stoll(argument1String,nullptr,0);
					argument2=std::stoll(argument2String,nullptr,0);
					argument1+=argument2;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				}
				goto clear_token_list_label;
			case TOK_OP_SUB:
				if(useDouble==true)
				{
					argument1Double=std::stod(argument1String,nullptr);
					argument2Double=std::stod(argument2String,nullptr);
					argument1Double-=argument2Double;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1Double));
				}
				else
				{
					argument1=std::stoll(argument1String,nullptr,0);
					argument2=std::stoll(argument2String,nullptr,0);
					argument1-=argument2;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				}
				goto clear_token_list_label;
			case TOK_OP_POW:
				if(useDouble==true)
				{
					argument1Double=std::stod(argument1String,nullptr);
					argument2Double=std::stod(argument2String,nullptr);
					currToken->setToken(TOK_NUMBER_DEC,toString(pow(argument1Double,argument2Double)));
				}
				else
				{
					argument1=std::stoll(argument1String,nullptr,0);
					argument2=std::stoll(argument2String,nullptr,0);
					argument1+=argument2;
					currToken->setToken(TOK_NUMBER_DEC,toString(argument1));
				}
				goto clear_token_list_label;
			case TOK_OP_JOIN:
				currToken->setToken(TOK_LITERAL,argument1String+argument2String);

				clear_token_list_label:
				currToken->childTokens.clear();
				break;
			default:
				break;
		}
	}
}
