#include "SyntaxValidator.h"

/** \brief This function checks whether the given token list has got errors/problems
 *
 * \param
 * \param
 * \return
 *
 */
std::vector<Problem> SyntaxValidator::checkSyntax(const std::list<Token*>& tokenList)
{
    // class to check for known/unknown functions/classes */
    FunctionAndClassChecker checker;
	// variable holding all found ERRORs, WARNINGs and INFORMATION
	std::vector<Problem> foundProblems;

    std::vector<Token*> maybeUndefinedFunctions;
    std::vector<Token*> maybeUndefinedClasses;

	// variables which determine if starting/ending keyword is allowed
	bool startingKeywordsAllowed=true; // starting keywords are: if, switch, while, do, select, for
	bool endingKeywordsAllowed=false; // ending keywords are: endif, endswitch, Wend, until, endselect, next

	// variables which determine if static/atomic/etc. were already used in this line
	bool staticUsed=false;
	bool atomicUsed=false;
	bool constUsed=false;
	bool vardeclUsed=false;
	bool volatileUsed=false;
    bool enumUsed=false;

	// variable holding depth of functions
	int funcCount=0;
	// variable holding depth of loops
	int loopCount=0;

	// variable holding all starting keywords, which were found
	// everytime a corresponding endkeyword were found, the last starting keyword will be removed from lastStartingKeywordType
	std::vector<Token*> lastStartingKeywordType;
	// variable holding all opening brackets, which were found
	// everytime a corresponding closing bracket were found, the last opening bracket will be removed from lastBracket
	std::vector<Token*> lastBracket;

	// variable holding current Token
	Token* currToken;
	// variable holding one token before current token
	Token* lastToken=nullptr;

	for(auto it = tokenList.begin();it!=tokenList.end();++it)
	{
		currToken=*it;
		if(lastToken!=nullptr)
		{
			switch(lastToken->type)
			{

				case TOK_PUNCT_QST:
                    lastStartingKeywordType.push_back(lastToken);
                    goto operator_not_allowed_label;


                case TOK_PUNCT_DOUBLEDOT:
					if(lastStartingKeywordType.empty())
					{
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
					}
					else
					{
						if(lastStartingKeywordType.back()->type!=TOK_PUNCT_QST)
						{
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastStartingKeywordType.back(),lastToken));
						}
					}
                    goto operator_not_allowed_label;

				case TOK_BRACKET_ROUND_OPEN:
				case TOK_BRACKET_ANGLE_OPEN:
				case TOK_BRACKET_ARRAY_OPEN:
                case TOK_BRACKET_OPEN:
					lastBracket.push_back(lastToken);
					goto operator_not_allowed_label;

                case TOK_PUNCT_COMMA:
                case TOK_OP_NOT:
				case TOK_OP_MOD:
				case TOK_OP_MUL:
				case TOK_OP_DIV:
				case TOK_OP_SHL:
				case TOK_OP_SHR:
				case TOK_OP_JOIN:
				case TOK_OP_POW:
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
				case TOK_COMP_EQCASE:
				case TOK_COMP_EQINCASE:
				case TOK_COMP_LOWEREQ:
				case TOK_COMP_GREATEREQ:
				case TOK_COMP_NOTEQ:
				case TOK_COMP_LESS:
				case TOK_COMP_GREATER:
                case TOK_LOGIC_AND:
				case TOK_LOGIC_OR:
				case TOK_OP_ADD:
				case TOK_OP_SUB:
				case TOK_BRACKET_ARRAY_ACCESS:
				case TOK_BRACKET_ARRAY_CREATION:
                case TOK_BRACKET_ARRAY_APPEND:
				case TOK_PUNCT_NOT:
				case TOK_PUNCT_DOT:
				case TOK_BYREF:
					goto operator_not_allowed_label;

				case TOK_RETURN:
					if(funcCount<=0)
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_OUTSIDE_OF_FUNC,currToken,lastToken));
					}
					goto operator_not_allowed_label;

                case TOK_THEN:
					if(lastStartingKeywordType.empty())
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_OUTSIDE_OF_IF,lastToken,nullptr));
					}
					else
					{
						if(lastStartingKeywordType.back()->type!=TOK_IF && lastStartingKeywordType.back()->type!=TOK_ELSEIF )
						{
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastStartingKeywordType.back(),lastToken));
						}
					}
					if(currToken->type==TOK_NEWLINE)
					{
						break;
					}
					goto operator_not_allowed_label;

                case TOK_NEW:
                    if(currToken->type!=TOK_FUNC)
                    {
                        foundProblems.push_back(Problem(ERROR_NOT_POSSIBLE_CLASSNAME,currToken,lastToken));
                    }
                    if(checker.isClassAvailable(currToken->tokenValue)==false)
                    {
                        maybeUndefinedClasses.push_back(currToken);
                    }
                    break;
                case TOK_CLASS:
                    if(currToken->type!=TOK_FUNC)
                    {
                        foundProblems.push_back(Problem(ERROR_NOT_POSSIBLE_CLASSNAME,currToken,lastToken));
                    }
                    checker.addClass(currToken->tokenValue);
                    lastStartingKeywordType.push_back(lastToken);
                    break;
                case TOK_FUNCDECL:
                    if(currToken->type!=TOK_FUNC)
                    {
                        foundProblems.push_back(Problem(ERROR_NOT_POSSIBLE_FUNCTIONNAME,currToken,lastToken));
                    }
					funcCount++;
					checker.addFunction(currToken->tokenValue);
                    lastStartingKeywordType.push_back(lastToken);
                    break;

				case TOK_IF:
				    lastStartingKeywordType.push_back(lastToken);
				    goto tok_is_keyword_label;
				case TOK_SWITCH:
				    lastStartingKeywordType.push_back(lastToken);
				    goto tok_is_keyword_label;
				case TOK_SELECT:
				    lastStartingKeywordType.push_back(lastToken);
				    goto tok_is_keyword_label;
                case TOK_WHILE:
					loopCount++;
				    lastStartingKeywordType.push_back(lastToken);
				    goto tok_is_keyword_label;
                case TOK_WITH:
				    lastStartingKeywordType.push_back(lastToken);
                    goto tok_is_keyword_label;
                case TOK_UNTIL:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
						loopCount--;
                        if(lastStartingKeywordType.back()->type!=TOK_DO)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
                    }
					goto tok_is_keyword_label;
				case TOK_CASE:
					if(lastStartingKeywordType.empty())
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
					}
					else
					{
						if(lastStartingKeywordType.back()->type!=TOK_SWITCH && lastStartingKeywordType.back()->type!=TOK_SELECT)
						{
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastStartingKeywordType.back(),lastToken));
						}
					}
                    goto tok_is_keyword_label;
                case TOK_FOR:
					loopCount++;
				    lastStartingKeywordType.push_back(lastToken);
					goto tok_is_keyword_label;
				case TOK_ELSEIF:
					if(lastStartingKeywordType.empty())
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_OUTSIDE_OF_IF,lastToken,currToken));
					}
					else
					{
						if(lastStartingKeywordType.back()->type!=TOK_IF && lastStartingKeywordType.back()->type!=TOK_ELSEIF )
						{
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastStartingKeywordType.back(),lastToken));
						}
					}
					lastStartingKeywordType.push_back(lastToken);
					goto tok_is_keyword_label;
				case TOK_TO:
				case TOK_STEP:
					if(lastStartingKeywordType.empty())
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastToken,currToken));
					}
					else
					{
						if(lastStartingKeywordType.back()->type!=TOK_FOR)
						{
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastStartingKeywordType.back(),lastToken));
						}
					}
					goto operator_not_allowed_label;

				    tok_is_keyword_label:
					// check if starting a keyword is allowed here
				    if (startingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }

                    operator_not_allowed_label:
					// check if our current token is an operator, if yes we generate an error
					switch(currToken->type)
					{
						case TOK_PREPROCESS:
						case TOK_VAR:
						case TOK_LITERAL:
						case TOK_PREDEFINEDVAR:
						case TOK_DEFAULT:
						case TOK_NULL:
						case TOK_BRACKET_OPEN:
						case TOK_FUNC:
                        case TOK_STANDARD_FUNC:
						case TOK_OP_NOT:
						case TOK_OP_ADD:
						case TOK_OP_SUB:
						case TOK_NUMBER_HEX:
						case TOK_NUMBER_DEC:
						case TOK_NUMBER_BIN:
						case TOK_TRUE:
						case TOK_FALSE:
						case TOK_BRACKET_ROUND_OPEN:
						case TOK_PUNCT_NOT:
						case TOK_NEW:
						case TOK_BYREF:
							// all ok
							break;
						case TOK_BRACKET_ROUND_CLOSE:
							if(lastToken->type!=TOK_BRACKET_ROUND_OPEN)
							{
								foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
							}
							break;
						default:
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
							break;
					}
					// if we start a new if/select/etc. or if we are in an term to evaluate no starting or ending keywords are allowed
					// so things like that are not allowed:
					// if true then endif
					// or
					// $var=10+3- endif
					// Correct syntax would be:
					// if true then \n endif
					// or
					// $var=10+3-1 \n endif
					startingKeywordsAllowed=false;
					endingKeywordsAllowed=false;
					break;

                case TOK_ENDWITH:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
                        if(lastStartingKeywordType.back()->type!=TOK_WITH)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
                    }
                    goto tok_is_keyword_end_label;
                case TOK_ENDIF:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
						// because we also push ElseIf and Else tokens to lastStartingKeywordType, we also have to pop all ElseIfs and Elses belonging to last If
						// when we reached an If, we break loop
						while(!lastStartingKeywordType.empty())
						{
							if(lastStartingKeywordType.back()->type==TOK_IF)
							{
								lastStartingKeywordType.pop_back();
								break;
							}
							else if(lastStartingKeywordType.back()->type!=TOK_ELSEIF && lastStartingKeywordType.back()->type!=TOK_ELSE )
							{
								foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
							}
							lastStartingKeywordType.pop_back();
						}
                    }
                    goto tok_is_keyword_end_label;
                case TOK_ENDSELECT:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
                        if(lastStartingKeywordType.back()->type!=TOK_SELECT)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }

						lastStartingKeywordType.pop_back();
                    }
				    goto tok_is_keyword_end_label;
                case TOK_ENDSWITCH:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
                        if(lastStartingKeywordType.back()->type!=TOK_SWITCH)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
                    }
                    goto tok_is_keyword_end_label;
                case TOK_ENDCLASS:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
                        if(lastStartingKeywordType.back()->type!=TOK_CLASS)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
                    }
                    goto tok_is_keyword_end_label;
                case TOK_NEXT:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
						loopCount--;
                        if(lastStartingKeywordType.back()->type!=TOK_FOR)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
                    }
                    goto tok_is_keyword_end_label;
                case TOK_WEND:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
						loopCount--;
                        if(lastStartingKeywordType.back()->type!=TOK_WHILE)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
                    }
                    goto tok_is_keyword_end_label;
                case TOK_ENDFUNC:
                    if (endingKeywordsAllowed==false)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,currToken,lastToken));
                    }
                    if(lastStartingKeywordType.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_UNUSED_ENDKEYWORD,lastToken,nullptr));
                    }
                    else
                    {
                        if(lastStartingKeywordType.back()->type!=TOK_FUNCDECL)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_ENDKEYWORD,lastStartingKeywordType.back(),lastToken));
                        }
                        lastStartingKeywordType.pop_back();
						funcCount--;
                    }
					goto tok_is_keyword_end_label;
                case TOK_ELSE:
					if(lastStartingKeywordType.empty())
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_OUTSIDE_OF_IF,lastToken,currToken));
					}
					else
					{
						if(lastStartingKeywordType.back()->type!=TOK_IF && lastStartingKeywordType.back()->type!=TOK_ELSEIF )
						{
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_HERE,lastStartingKeywordType.back(),lastToken));
						}
					}
					lastStartingKeywordType.push_back(lastToken);
					goto tok_is_keyword_end_label;
				case TOK_DO:
					loopCount++;
					lastStartingKeywordType.push_back(lastToken);
					goto tok_is_keyword_end_label;
				case TOK_EXIT_LOOP:
				case TOK_CONTINUE:
					if(loopCount<=0)
					{
						foundProblems.push_back(Problem(ERROR_TOKEN_OUTSIDE_OF_LOOP,currToken,lastToken));
					}
					goto tok_is_keyword_end_label;
                case TOK_GEN_EXIT:
                case TOK_JMP_LABEL:
                    tok_is_keyword_end_label:
                    if(currToken->type!=TOK_NEWLINE && currToken->type!=TOK_END)
                    {
                        foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
                    }
					break;

				case TOK_NEWLINE:
					// we are in new line so update/reset used/allowed variables
                    startingKeywordsAllowed=true;
					endingKeywordsAllowed=true;
					staticUsed=false;
					atomicUsed=false;
					constUsed=false;
					vardeclUsed=false;
					volatileUsed=false;
					enumUsed=false;
                    break;

                case TOK_BRACKET_CLOSE:
                    if(lastBracket.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_MISSING_OPEN_BRACKETS,nullptr,lastToken));
                    }
                    else
                    {
                        if(lastBracket.back()->type!=TOK_BRACKET_OPEN)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_CLOSING_BRACKET,lastBracket.back(),lastToken));
                        }
                        lastBracket.pop_back();
                    }
                    break;
				case TOK_BRACKET_ROUND_CLOSE:
                    if(lastBracket.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_MISSING_OPEN_BRACKETS,nullptr,lastToken));
                    }
                    else
                    {
                        if(lastBracket.back()->type!=TOK_BRACKET_ROUND_OPEN)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_CLOSING_BRACKET,lastBracket.back(),lastToken));
                        }
                        lastBracket.pop_back();
                    }
                    break;
                case TOK_BRACKET_ANGLE_CLOSE:
                    if(lastBracket.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_MISSING_OPEN_BRACKETS,nullptr,lastToken));
                    }
                    else
                    {
                        if(lastBracket.back()->type!=TOK_BRACKET_ANGLE_OPEN)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_CLOSING_BRACKET,lastBracket.back(),lastToken));
                        }
                        lastBracket.pop_back();
                    }
                    break;
				case TOK_BRACKET_ARRAY_CLOSE:
                    if(lastBracket.empty())
                    {
                        foundProblems.push_back(Problem(ERROR_MISSING_OPEN_BRACKETS,nullptr,lastToken));
                    }
                    else
                    {
                        if(lastBracket.back()->type!=TOK_BRACKET_ARRAY_OPEN)
                        {
                            foundProblems.push_back(Problem(ERROR_NOT_MATCHING_CLOSING_BRACKET,lastBracket.back(),lastToken));
                        }
                        lastBracket.pop_back();
                    }
                    break;

				case TOK_STATIC:
					if(staticUsed==true)
					{
						// only generate warning when something like:
						// Local static static $var
						// occurs because two times static is no problem at all
						// same for most of the other variable identifiers
						foundProblems.push_back(Problem(WARNING_REMOVABLE_STATEMENT,currToken,lastToken,IMPORTANCE_WARNING));
					}
					staticUsed=true;
					goto only_var_vardecl_allowed;
				case TOK_SYNCHRONIZED:
					if(atomicUsed==true)
					{
						foundProblems.push_back(Problem(WARNING_REMOVABLE_STATEMENT,currToken,lastToken,IMPORTANCE_WARNING));
					}
					atomicUsed=true;
					goto only_var_vardecl_allowed;
				case TOK_VOLATILE:
					if(volatileUsed==true)
					{
						foundProblems.push_back(Problem(WARNING_REMOVABLE_STATEMENT,currToken,lastToken,IMPORTANCE_WARNING));
					}
					volatileUsed=true;
					goto only_var_vardecl_allowed;
				case TOK_CONST:
                    if(constUsed==true)
					{
						foundProblems.push_back(Problem(WARNING_REMOVABLE_STATEMENT,currToken,lastToken,IMPORTANCE_WARNING));
					}
					constUsed=true;
					goto only_var_vardecl_allowed;
                case TOK_ENUM:
					if(enumUsed==true)
					{
						foundProblems.push_back(Problem(WARNING_REMOVABLE_STATEMENT,currToken,lastToken,IMPORTANCE_WARNING));
					}
					enumUsed=true;
					goto only_var_vardecl_allowed;
				case TOK_VARDECL:
					if(vardeclUsed==true)
					{
						// Two variable declarations in one line is not allowed, so this is wrong:
						// Local Global $var
						// but also
						// Local Local $var
						foundProblems.push_back(Problem(ERROR_MORE_THAN_ONE_VARDECL_NOT_ALLOWED,currToken,lastToken));
					}
					vardeclUsed=true;

					only_var_vardecl_allowed:
					// check if our next token is a variable declaration or the variable itself
					switch(currToken->type)
					{
						case TOK_CONST:
						case TOK_VARDECL:
						case TOK_VOLATILE:
						case TOK_SYNCHRONIZED:
						case TOK_STATIC:
						case TOK_VAR:
						case TOK_ENUM:
							// all ok
							break;
						default:
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
							break;
					}
					break;

				case TOK_DEFAULT:
				case TOK_NULL:
				case TOK_LITERAL:
				case TOK_PREDEFINEDVAR:
				case TOK_NUMBER_HEX:
				case TOK_NUMBER_DEC:
				case TOK_NUMBER_BIN:
				case TOK_TRUE:
				case TOK_FALSE:
				case TOK_OP_INC:
				case TOK_OP_DEC:
					switch(currToken->type)
					{
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
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
							break;
						default:
							//all ok
							break;
					}
					goto only_operator_allowed_label;
				case TOK_VAR:
                case TOK_COMMA_VAR:
					only_operator_allowed_label:
					switch(currToken->type)
					{
						case TOK_PREPROCESS:
						case TOK_VAR:
						case TOK_LITERAL:
						case TOK_PREDEFINEDVAR:
						case TOK_DEFAULT:
						case TOK_NULL:
						case TOK_BRACKET_OPEN:
						case TOK_FUNC:
						case TOK_NUMBER_HEX:
						case TOK_NUMBER_DEC:
						case TOK_NUMBER_BIN:
						case TOK_TRUE:
						case TOK_FALSE:
						case TOK_BRACKET_ROUND_OPEN:
							foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
						break;
						default:
							//all ok
						break;
					}
					break;

				case TOK_FUNC:
                case TOK_STANDARD_FUNC:
                    if(checker.isAvailable(lastToken->tokenValue)==false)
                    {
                        maybeUndefinedFunctions.push_back(lastToken);
                    }
                    // missing break is right
				case TOK_PREPROCESS:
					if(currToken->type!=TOK_BRACKET_ROUND_OPEN && currToken->type!=TOK_PUNCT_DOT)
					{
						// after each function and preprocess call there must be an opening round bracket
						foundProblems.push_back(Problem(ERROR_ROUND_OPEN_BRACKET_OR_DOT_EXPECTED,currToken,lastToken));
					}
					break;

                case TOK_UNKNOWN:
				case TOK_PUNCT_COMMENT:
				case TOK_END:
				case TOK_ROOT:
				    break;
			}

			if(currToken->type!=TOK_NEWLINE && currToken->type!=TOK_END)
			{
				switch(lastToken->type)
				{
					case TOK_ELSE:
					case TOK_THEN:
					case TOK_GEN_EXIT:
					case TOK_ENDWITH:
					case TOK_ENDIF:
					case TOK_ENDSELECT:
					case TOK_ENDSWITCH:
					case TOK_NEXT:
					case TOK_WEND:
					case TOK_ENDFUNC:
					case TOK_ENDCLASS:
					case TOK_JMP_LABEL:
						foundProblems.push_back(Problem(ERROR_TOKEN_NOT_ALLOWED_AFTER_TOKEN,currToken,lastToken));
						break;
					default:
						//all ok
						break;
				}
			}
		}

		lastToken=currToken;
	}

	// remove keywords and brackets and generate error if there are keywords/brackets left
	// This happen e.g. if you do something like that
	// $var = 10*(1^(1+2)
	if(lastStartingKeywordType.size()>0)
    {
        for(unsigned int i=0;i<lastStartingKeywordType.size();i++)
        {
             foundProblems.push_back(Problem(ERROR_MISSING_ENDKEYWORD,lastStartingKeywordType[i],nullptr));
        }
    }

    if(lastBracket.size()>0)
    {
        for(unsigned int i=0;i<lastBracket.size();i++)
        {
            foundProblems.push_back(Problem(ERROR_MISSING_CLOSING_BRACKETS,lastBracket[i],nullptr));
        }
    }

    for(unsigned int i=0;i<maybeUndefinedFunctions.size();i++)
    {
        if(checker.isAvailable(maybeUndefinedFunctions[i]->tokenValue)==false)
        {
             foundProblems.push_back(Problem(ERROR_UNKNOWN_FUNCTION,nullptr,maybeUndefinedFunctions[i]));
        }
    }

    for(unsigned int i=0;i<maybeUndefinedClasses.size();i++)
    {
        if(checker.isClassAvailable(maybeUndefinedClasses[i]->tokenValue)==false)
        {
             foundProblems.push_back(Problem(ERROR_UNKNOWN_CLASS,maybeUndefinedClasses[i],nullptr));
        }
    }

	return foundProblems;
}
