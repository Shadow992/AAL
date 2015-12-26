#include "Compiler.h"

Compiler::Compiler()
{
    srand(time(nullptr));
    std::map<std::string,int> created;

    while(created.size()<20)
    {
        std::string tmp=generateRandomString(1);
        if(created.find(tmp)==created.end())
        {
            created[tmp]=1;
        }
    }

    auto it=created.begin();

    STR_END_WHILE_LABEL=it->first;
    ++it;
    STR_START_WHILE_LABEL=it->first;
    ++it;
    STR_END_AND_FALSE_LABEL=it->first;
    ++it;
    STR_END_AND_TRUE_LABEL=it->first;
    ++it;
    STR_END_OR_FALSE_LABEL=it->first;
    ++it;
    STR_END_OR_TRUE_LABEL=it->first;
    ++it;
    STR_END_TERNARY_LABEL=it->first;
    ++it;
    STR_TERNARY_FALSE_LABEL=it->first;
    ++it;
    STR_START_DO_LABEL=it->first;
    ++it;
    STR_END_IF_LABEL=it->first;
    ++it;
    STR_NEXT_IF_LABEL=it->first;
    ++it;
	STR_END_LOOP_LABEL=it->first;
	++it;
	STR_END_FUNC_LABEL=it->first;
	++it;
}

Compiler::~Compiler()
{
    //dtor
}

/**
 *  \brief This function generates a few temporary names for "temporary variables" (these variables only holds temporary results)
 */
void Compiler::generateTmpVars()
{
    for(int i=0; i<20; i++)
    {
        unusedVars.push_back("$2"+toString(lastTmpVar));
        lastTmpVar++;
    }
}

/**
 *  \brief This function generates a few temporary names for "temporary pointer" (these variables only holds temporary pointer)
 */
void Compiler::generateTmpPointer()
{
    for(int i=0; i<20; i++)
    {
        unusedPointer.push_back("$3"+toString(lastTmpPointer));
        lastTmpPointer++;
    }
}

/**
 *  \brief This function sets a temporary variable or pointer to unused (so it can be used again in ByteCode)
 *  \param var Variable to set to unused
 */
void Compiler::setVarUnused(const std::string& var)
{
    if(var.length()>1 &&  var[0]=='$')
    {
        if(var[1]=='2' && std::find(unusedVars.begin(), unusedVars.end(), var) == unusedVars.end() )
        {
            unusedVars.push_front(var);
        }
        else if(var[1]=='3' && std::find(unusedPointer.begin(), unusedPointer.end(), var) == unusedPointer.end() )
        {
            unusedPointer.push_front(var);
        }
    }
}

/**
 *  \brief This function renames all variables specified in original AAL-Code (e.g. "$var=10") to a variable which only contains numbers (e.g. "$10") 
 *  \param currToken Variable to set to unused
 *  \param renamedVars Variable to set to unused
 *  \param cRenamedVars Number of renamed variables
 */
void Compiler::renameVars(Token* currToken,std::map<std::string,int>& renamedVars,int& cRenamedVars)
{
    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        renameVars(*it,renamedVars,cRenamedVars);
    }

    if(currToken->type==TOK_VAR)
    {
        auto it=renamedVars.find(currToken->tokenValue);
        if( it== renamedVars.end())
        {
            renamedVars[currToken->tokenValue]=cRenamedVars;
            currToken->tokenValue="$1"+toString(cRenamedVars);
            cRenamedVars++;
        }
        else
        {
            currToken->tokenValue="$1"+toString(it->second);
        }
    }
}

void Compiler::extractFunctions(Token* currToken,std::map<std::string,std::string>& renamedFuncs)
{
    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        extractFunctions(*it,renamedFuncs);
    }

	if(currToken->type==TOK_FUNCDECL)
    {
		if(currToken->childTokens.size()>=0)
		{
			auto it=currToken->childTokens.begin();
			renamedFuncs[(*it)->tokenValue]=hasher.oneWayFunction((*it)->tokenValue);
		}
	}
}

void Compiler::renameFunctions(Token* currToken,std::map<std::string,std::string>& renamedFuncs)
{
    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        renameFunctions(*it,renamedFuncs);
    }

    if(currToken->type==TOK_FUNC)
    {
        auto it=renamedFuncs.find(currToken->tokenValue);
        if( it != renamedFuncs.end())
        {
			currToken->tokenValue=it->second;
        }
        else
        {
            std::transform(currToken->tokenValue.begin(),currToken->tokenValue.end(),currToken->tokenValue.begin(),tolower);
        }
    }
}

