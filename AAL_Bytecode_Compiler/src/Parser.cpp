#include "Parser.h"

Parser::Parser(const std::string& code)
{
    parse(code);
}

Parser::~Parser()
{
    //dtor
}

/** \brief This function creates a syntax tree (multithreaded)
 *
 * \param
 * \param
 *
 */
void Parser::createSyntaxTreeThreaded(std::list<Token*>::iterator it, int cThreads)
{
    while(it!=rootToken.childTokens.end())
    {
        createSyntaxTree(*it);
        for(int i=0;i<cThreads;i++)
        {
            ++it;
            if(it==rootToken.childTokens.end())
                return;
        }
    }
}

/** \brief This function parses given code, checks its syntax, recognizes the corresponding tokens and creates a syntax-tree
 *
 * \param
 * \param
 *
 */
bool Parser::parse(const std::string& code)
{
	#if SHOW_TIMINGS == 1
		auto start = std::chrono::steady_clock::now();
	#endif

    tokenizer.tokenizeString(code);

	#if DEBUG>1
		std::cout<<"After rough tokenizing: "<<std::endl;
		for(auto it=tokenizer.tokens.begin(); it!=tokenizer.tokens.end(); ++it)
		std::cout<<getTokenName((*it)->type)<<"\n";
	#endif

	#if SHOW_TIMINGS == 1
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Rough tokenizing: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif

    updateSpecificTokenType(tokenizer.tokens);

	#if DEBUG>1
		std::cout<<"After complete tokenizing: "<<std::endl;
		for(auto it=tokenizer.tokens.begin(); it!=tokenizer.tokens.end(); ++it)
		{
			std::cout<<getTokenName((*it)->type)<<"\n";
		}
	#endif

	#if SHOW_TIMINGS == 1
		end = std::chrono::steady_clock::now();
		diff = end - start;
		std::cout << "Complete tokenizing: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif

    SyntaxValidator newValidator;
    auto foundProblems=newValidator.checkSyntax(tokenizer.tokens);

	#if SHOW_TIMINGS == 1
		end = std::chrono::steady_clock::now();
		diff = end - start;
		std::cout << "Syntax Validation: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif

    if(foundProblems.size()>0)
    {
		bool foundError=false;
		for(auto it=foundProblems.begin(); it!=foundProblems.end(); ++it)
		{
			if(it->importance==IMPORTANCE_ERROR)
			{
				foundError=true;
			}
			std::cout<<getProblemMessage(*it)<<std::endl;
		}

		if (foundError==true)
		{
			return false;
		}
    }

    Token* tmpToken;
    Token* currentToken=tokenizer.createNewToken();
    currentToken->setToken(TOK_NEWLINE,"\n");
    Token* lastNewLine=currentToken;
	Token* forVarToken=nullptr;
	std::vector<Token*> stepToken;

    rootToken.setToken(TOK_ROOT,"");
    rootToken.childTokens.push_back(lastNewLine);
    int neededClosingBracket=0;
    bool thenFound=false;
    bool insertEndif=false;
	bool inVarDecl=false;
	bool arrayDeclPossible=false;
	int bracketDepth=0;
	bool toFound=false;
	bool stepFound=false;
	bool assignForVarToken=false;
    bool createNewArray=true;
    for(auto it=tokenizer.tokens.begin(); it!=tokenizer.tokens.end(); ++it)
    {
        // Replace all [ and ] with ( and ) and insert before the replaced brackets the array "[]" operator-token
		// also change one line ifs to multiple line ifs
		// and insert brackets at specific position to make the syntaxtree creation easier
		// also update some "=" and "[", "]" tokens because they may have got wrong TokenType
		// and replace all for loops with while loops
        currentToken=*it;
		switch(currentToken->type)
		{
            case TOK_END:
                for(;neededClosingBracket>0;neededClosingBracket--)
                {
                    tmpToken=tokenizer.createNewToken();
                    tmpToken->setToken(TOK_BRACKET_ROUND_CLOSE,")");
                    lastNewLine->childTokens.push_back(tmpToken);
                }
                insertEndif=false;
                break;
			case TOK_NEWLINE:
			    inVarDecl=false;
				stepFound=false;
				insertEndif=false;

				if(toFound==true)
				{
					stepToken.push_back(currentToken);

					tmpToken=tokenizer.createNewToken();
					tmpToken->setToken(TOK_VAR,forVarToken->tokenValue);
					currentToken->childTokens.push_back(tmpToken);

					tmpToken=tokenizer.createNewToken();
					tmpToken->setToken(TOK_ASSIGN_ADD,"+=");
					currentToken->childTokens.push_back(tmpToken);

					tmpToken=tokenizer.createNewToken();
					tmpToken->setToken(TOK_NUMBER_DEC,"1");
					currentToken->childTokens.push_back(tmpToken);
					toFound=false;

					lastNewLine=tokenizer.createNewToken();
                    lastNewLine->setToken(TOK_NEWLINE,"\n");
					rootToken.childTokens.push_back(lastNewLine);
				}

                for(;neededClosingBracket>0;neededClosingBracket--)
                {
                    tmpToken=tokenizer.createNewToken();
                    tmpToken->setToken(TOK_BRACKET_ROUND_CLOSE,")");
                    lastNewLine->childTokens.push_back(tmpToken);
                }

                if(lastNewLine->childTokens.size()>0)
                {
                    lastNewLine=currentToken;
                    rootToken.childTokens.push_back(lastNewLine);
                }
				break;
			case TOK_BRACKET_ARRAY_OPEN:
				bracketDepth++;
				if(arrayDeclPossible==true && inVarDecl==true)
				{
				    if(createNewArray==true)
                    {
                        currentToken->type=TOK_BRACKET_ARRAY_CREATION;
                        createNewArray=false;
                    }
					else
                    {
                        currentToken->type=TOK_BRACKET_ARRAY_APPEND;
                    }
				}
				else
				{
					currentToken->type=TOK_BRACKET_ARRAY_ACCESS;
				}
				currentToken->tokenValue="[]";
				lastNewLine->childTokens.push_back(currentToken);

				currentToken=tokenizer.createNewToken();
                currentToken->setToken(TOK_BRACKET_ROUND_OPEN,"(");
				lastNewLine->childTokens.push_back(currentToken);
				break;
			case TOK_FOR:
				assignForVarToken=true;
				break;
            case TOK_TO:
				toFound=true;
				if(lastNewLine->childTokens.size()>0)
				{
					lastNewLine=currentToken;
					currentToken->setToken(TOK_NEWLINE,"\n");
					rootToken.childTokens.push_back(lastNewLine);
				}

				tmpToken=tokenizer.createNewToken();
				tmpToken->setToken(TOK_WHILE,"while");
				lastNewLine->childTokens.push_back(tmpToken);

				tmpToken=tokenizer.createNewToken();
				tmpToken->setToken(TOK_VAR,forVarToken->tokenValue);
				lastNewLine->childTokens.push_back(tmpToken);

				tmpToken=tokenizer.createNewToken();
				tmpToken->setToken(TOK_COMP_LOWEREQ,"<=");
				lastNewLine->childTokens.push_back(tmpToken);

				break;
            case TOK_VARDECL:
			case TOK_CONST:
            case TOK_SYNCHRONIZED:
            case TOK_ENUM:
            case TOK_STATIC:
            case TOK_VOLATILE:
				arrayDeclPossible=true;
				inVarDecl=true;
				createNewArray=true;
            case TOK_CASE:
            case TOK_NEW:
            case TOK_CLASS:
                lastNewLine->childTokens.push_back(currentToken);

				tmpToken=tokenizer.createNewToken();
                tmpToken->setToken(TOK_BRACKET_ROUND_OPEN,"(");
				lastNewLine->childTokens.push_back(tmpToken);

				neededClosingBracket++;
				break;
            case TOK_STEP:
				stepToken.push_back(currentToken);
				currentToken->setToken(TOK_NEWLINE,"\n");

                tmpToken=tokenizer.createNewToken();
                tmpToken->setToken(TOK_VAR,forVarToken->tokenValue);
				currentToken->childTokens.push_back(tmpToken);

				tmpToken=tokenizer.createNewToken();
                tmpToken->setToken(TOK_ASSIGN_ADD,"+=");
				currentToken->childTokens.push_back(tmpToken);
				toFound=false;
				stepFound=true;
				break;
			case TOK_NEXT:
				if(!stepToken.empty())
				{
					for(auto it=stepToken.back()->childTokens.begin();it!=stepToken.back()->childTokens.end();++it)
					{
						lastNewLine->childTokens.push_back(*it);
					}

					stepToken.pop_back();

					lastNewLine=tokenizer.createNewToken();;
					lastNewLine->setToken(TOK_NEWLINE,"\n");
					rootToken.childTokens.push_back(lastNewLine);

					currentToken->setToken(TOK_WEND,"wend");
					lastNewLine->childTokens.push_back(currentToken);
				}
				else
				{
					// error
				}
			break;
			case TOK_BRACKET_ARRAY_CLOSE:
				bracketDepth--;
				currentToken->type=TOK_BRACKET_ROUND_CLOSE;
				currentToken->tokenValue=")";
				lastNewLine->childTokens.push_back(currentToken);
				break;
            case TOK_THEN:
                currentToken->type=TOK_NEWLINE;
				currentToken->tokenValue="\n";
				lastNewLine=currentToken;
				rootToken.childTokens.push_back(currentToken);
                thenFound=true;
                break;
			case TOK_PUNCT_COMMA:
				if(inVarDecl==true && bracketDepth==0)
				{
					arrayDeclPossible=true;
					createNewArray=true;
				}
				goto push_line_label;
			case TOK_BRACKET_ROUND_OPEN:
			case TOK_BRACKET_OPEN:
				bracketDepth++;
				goto push_line_label;
			case TOK_BRACKET_ROUND_CLOSE:
			case TOK_BRACKET_CLOSE:
				bracketDepth--;
				goto push_line_label;
			case TOK_COMP_EQINCASE:
				if(inVarDecl==true && arrayDeclPossible==true && bracketDepth==0)
				{
					currentToken->type=TOK_ASSIGN;
					arrayDeclPossible=false;
				}
				goto push_line_label;
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
				arrayDeclPossible=false;
				goto push_line_label;
			case TOK_VAR:
				if(assignForVarToken==true)
				{
					forVarToken=currentToken;
					assignForVarToken=false;
				}
				goto push_line_label;
			default:
				push_line_label:
				if(!stepToken.empty() && stepFound==true)
				{
					stepToken.back()->childTokens.push_back(currentToken);
				}
				else
				{
					lastNewLine->childTokens.push_back(currentToken);
				}
				break;
		}

        if(insertEndif)
        {
            tmpToken=tokenizer.createNewToken();
            tmpToken->setToken(TOK_NEWLINE,"\n");
            rootToken.childTokens.push_back(tmpToken);

            Token* tmpToken2=tokenizer.createNewToken();
            tmpToken2->setToken(TOK_ENDIF,"EndIf");
            tmpToken->childTokens.push_back(tmpToken2);
            insertEndif=false;
        }

		insertEndif=false;
		if(thenFound)
		{
            insertEndif=true;
            thenFound=false;
		}
    }
	#if DEBUG>0
		std::cout<<"Before Syntaxtree creation: "<<std::endl;
		printSyntaxTree(&rootToken,1);
	#endif

	#if SHOW_TIMINGS == 1
		end = std::chrono::steady_clock::now();
		diff = end - start;
		std::cout << "Code Preparation: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif
    auto it=rootToken.childTokens.begin();

	#if USE_MULTITHREADING == 1
		std::thread *threads=new std::thread[THREAD_COUNT];
		int usedThreads=0;
		for(int i=0;i<THREAD_COUNT && it!=rootToken.childTokens.end();i++)
		{
			threads[i]=std::thread(&Parser::createSyntaxTreeThreaded, this,it,THREAD_COUNT);
			++it;
			usedThreads++;
		}

		for(int i=0;i<usedThreads;i++)
		{
			threads[i].join();
		}
		delete[] threads;
	#else
		for(it=rootToken.childTokens.begin(); it!=rootToken.childTokens.end(); ++it)
		{
			createSyntaxTree(*it);
		}
	#endif

	#if DEBUG>1
		std::cout<<"Created Syntaxtree: "<<std::endl;
		printSyntaxTree(&rootToken,1);
	#endif

	#if SHOW_TIMINGS == 1
		end = std::chrono::steady_clock::now();
		diff = end - start;
		std::cout << "Creating SyntaxTree: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();

    #endif

    return true;
}