std::string Compiler::getNextTmpVar()
{
    if(unusedVars.empty())
    {
        generateTmpVars();
    }
    std::string tmp=unusedVars.front();
    unusedVars.pop_front();
    return tmp;
}

std::string Compiler::getNextTmpPointer()
{
    if(unusedPointer.empty())
    {
        generateTmpPointer();
    }
    std::string tmp=unusedPointer.front();
    unusedPointer.pop_front();
    return tmp;
}

bool Compiler::validArgumentCount(Token* currToken)
{
    switch(currToken->type)
    {
    case TOK_ROOT:
    case TOK_NEWLINE:
    case TOK_END:
        return true;

    case TOK_OP_MOD:
    case TOK_OP_MUL:
    case TOK_OP_DIV:
    case TOK_OP_SHL:
    case TOK_OP_SHR:
    case TOK_OP_JOIN:
    case TOK_OP_POW:
    case TOK_OP_ADD:
    case TOK_OP_SUB:
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
    case TOK_COMP_EQCASE:
    case TOK_COMP_EQINCASE:
    case TOK_COMP_LOWEREQ:
    case TOK_COMP_GREATEREQ:
    case TOK_COMP_NOTEQ:
    case TOK_COMP_LESS:
    case TOK_COMP_GREATER:
    case TOK_PUNCT_DOT:
    case TOK_LOGIC_AND:
    case TOK_LOGIC_OR:
    case TOK_DEFAULT_ASSIGN:
    case TOK_PUNCT_COMMA:
    case TOK_BRACKET_ARRAY_ACCESS:
    case TOK_BRACKET_ARRAY_CREATION:
    case TOK_BRACKET_ARRAY_APPEND:
    case TOK_PUNCT_DOUBLEDOT:
    case TOK_PUNCT_QST:
        return currToken->childTokens.size()==2;

    case TOK_ELSEIF:
    case TOK_UNTIL:
    case TOK_WHILE:
    case TOK_IF:
    case TOK_OP_NOT:
    case TOK_CONST:
    case TOK_VARDECL:
    case TOK_VOLATILE:
    case TOK_SYNCHRONIZED:
    case TOK_STATIC:
    case TOK_ENUM:
    case TOK_FUNCDECL:
    case TOK_RETURN:
    case TOK_OP_INC:
    case TOK_OP_DEC:
    case TOK_BYREF:
        return currToken->childTokens.size()==1;

    case TOK_JMP_LABEL:
    case TOK_WEND:
    case TOK_ELSE:
    case TOK_DO:
    case TOK_VAR:
    case TOK_NULL:
    case TOK_NUMBER_BIN:
    case TOK_NUMBER_HEX:
    case TOK_NUMBER_DEC:
    case TOK_PREDEFINEDVAR:
    case TOK_EXIT_LOOP:
    case TOK_LITERAL:
    case TOK_CONTINUE:
    case TOK_BRACKET_OPEN:
    case TOK_BRACKET_CLOSE:
    case TOK_TRUE:
    case TOK_ENDIF:
    case TOK_ENDFUNC:
        return currToken->childTokens.size()==0;

    case TOK_FUNC:
        return true;

    default:
        break;
    }
    return false;
}

std::string Compiler::getAssignVarForToken(Token* currToken,const std::string& str)
{
    if(str!="")
        return str;
    switch(currToken->type)
    {
    case TOK_PUNCT_DOT:
    case TOK_PUNCT_COMMA:
    case TOK_PUNCT_DOUBLEDOT:
    case TOK_PUNCT_QST:
    case TOK_BRACKET_ARRAY_ACCESS:
        return "*"+getNextTmpPointer();

    case TOK_LOGIC_AND:
    case TOK_LOGIC_OR:
    case TOK_OP_MOD:
    case TOK_OP_MUL:
    case TOK_OP_DIV:
    case TOK_OP_SHL:
    case TOK_OP_SHR:
    case TOK_OP_JOIN:
    case TOK_OP_POW:
    case TOK_OP_ADD:
    case TOK_OP_SUB:
    case TOK_COMP_EQCASE:
    case TOK_COMP_EQINCASE:
    case TOK_COMP_LOWEREQ:
    case TOK_COMP_GREATEREQ:
    case TOK_COMP_NOTEQ:
    case TOK_COMP_LESS:
    case TOK_COMP_GREATER:
    case TOK_OP_NOT:
    case TOK_FUNC:
        return getNextTmpVar();

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
    case TOK_CONST:
    case TOK_VARDECL:
    case TOK_VOLATILE:
    case TOK_SYNCHRONIZED:
    case TOK_STATIC:
    case TOK_ENUM:
    case TOK_OP_INC:
    case TOK_OP_DEC:
    case TOK_BRACKET_ARRAY_CREATION:
    case TOK_BRACKET_ARRAY_APPEND:
    return (*(currToken->childTokens.begin()))->tokenValue;

    default:
        break;
    }
    return getNextTmpVar();
}

std::string Compiler::getRightSideForToken(Token* currToken)
{
    std::vector<std::string> childs;
    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        if((*it)->type==TOK_LITERAL)
        {
            childs.push_back(addQuotes((*it)->tokenValue));
        }
        else
        {
            childs.push_back((*it)->tokenValue);
        }
    }

    switch(currToken->type)
    {
	case TOK_FUNC:
	{
		TokenTypes currType=currToken->type;
		if(definedStandardFunctions.isFunctionAvailable(currToken->tokenValue)==true)
		{
			currType=TOK_STANDARD_FUNC;
		}

	    if(childs.size()>0)
        {
            return toString(currType)+" "+currToken->tokenValue+" "+childs[0];
        }
		else
        {
            return toString(currType)+" "+currToken->tokenValue;
        }
	}

    case TOK_OP_MOD:
    case TOK_OP_MUL:
    case TOK_OP_DIV:
    case TOK_OP_SHL:
    case TOK_OP_SHR:
    case TOK_OP_JOIN:
    case TOK_OP_POW:
    case TOK_OP_ADD:
    case TOK_OP_SUB:
    case TOK_COMP_EQCASE:
    case TOK_COMP_EQINCASE:
    case TOK_COMP_LOWEREQ:
    case TOK_COMP_GREATEREQ:
    case TOK_COMP_NOTEQ:
    case TOK_COMP_LESS:
    case TOK_COMP_GREATER:
    case TOK_PUNCT_DOT:
    case TOK_LOGIC_AND:
    case TOK_LOGIC_OR:
    case TOK_PUNCT_COMMA:
    case TOK_BRACKET_ARRAY_ACCESS:
        return toString(currToken->type)+" "+childs[0]+" "+childs[1];

    case TOK_BRACKET_ARRAY_CREATION:
    case TOK_BRACKET_ARRAY_APPEND:
        return toString(currToken->type)+" "+childs[1];

    case TOK_OP_NOT:
    case TOK_CONST:
    case TOK_VARDECL:
    case TOK_VOLATILE:
    case TOK_SYNCHRONIZED:
    case TOK_STATIC:
    case TOK_ENUM:
        return toString(currToken->type)+" "+childs[0];

    case TOK_ASSIGN_MOD:
        return toString(TOK_OP_MOD)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_ADD:
        return toString(TOK_OP_ADD)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_SUB:
        return toString(TOK_OP_SUB)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_MUL:
        return toString(TOK_OP_MUL)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_DIV:
        return toString(TOK_OP_DIV)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_SHL:
        return toString(TOK_OP_SHL)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_SHR:
        return toString(TOK_OP_SHR)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_JOIN:
        return toString(TOK_OP_JOIN)+" "+childs[0]+" "+childs[1];
    case TOK_ASSIGN_POW:
        return toString(TOK_OP_POW)+" "+childs[0]+" "+childs[1];

    case TOK_OP_INC:
        return toString(TOK_OP_ADD)+" "+childs[0]+" 1";
    case TOK_OP_DEC:
        return toString(TOK_OP_SUB)+" "+childs[0]+" 1";

    case TOK_ASSIGN:
    case TOK_DEFAULT_ASSIGN:
        return childs[1];

    default:
        break;
    }
    return "*"+getNextTmpVar();
}

void Compiler::addLine(const std::string& str,std::ofstream& outFile, std::vector<std::string>& compiledLines, bool useVector)
{
    if(useVector)
    {
        compiledLines.push_back(str);
    }
    else
    {
        outFile<<str<<std::endl;
    }
}

void Compiler::addSavedLines(std::ofstream& outFile,std::vector<std::string>& compiledLines, bool useVector)
{
    for(auto it=varsToFree.back().begin(); it!=varsToFree.back().end(); ++it)
    {
        setVarUnused(*it);
    }
    varsToFree.pop_back();

    for(auto it=savedLines.back().begin(); it!=savedLines.back().end(); ++it)
    {
        addLine(*it,outFile,compiledLines,useVector);
    }
    savedLines.pop_back();
}