/** \brief This function gets precedence of token or return -1 if token were already processed completly
 *
 * \param
 * \param
 *
 */
int Parser::getChildTokenPrecedence(Token* currChildToken)
{
	if(currChildToken->processed==false)
	{
		// our token doesnt have childs so we didnt process him or we have got a number/command/variable, if we have got a number/etc. we will also get -1
		return getPrecedence(currChildToken->type);
	}
	else
	{
		// we already proccessed this token so just set precedence to -1
		return -1;
	}
}

/** \brief This function creates syntax tree
 *
 * \param
 * \param
 *
 */
void Parser::createSyntaxTree(Token* currToken)
{

	// current child of currToken token
	Token* currChildToken;
	// last operator token
	Token* lastOpChildToken=nullptr;
	// precedence of last operator
	int lastOpPrecedence=-1;
	// precedence of current operator
	int currPrecedence;
	// tokens which are open to process
	std::stack<Token*> openTokens;
	//iterator for currToken
	auto it=currToken->childTokens.begin();
	// universal end token to make recursive processing easier
	// (now function knows even when called recurisve where children list ends)
	Token* universalEnd=new Token(TOK_END,"");
	// Add at the end of each token list an "end"-Token so we know when list is processed completely
	currToken->childTokens.push_back(universalEnd);

	#if DEBUG>2
		std::cout<<std::endl<<"createSyntaxTree Entry: "<<std::endl;
		printSyntaxTree(currToken,1);
	#endif

	while(!currToken->childTokens.empty())
	{
		it=currToken->childTokens.begin();
		currChildToken=*it;
		currToken->childTokens.pop_front();

		currPrecedence=getChildTokenPrecedence(currChildToken);

		#if DEBUG>4
			std::cout<<"currChildToken: "<< currChildToken->tokenValue<< " "<< getTokenName(currChildToken->type) <<""<<std::endl;
		#endif

		if(currPrecedence>-1)
		{
			if(currPrecedence <= lastOpPrecedence && lastOpPrecedence!=0 && lastOpChildToken!=nullptr)
			{
				// our current operator has got lower precedence than the last one
				int checkingPrecedence=getChildTokenPrecedence(openTokens.top());
				while(!openTokens.empty() && (checkingPrecedence==-1 || checkingPrecedence >= currPrecedence))
				{
					// while there are tokens left in openTokens-Stack and (we are processing numbers/etc. or higher or equal precedence operators)
					if(checkingPrecedence>-1)
					{
						// we found operator
						if(lastOpChildToken!=openTokens.top())
						{
							// if operator is not current operator
							// push old operator to our new operator childrens list
							openTokens.top()->childTokens.push_front(lastOpChildToken);
							// set new operator to current operator
							lastOpChildToken=openTokens.top();
						}
						openTokens.pop();
					}
					else
					{
						Token* tmpBracketTokens=openTokens.top();

						if(tmpBracketTokens->type == TOK_BRACKET_ROUND_CLOSE)
						{
							// we found no operator, but a bracket
							openTokens.pop();
							int bracketDepth=-1;
							lastOpChildToken->childTokens.push_front(universalEnd);
							while(!openTokens.empty() && bracketDepth!=0)
							{
								// add all between the brackets to the childrenTokens of last operator (so they get evaluated earlier)
								tmpBracketTokens=openTokens.top();
								openTokens.pop();

								if(tmpBracketTokens->type == TOK_BRACKET_ROUND_CLOSE)
								{
									bracketDepth--;
								}
								else if(tmpBracketTokens->type == TOK_BRACKET_ROUND_OPEN)
								{
									bracketDepth++;
									if(bracketDepth==0)
										break;
								}
								lastOpChildToken->childTokens.push_front(tmpBracketTokens);
							}

							createSyntaxTree(lastOpChildToken);
						}
						else
						{
							// we found no operator, so push found token to last operator
							lastOpChildToken->childTokens.push_front(openTokens.top());
							openTokens.pop();
						}
					}
					// process next token
					if(!openTokens.empty())
						checkingPrecedence=getChildTokenPrecedence(openTokens.top());
				}

				if(currChildToken->type!=TOK_END)
				{
					currToken->childTokens.push_front(currChildToken);
				}
				currToken->childTokens.push_front(lastOpChildToken);
				lastOpChildToken->processed=true;

				#if DEBUG>4
					std::cout<<"currChildToken2: "<< currChildToken->tokenValue<< " "<< getTokenName(currChildToken->type) <<""<<std::endl;
					std::cout<<"lastOpChildToken: "<< lastOpChildToken->tokenValue<< " "<< getTokenName(lastOpChildToken->type) <<""<<std::endl;
				#endif

				if(currPrecedence==0)
				{
					break;
				}


				lastOpPrecedence=-1;

				lastOpChildToken=nullptr;

			}
			else
			{
				lastOpChildToken=currChildToken;
				lastOpPrecedence=currPrecedence;
				openTokens.push(currChildToken);
			}
		}
		else if(currChildToken->type!=TOK_END && currChildToken->type!=TOK_NEWLINE)
		{
			if(currChildToken->type == TOK_BRACKET_ROUND_OPEN)
			{
				// handle all between "(" and ")" as one big "number/argument"
				int bracketDepth=1;
				openTokens.push(currChildToken);
				while(!currToken->childTokens.empty() && bracketDepth!=0)
				{
					it=currToken->childTokens.begin();
					currChildToken=*it;
					currToken->childTokens.pop_front();

					if(currChildToken->type == TOK_BRACKET_ROUND_CLOSE)
					{
						bracketDepth--;
					}
					else if(currChildToken->type == TOK_BRACKET_ROUND_OPEN)
					{
						bracketDepth++;
					}
					openTokens.push(currChildToken);
				}
			}
			else
			{
				openTokens.push(currChildToken);
			}
		}
		else
		{
			break;
		}
	}

	#if DEBUG>3
		std::cout<<std::endl<<"createSyntaxTree After first loop: "<<std::endl;
		printSyntaxTree(currToken,1);
		std::cout<<"printed Tree"<<std::endl;
	#endif

    while(!openTokens.empty())
    {
        Token* workingToken=openTokens.top();
        openTokens.pop();
		#if DEBUG>4
			std::cout<<"openToken: "<< workingToken->tokenValue<< " (size:"<< openTokens.size() <<")"<<std::endl;
		#endif
        if(workingToken->type!=TOK_END && workingToken->type!=TOK_NEWLINE)
        {
            if(workingToken->type!=TOK_BRACKET_ROUND_CLOSE)
            {
                currToken->childTokens.push_front(workingToken);
            }
            else
            {
                int bracketDepth=-1;
                currToken->childTokens.push_front(universalEnd);
                while(!openTokens.empty() && bracketDepth!=0)
                {
                    // add all between the brackets to the childrenTokens of last operator (so they get evaluated earlier)
                    workingToken=openTokens.top();
                    openTokens.pop();

                    if(workingToken->type == TOK_BRACKET_ROUND_CLOSE)
                    {
                        bracketDepth--;
                    }
                    else if(workingToken->type == TOK_BRACKET_ROUND_OPEN)
                    {
                        bracketDepth++;
                        if(bracketDepth==0)
                            break;
                    }
                    currToken->childTokens.push_front(workingToken);
                }
                createSyntaxTree(currToken);
            }
        }

    }

	#if DEBUG>3
		std::cout<<std::endl<<"createSyntaxTree leave: "<<std::endl;
		printSyntaxTree(currToken,1);
	#endif

	while(!currToken->childTokens.empty() && currToken->childTokens.back()==universalEnd)
	{
		currToken->childTokens.pop_back();
	}
    delete universalEnd;
}