bool Compiler::isCompileable(Token* currToken)
{
	switch(currToken->type)
    {
		case TOK_ROOT:
		case TOK_NEWLINE:
		case TOK_END:
			return false;
		default:
			return true;
    }
}

void Compiler::compileTokenRecursive(Token* currToken,std::ofstream& outFile,std::vector<std::string>& compiledLines, bool useVector,const std::string& parentAssignVar, int& funcDeclParam)
{
    if(!validArgumentCount(currToken))
    {
        //error, invalid argument count
        std::cout<<"Invalid argument count for "<<currToken->tokenValue<<" found argument count: "<<currToken->childTokens.size()<<std::endl;
    }

    Token* currChildToken;
    std::string tmp;
	// variable which is true if new lines should be appended to buffer and later on written into file (needed for while loops etc.)
    bool useSaveLines=false;

    if(useVector==true)
    {
        useSaveLines=true;
    }

    switch(currToken->type)
    {
	case TOK_FUNCDECL:
	{
		bool hasExactlyOneArg=false;
		lastKeywordNum++;
		jmpLabels.push_back(STR_END_FUNC_LABEL+toCompressedString(lastKeywordNum));
		addLine(STR_JUMP+" "+STR_END_FUNC_LABEL+toCompressedString(lastKeywordNum),outFile,compiledLines,useVector);
		addLine("_func_"+(*(currToken->childTokens.begin()))->tokenValue+":",outFile,compiledLines,useVector);
		//addLine("^",outFile,compiledLines,useVector);
		currToken=*(currToken->childTokens.begin());
		if(currToken->childTokens.size()>0 && (*(currToken->childTokens.begin()))->type!=TOK_PUNCT_COMMA)
		{
			hasExactlyOneArg=true;
		}
		funcDeclParam=0;
		currToken->setToken(TOK_NEWLINE,"");
		compileTokenRecursive(currToken,outFile,compiledLines,useVector,parentAssignVar,funcDeclParam);
		if(hasExactlyOneArg==true)
		{
			addLine((*(currToken->childTokens.begin()))->tokenValue+"=@param",outFile,compiledLines,useVector);
		}
		addLine("ep",outFile,compiledLines,useVector);
		currToken->childTokens.clear();
		funcDeclParam=-1;
		return;
	}
	case TOK_CONTINUE:
		for(int i=jmpLabels.size()-1;i>=0;i--)
		{
			if(jmpLabels[i].find(STR_START_WHILE_LABEL)==0 || jmpLabels[i].find(STR_START_DO_LABEL)==0 )
			{
				tmp=jmpLabels[i];
				break;
			}
		}
		addLine(STR_JUMP+" "+tmp,outFile,compiledLines,useVector);
		return;
	case TOK_EXIT_LOOP:
		lastKeywordNum++;
		jmpEndLoopLabel=STR_END_LOOP_LABEL+toCompressedString(lastKeywordNum);
		addLine(STR_JUMP+" "+jmpEndLoopLabel,outFile,compiledLines,useVector);
		return;
	case TOK_IF:
	case TOK_CLASS:
		addLine("^",outFile,compiledLines,useVector);
		break;
    case TOK_JMP_LABEL:
		// Add "_" to user label so we do not get colisions with generated jump labels
        addLine("_"+currToken->tokenValue+":",outFile,compiledLines,useVector);
        // nothing more to do so return
        return;
    case TOK_PUNCT_QST:
        lastKeywordNum++;
        jmpLabels.push_back(STR_END_TERNARY_LABEL+toCompressedString(lastKeywordNum));
        jmpLabels.push_back(STR_TERNARY_FALSE_LABEL+toCompressedString(lastKeywordNum));
        break;
    case TOK_ELSEIF:
        // we reached end of if/elseif branch before, so before we process next elseif we have to add the following:
        // jmp end_if_label
        // next_if_label:
        tmp=jmpLabels.back();
        jmpLabels.pop_back();
        addLine(STR_JUMP+" "+jmpLabels.back(),outFile,compiledLines,useVector);
        addLine(tmp,outFile,compiledLines,useVector);
        break;
    case TOK_WHILE:
        lastKeywordNum++;
		// ^ is similiar to "push esp" in asm
		// ! is similiar to "pop esp" in asm
		addLine("^",outFile,compiledLines,useVector);
        addLine(STR_JUMP+" "+STR_END_WHILE_LABEL+toCompressedString(lastKeywordNum),outFile,compiledLines,useVector);
        addLine(STR_START_WHILE_LABEL+toCompressedString(lastKeywordNum)+":",outFile,compiledLines,useVector);

        jmpLabels.push_back(STR_END_WHILE_LABEL+toCompressedString(lastKeywordNum));
        jmpLabels.push_back(STR_START_WHILE_LABEL+toCompressedString(lastKeywordNum));
        savedLines.push_back(std::vector<std::string>());
        varsToFree.push_back(std::vector<std::string>());
        useSaveLines=true;
        break;
    case TOK_DO:
        lastKeywordNum++;
		addLine("^",outFile,compiledLines,useVector);
        addLine(STR_START_DO_LABEL+toCompressedString(lastKeywordNum)+":",outFile,compiledLines,useVector);
        jmpLabels.push_back(STR_START_DO_LABEL+toCompressedString(lastKeywordNum));
        break;
    case TOK_LOGIC_AND:
        lastKeywordNum++;
        jmpLabels.push_back(STR_END_AND_TRUE_LABEL+toCompressedString(lastKeywordNum));
        jmpLabels.push_back(STR_END_AND_FALSE_LABEL+toCompressedString(lastKeywordNum));
        break;
    case TOK_LOGIC_OR:
        lastKeywordNum++;
        jmpLabels.push_back(STR_END_OR_FALSE_LABEL+toCompressedString(lastKeywordNum));
        jmpLabels.push_back(STR_END_OR_TRUE_LABEL+toCompressedString(lastKeywordNum));
        break;
    default:
        break;
    }

    std::string assignVar="";
	std::string tmpVar;
    std::string tmpVar2;
    for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
    {
        currChildToken=*it;

        if(currToken->type==TOK_ASSIGN)
        {
            if((*it)->childTokens.empty() && currToken->childTokens.begin()!=it)
            {
                auto it2=currToken->childTokens.begin();
                ++it2;
                if((*it2)->type!=TOK_FUNC)
                {
                    addLine(assignVar+"="+getRightSideForToken(currToken),outFile,compiledLines,useVector);
                }
            }
        }

        if(useSaveLines)
        {
			int newFuncDecl=-1;
			if(currToken->type==TOK_FUNC)
			{
				compileTokenRecursive(currChildToken,outFile,savedLines.back(),true,assignVar,newFuncDecl);
			}
			else
			{
				compileTokenRecursive(currChildToken,outFile,savedLines.back(),true,assignVar,funcDeclParam);
			}
        }
        else
        {
			int newFuncDecl=-1;
			if(currToken->type==TOK_FUNC)
			{
				compileTokenRecursive(currChildToken,outFile,compiledLines,false,assignVar,newFuncDecl);
			}
			else
			{
				compileTokenRecursive(currChildToken,outFile,compiledLines,false,assignVar,funcDeclParam);
			}
        }
		switch(currToken->type)
		{
			case TOK_PUNCT_DOT:
				if(it==currToken->childTokens.begin())
				{
					tmpVar=getAssignVarForToken(currToken,parentAssignVar);
					addLine(tmpVar+"="+getRightSideForToken(currToken),outFile,compiledLines,useVector);
					auto it2=it;
					++it2;
					(*it2)->tokenValue=tmpVar.substr(1);
				}
				break;
			case TOK_LOGIC_AND:
				addLine(STR_JUMP_ON_FALSE+" "+currChildToken->tokenValue+" "+jmpLabels.back(),outFile,compiledLines,useVector);
				break;
			case TOK_LOGIC_OR:
				addLine(STR_JUMP_ON_TRUE+" "+currChildToken->tokenValue+" "+jmpLabels.back(),outFile,compiledLines,useVector);
				break;
			case TOK_ASSIGN:
				if(currToken->childTokens.begin()==it)
				{
					assignVar=(*it)->tokenValue;
				}
				break;
			case TOK_PUNCT_QST:
				if(currToken->childTokens.begin()==it)
				{
					addLine(STR_JUMP_ON_FALSE+" "+(*(currToken->childTokens.begin()))->tokenValue+" "+jmpLabels.back(),outFile,compiledLines,useVector);
				}
				break;
			case TOK_PUNCT_DOUBLEDOT:
				if(currToken->childTokens.begin()==it)
				{
					useSaveLines=true;
					savedLines.push_back(std::vector<std::string>());
					varsToFree.push_back(std::vector<std::string>());
				}
				break;
			case TOK_PUNCT_COMMA:
				if(funcDeclParam>-1)
				{
					if((*it)->tokenValue!="")
					{
						//addLine("_"+toString(funcDeclParam)+":",outFile,compiledLines,useVector);
						addLine((*it)->tokenValue+"=@param",outFile,compiledLines,useVector);
						funcDeclParam++;
					}
				}
				break;
			default:
				break;
		}

    }

    std::vector<std::string> varsToFreeTmp;

	// mark old temporary values after they got used, so we can reuse temporary variables
    if(!useSaveLines)
    {
        for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
        {
            varsToFreeTmp.push_back((*it)->tokenValue);
        }
    }
    else
    {
        for(auto it=currToken->childTokens.begin(); it!=currToken->childTokens.end(); ++it)
        {
            if((*it)->tokenValue.length()>1 && (*it)->tokenValue[1]=='2' && (*it)->tokenValue[0]=='$')
            {
                varsToFree.back().push_back((*it)->tokenValue);
            }
        }
    }

    auto it=currToken->childTokens.begin();
    switch(currToken->type)
    {
    case TOK_PUNCT_QST:
        ++it;
        tmpVar=getAssignVarForToken(currToken,parentAssignVar);
        addLine(tmpVar+"="+(*it)->tokenValue,outFile,compiledLines,useVector);
        currToken->setToken(TOK_VAR,tmpVar.substr(1));
        break;
    case TOK_PUNCT_DOUBLEDOT:
        tmpVar=getAssignVarForToken(currToken,parentAssignVar);
        tmp=jmpLabels.back();
        jmpLabels.pop_back();
        addLine(tmpVar+"="+(*it)->tokenValue,outFile,compiledLines,useVector);
        ++it;
        addLine(STR_JUMP+" "+jmpLabels.back(),outFile,compiledLines,useVector);
        addLine(tmp+":",outFile,compiledLines,useVector);

        addLine(tmpVar+"="+(*it)->tokenValue,outFile,compiledLines,useVector);

        addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);
        jmpLabels.pop_back();
        currToken->setToken(TOK_VAR,tmpVar.substr(1));
        break;
    case TOK_LOGIC_AND:
        tmpVar=getAssignVarForToken(currToken,parentAssignVar);
        addLine(tmpVar+"=1",outFile,compiledLines,useVector);
        if(tmpVar[0]=='*')
        {
            currToken->setToken(TOK_VAR,tmpVar.substr(1));
        }
        else
        {
            currToken->setToken(TOK_VAR,tmpVar);
        }
        tmpVar2=jmpLabels.back();
        jmpLabels.pop_back();
        addLine(STR_JUMP+" "+jmpLabels.back(),outFile,compiledLines,useVector);
        addLine(tmpVar2+":",outFile,compiledLines,useVector);
        addLine(tmpVar+"=0",outFile,compiledLines,useVector);
        addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);
        jmpLabels.pop_back();
        break;
    case TOK_LOGIC_OR:
        tmpVar=getAssignVarForToken(currToken,parentAssignVar);
        addLine(tmpVar+"=0",outFile,compiledLines,useVector);
        if(tmpVar[0]=='*')
        {
            currToken->setToken(TOK_VAR,tmpVar.substr(1));
        }
        else
        {
            currToken->setToken(TOK_VAR,tmpVar);
        }
        tmpVar2=jmpLabels.back();
        jmpLabels.pop_back();
        addLine(STR_JUMP+" "+jmpLabels.back(),outFile,compiledLines,useVector);
        addLine(tmpVar2+":",outFile,compiledLines,useVector);
        addLine(tmpVar+"=1",outFile,compiledLines,useVector);
        addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);
        jmpLabels.pop_back();
        break;
    case TOK_WHILE:
        savedLines.back().push_back(STR_JUMP_ON_TRUE+" "+(*it)->tokenValue+" "+jmpLabels.back());
        lastKeywordNum++;
        break;
    case TOK_DO:
        lastKeywordNum++;
        break;
    case TOK_UNTIL:
        addLine(STR_JUMP_ON_FALSE+" "+(*it)->tokenValue+" "+jmpLabels.back(),outFile,compiledLines,useVector);
        jmpLabels.pop_back();
		if(jmpEndLoopLabel!="")
		{
			addLine(jmpEndLoopLabel+":",outFile,compiledLines,useVector);
			jmpEndLoopLabel="";
		}
		// ! is similiar to "pop esp" in asm
		addLine("!",outFile,compiledLines,useVector);
        lastKeywordNum++;
        break;
    case TOK_WEND:
        jmpLabels.pop_back();
        addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);

        addSavedLines(outFile,compiledLines,useVector);
        jmpLabels.pop_back();
		if(jmpEndLoopLabel!="")
		{
			addLine(jmpEndLoopLabel+":",outFile,compiledLines,useVector);
			jmpEndLoopLabel="";
		}
		addLine("!",outFile,compiledLines,useVector);
        lastKeywordNum++;
        break;
    case TOK_IF:
        // we found an if, so generate following code:
        // jmp_on_false next_if_label
        // and push end_if_label and next_if_label onto stack
        lastKeywordNum++;
        jmpLabels.push_back(STR_END_IF_LABEL+toCompressedString(lastKeywordNum));
        jmpLabels.push_back(STR_NEXT_IF_LABEL+toCompressedString(lastKeywordNum));
        addLine(STR_JUMP_ON_FALSE+" "+(*it)->tokenValue+" "+STR_NEXT_IF_LABEL+toCompressedString(lastKeywordNum),outFile,compiledLines,useVector);
        break;
    case TOK_ELSE:
        tmp=jmpLabels.back();
        jmpLabels.pop_back();
        // when we are in an else branch we have to jump after execution to the end of if
        addLine(STR_JUMP+" "+jmpLabels.back(),outFile,compiledLines,useVector);
        // push last next_if_label onto compiledLines
        addLine(tmp+":",outFile,compiledLines,useVector);
        // generate next next_if_label
        lastKeywordNum++;
        jmpLabels.push_back(STR_NEXT_IF_LABEL+toCompressedString(lastKeywordNum));
        break;
    case TOK_ELSEIF:
        // we did most work of elseif before (see lines before recursive call)
        // we only have to add now a jump_on_false and push onto stack next next_if_labels
        lastKeywordNum++;
        addLine(STR_JUMP_ON_FALSE+" "+(*it)->tokenValue+" "+STR_NEXT_IF_LABEL+toCompressedString(lastKeywordNum),outFile,compiledLines,useVector);
        jmpLabels.push_back(STR_NEXT_IF_LABEL+toCompressedString(lastKeywordNum));
        break;
    case TOK_ENDIF:
        // add at the end of if:
        // end_if_label:
        // next_if_label
        addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);
        jmpLabels.pop_back();
        addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);
        jmpLabels.pop_back();
		addLine("!",outFile,compiledLines,useVector);
        break;
	 case TOK_ENDFUNC:
        //addLine("!",outFile,compiledLines,useVector);
		addLine("ret @empty",outFile,compiledLines,useVector);
		addLine(jmpLabels.back()+":",outFile,compiledLines,useVector);
		jmpLabels.pop_back();
        break;
	case TOK_PUNCT_DOT:
		++it;
		tmpVar=(*it)->tokenValue;
		if(tmpVar[0]=='*')
		{
			currToken->setToken(TOK_VAR,tmpVar.substr(1));
		}
		else
		{
			currToken->setToken(TOK_VAR,tmpVar);
		}
		currToken->childTokens.clear();
		break;
	case TOK_ASSIGN:
        // nothing to do
        currToken->setToken(TOK_VAR,(*it)->tokenValue);
        currToken->childTokens.clear();
        break;
	case TOK_BYREF:
		currToken->setToken(TOK_VAR,"*"+(*it)->tokenValue);
		currToken->childTokens.clear();
		break;
    case TOK_RETURN:
        addLine("ret "+(*it)->tokenValue,outFile,compiledLines,useVector);
        break;
	case TOK_PUNCT_COMMA:
		if(funcDeclParam>-1)
		{
			currToken->setToken(TOK_VAR,"");
			break;
		}
        if(currToken->childTokens.size()>0)
        {
            auto itChilds=currToken->childTokens.begin();
            //++itChilds;
            if((*itChilds)->type==TOK_COMMA_VAR)
            {
                tmpVar=(*itChilds)->tokenValue;
            }
            else
            {
                tmpVar=getAssignVarForToken(currToken,parentAssignVar);
            }

            addLine(tmpVar+"="+getRightSideForToken(currToken),outFile,compiledLines,useVector);
            if(tmpVar[0]=='*')
            {
                currToken->setToken(TOK_COMMA_VAR,tmpVar.substr(1));
            }
            else
            {
                currToken->setToken(TOK_COMMA_VAR,tmpVar);
            }
            currToken->childTokens.clear();
        }
		break;
    default:
        // we did not find any special token, so just do standard processing
        if(isCompileable(currToken))
        {
            if(currToken->childTokens.size()>0)
            {
                tmpVar=getAssignVarForToken(currToken,parentAssignVar);
                addLine(tmpVar+"="+getRightSideForToken(currToken),outFile,compiledLines,useVector);
                if(tmpVar[0]=='*')
                {
                    currToken->setToken(TOK_VAR,tmpVar.substr(1));
                }
                else
                {
                    currToken->setToken(TOK_VAR,tmpVar);
                }
                currToken->childTokens.clear();
            }
            else if(currToken->type==TOK_FUNC)
            {
                tmpVar=getAssignVarForToken(currToken,parentAssignVar);
                addLine(tmpVar+"="+getRightSideForToken(currToken),outFile,compiledLines,useVector);
                if(tmpVar[0]=='*')
                {
                    currToken->setToken(TOK_VAR,tmpVar.substr(1));
                }
                else
                {
                    currToken->setToken(TOK_VAR,tmpVar);
                }
                currToken->childTokens.clear();
            }

        }
    }

	// mark old temporary values after they got used, so we can reuse temporary variables
    for(auto it=varsToFreeTmp.begin(); it!=varsToFreeTmp.end(); ++it)
    {
        setVarUnused(*it);
    }

}

void Compiler::writeUsingDirectives(std::ofstream& outFile)
{
	outFile<<STR_USING_DIRCETIVE<<" n "<<toString(hasher.n)<<std::endl;
	outFile<<STR_USING_DIRCETIVE<<" e "<<toString(hasher.e)<<std::endl;
}

bool Compiler::compile(std::string code)
{
#if SHOW_TIMINGS == 1
    auto start = std::chrono::steady_clock::now();
#endif

    insertIncludes(code);

#if SHOW_TIMINGS == 1
    auto end = std::chrono::steady_clock::now();
    auto diff = end - start;
    std::cout << "Insert Includes: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
    start = std::chrono::steady_clock::now();
#endif

    bool success=parser.parse(code);
    code.clear();
    if(success==false)
    {
        return false;
    }
    Token* rootToken=&(parser.rootToken);

#if SHOW_TIMINGS == 1
    start = std::chrono::steady_clock::now();
#endif

    std::vector<Token*> extractedPreProcessTokens;
    extractPreProcess(rootToken,extractedPreProcessTokens);

    processPreProcessTokens(extractedPreProcessTokens);

    std::map<std::string,int> renamedVars;
    int cRenamedVars=0;
    renameVars(rootToken,renamedVars,cRenamedVars);

	std::map<std::string,std::string> renamedFuncs;
	extractFunctions(rootToken,renamedFuncs);
	renameFunctions(rootToken,renamedFuncs);

#if SHOW_TIMINGS == 1
    end = std::chrono::steady_clock::now();
    diff = end - start;
    std::cout << "Preprocessing: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
    start = std::chrono::steady_clock::now();
#endif

#if DEBUG>0
    std::cout<<"Created preproccessed Syntaxtree: "<<std::endl;
    printSyntaxTree(rootToken,1);
#endif

#if DEBUG > 1
    std::cout<<"Preprocessed tokens:"<<std::endl;
    for(auto it=extractedPreProcessTokens.begin(); it!=extractedPreProcessTokens.end(); ++it)
    {
        printSyntaxTree(*it,1);
    }
#endif

    Optimizer optimizer;
    optimizer.optimize(rootToken,1);

#if SHOW_TIMINGS == 1
    start = std::chrono::steady_clock::now();
#endif

    remove("out.tmp");
    std::vector<std::string> compiledLines;
    std::ofstream outFile("out.tmp", std::ios::app | std::ios::out);
	int funcDeclParam=-1;

	writeUsingDirectives(outFile);
    compileTokenRecursive(rootToken,outFile,compiledLines,false,"",funcDeclParam);
    outFile.close();

#if SHOW_TIMINGS == 1
    end = std::chrono::steady_clock::now();
    diff = end - start;
    std::cout << "Compilation: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
    start = std::chrono::steady_clock::now();
#endif

#if DEBUG>2
    std::cout<<"SyntaxTree after compilation: "<<std::endl;
    printSyntaxTree(rootToken,1);
#endif

    return true;
}