/** \brief This function updates the found tokens to more specific one, this function also merges tokens if needed
 *
 * \param
 * \param
 *
 */
void Parser::updateSpecificTokenType(std::list<Token*>& updatedTokens)
{
	// Variable holding current line number
	int currLine=1;
	// Variable, which is true if assignement for token is allowed (=true) or not (=false)
    bool assignPossible=true;
	// Holding current token
    Token* currToken=nullptr;
	// Position of token: currToken-1
    Token* lastToken=nullptr;
	// Position of token: currToken+1
    Token* nextToken=nullptr;
	// Variable holding true if assignment is not already used in line (used=true, not used=false)
	bool assignAlreadyUsed=false;
	// Current depth of all brackets
	int currBracketDepth=0;
	// Variable holding true when in function declaration
	bool inFuncDecl=false;
	// Variable holding true when in variable declaration
	bool inVarDecl=false;

    std::list<Token*>::iterator lastTokenIt=updatedTokens.begin();
    std::list<Token*>::iterator nextTokenIt;

    for(auto tokenIt=updatedTokens.begin(); tokenIt!=updatedTokens.end(); ++tokenIt)
    {
        currToken=*tokenIt;
        nextTokenIt=tokenIt;
        ++nextTokenIt;
        if(nextTokenIt!=updatedTokens.end())
            nextToken=*nextTokenIt;
        else
            nextToken=nullptr;

		// Check if we found end of script token
        if (currToken->type==TOK_END)
            break;

		currToken->currLine=currLine;

		if(currToken->type==TOK_FUNCDECL)
		{
			inFuncDecl=true;
		}
		else if(currToken->type==TOK_VARDECL || currToken->type==TOK_CONST || currToken->type==TOK_VOLATILE || currToken->type==TOK_SYNCHRONIZED || currToken->type==TOK_STATIC)
		{
			inVarDecl=true;
		}

		if(nextToken->type==TOK_NEWLINE)
		{
			// When a new line starts we cannot be in a function declaration
			inFuncDecl=false;
			inVarDecl=false;
			currLine++;
		}

		if(currToken->type==TOK_PUNCT_COMMA)
		{
			if(inFuncDecl==true && currBracketDepth==1)
			{
				// There can be multiple assignments in function declaration
				assignPossible=true;
			}
			else if(inVarDecl==true && currBracketDepth==0)
			{
				// There can be multiple assignments in variable declaration
				assignPossible=true;
			}
		}

        if(lastToken==nullptr)
        {
			// nothing to do at all?
        }
        else
        {
            switch(lastToken->type)
            {
                case TOK_VAR:
                    if(nextToken->type == TOK_OP_ADD && currToken->type == TOK_OP_ADD)
                    {
						// merged token: ++
                        currToken->type=TOK_OP_INC;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                    }
                    else if(nextToken->type == TOK_OP_SUB && currToken->type == TOK_OP_SUB)
                    {
						// merged token: --
                        currToken->type=TOK_OP_DEC;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                    }
                    break;
                case TOK_NUMBER_DEC:
                    if(currToken->type==TOK_PUNCT_DOT && nextToken->type==TOK_NUMBER_DEC)
                    {
						// merged token (e.g.): 123.11515
                        lastToken->tokenValue+=currToken->tokenValue;
                        lastToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        updatedTokens.erase(tokenIt);
                        tokenIt=lastTokenIt;
                    }
                    break;
				case TOK_FUNCDECL:
					// we found a function declaration before so the next token must be a function
					currToken->type=TOK_FUNC;
					break;
                default:

                    break;
            }
        }

        if(nextToken->type==TOK_ASSIGN)
        {
            if(assignPossible==true && currBracketDepth==0)
            {
                // Assignment is possible
                assignPossible=false;
				assignAlreadyUsed=true;
                switch(currToken->type)
                {
                    case TOK_OP_ADD:
						// merged token: +=
                        currToken->type=TOK_ASSIGN_ADD;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_MUL:
						// merged token: *=
                        currToken->type=TOK_ASSIGN_MUL;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_SUB:
						// merged token: -=
                        currToken->type=TOK_ASSIGN_SUB;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_DIV:
						// merged token: /=
                        currToken->type=TOK_ASSIGN_DIV;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_JOIN:
						// merged token: &=
                        currToken->type=TOK_ASSIGN_JOIN;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_MOD:
						// merged token: %=
                        currToken->type=TOK_ASSIGN_MOD;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_POW:
						// merged token: ^=
                        currToken->type=TOK_ASSIGN_POW;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_SHL:
						// merged token: <<=
                        currToken->type=TOK_ASSIGN_SHL;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_SHR:
						// merged token: >>=
                        currToken->type=TOK_ASSIGN_SHR;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_ASSIGN:
						// merged token: ==
                        currToken->type=TOK_COMP_EQCASE;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_PUNCT_NOT:
						// merged token: !=
                        currToken->type=TOK_COMP_NOTEQ;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_VAR:
                        // nothing to do
						// merged token: =
                        break;
                    default:

                        break;
                }
            }
            else if(assignPossible==true && inFuncDecl==true && currBracketDepth==1)
			{
				if(currToken->type==TOK_ASSIGN)
				{
					// merged token: ==
					currToken->type=TOK_COMP_EQCASE;
					currToken->tokenValue+=nextToken->tokenValue;
					updatedTokens.erase(nextTokenIt);
					tokenIt=lastTokenIt;
				}
				else
				{
					if(inFuncDecl==true)
					{
						// merged token: =
						nextToken->type=TOK_DEFAULT_ASSIGN;
					}
					// else:
					// vardecl, so nothing to do
				}
				assignPossible=false;
			}
			else
            {
                // Assignment is not possible
                switch(currToken->type)
                {
                    case TOK_ASSIGN:
					case TOK_COMP_EQINCASE:
						// merged token: ==
                        currToken->type=TOK_COMP_EQCASE;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_BRACKET_ANGLE_OPEN:
						// merged token: <=
                        currToken->type=TOK_COMP_LOWEREQ;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_BRACKET_ANGLE_CLOSE:
						// merged token: >=
                        currToken->type=TOK_COMP_GREATEREQ;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_OP_NOT:
						// merged token: !=
                        currToken->type=TOK_COMP_NOTEQ;
                        currToken->tokenValue+=nextToken->tokenValue;
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    case TOK_PUNCT_NOT:
						// merged token: !=
                        currToken->type=TOK_COMP_NOTEQ;
						currToken->tokenValue="!=";
                        updatedTokens.erase(nextTokenIt);
                        tokenIt=lastTokenIt;
                        break;
                    default:
						// merged token (comparator): =
                        nextToken->type=TOK_COMP_EQINCASE;
                        break;
                }
            }
        }
        else if(currToken->type == TOK_BRACKET_ANGLE_OPEN)
        {
            if( nextToken->type == TOK_BRACKET_ANGLE_CLOSE)
            {
				// merged token: <>
                currToken->type=TOK_COMP_NOTEQ;
                currToken->tokenValue+=nextToken->tokenValue;
                updatedTokens.erase(nextTokenIt);
                tokenIt=lastTokenIt;
            }
            else if(nextToken->type == TOK_BRACKET_ANGLE_OPEN)
            {
				// merged token: <<
                currToken->type=TOK_OP_SHL;
                currToken->tokenValue+=nextToken->tokenValue;
                updatedTokens.erase(nextTokenIt);
                tokenIt=lastTokenIt;
            }
            else
            {
				// merged token: <
                currToken->type=TOK_COMP_LESS;
                tokenIt=lastTokenIt;
            }
        }
        else if(currToken->type==TOK_BRACKET_ANGLE_CLOSE)
        {
            if(nextToken->type == TOK_BRACKET_ANGLE_CLOSE)
            {
				// merged token: >>
                currToken->type=TOK_OP_SHR;
                currToken->tokenValue+=nextToken->tokenValue;
                updatedTokens.erase(nextTokenIt);
                tokenIt=lastTokenIt;
            }
            else
            {
				// merged token: >
                currToken->type=TOK_COMP_GREATER;
                tokenIt=lastTokenIt;
            }
        }
		else if(currToken->type==TOK_FUNC && nextToken->type==TOK_NEWLINE && currToken->tokenValue=="_" )
        {
			// Tokens:
			// _\n
			// are erased
			updatedTokens.erase(nextTokenIt);
			updatedTokens.erase(tokenIt);
			tokenIt=lastTokenIt;
		}
		else if(currToken->type==TOK_OP_SUB || currToken->type==TOK_OP_ADD)
        {
			if(nextToken->type==TOK_NUMBER_DEC || nextToken->type==TOK_NUMBER_HEX || nextToken->type==TOK_NUMBER_BIN)
			{
				if(lastToken==nullptr || (lastToken->type!=TOK_LITERAL && lastToken->type!=TOK_VAR && lastToken->type!=TOK_PREDEFINEDVAR &&
				lastToken->type!=TOK_DEFAULT && lastToken->type!=TOK_NULL && lastToken->type!=TOK_NUMBER_HEX && lastToken->type!=TOK_NUMBER_DEC &&
				lastToken->type!=TOK_NUMBER_BIN && lastToken->type!=TOK_TRUE && lastToken->type!=TOK_FALSE && lastToken->type!=TOK_BRACKET_ROUND_CLOSE &&
				lastToken->type!=TOK_BRACKET_ARRAY_CLOSE && lastToken->type!=TOK_OP_INC && lastToken->type!=TOK_OP_DEC))
				{
					// merged token (e.g.): -123
					// OR
					// merged token (e.g.): +123
					nextToken->tokenValue=currToken->tokenValue+nextToken->tokenValue;
					updatedTokens.erase(tokenIt);
					tokenIt=lastTokenIt;
				}
			}
		}
        else if(currToken->type == TOK_FUNC && nextToken->type == TOK_PUNCT_DOUBLEDOT)
        {
            // merged token: label
            currToken->type=TOK_JMP_LABEL;
            updatedTokens.erase(nextTokenIt);
            tokenIt=lastTokenIt;
        }
		// Update BracketDepth
		if(nextToken->type==TOK_BRACKET_ARRAY_OPEN)
			currBracketDepth++;
		else if(nextToken->type==TOK_BRACKET_ARRAY_CLOSE)
			currBracketDepth--;
		else if(nextToken->type==TOK_BRACKET_ROUND_OPEN)
			currBracketDepth++;
		else if(nextToken->type==TOK_BRACKET_ROUND_CLOSE)
			currBracketDepth--;
		else if(currBracketDepth==0)
		{
			// Update if assignment is still possible
			switch (currToken->type)
			{
				case TOK_IF:
				case TOK_WITH:
				case TOK_WHILE:
				case TOK_SELECT:
				case TOK_SWITCH:
				case TOK_UNTIL:
				case TOK_BRACKET_ARRAY_OPEN:
				case TOK_BRACKET_ROUND_OPEN:
				case TOK_FALSE:
				case TOK_TRUE:
				case TOK_FUNC:
				case TOK_CASE:
				case TOK_BRACKET_OPEN:
				case TOK_BRACKET_CLOSE:
				case TOK_CONTINUE:
				case TOK_ELSEIF:
				case TOK_EXIT_LOOP:
				case TOK_LITERAL:
				case TOK_LOGIC_AND:
				case TOK_LOGIC_OR:
				case TOK_NULL:
				case TOK_NUMBER_BIN:
				case TOK_NUMBER_HEX:
				case TOK_NUMBER_DEC:
				case TOK_PREDEFINEDVAR:
				case TOK_PUNCT_NOT:
				case TOK_PUNCT_QST:
				case TOK_PUNCT_DOUBLEDOT:
				case TOK_RETURN:
				case TOK_STEP:
				case TOK_TO:
					assignPossible=false;
					break;
				case TOK_THEN:
				case TOK_NEWLINE:
					assignAlreadyUsed=false;
					assignPossible=true;
					break;
				case TOK_BRACKET_ARRAY_CLOSE:
					if(assignAlreadyUsed==false)
						assignPossible=true;
					break;
				default:

					break;
			}
		}

        lastTokenIt=tokenIt;
        lastToken=*tokenIt;
    }
}
