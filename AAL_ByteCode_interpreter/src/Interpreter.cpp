#include "Interpreter.h"

Interpreter::Interpreter(const std::string& code)
{
    aalStandardFunctions=new StandardFunctions;
    ui=new UserInterface;
    parser.parseBytecode(code);
}

Interpreter::~Interpreter()
{
    delete ui;
    delete aalStandardFunctions;
}

/**
* \brief  This method parses a given Bytecode-Line and converts the first and second argument to double or long long (whatever fits better).
*
* \param bytecode [in] Bytecode-Line to parse
* \param type [out] type used, can be TYPE_DOUBLE or TYPE_LONG
* \param tmpDouble1 [out] if type set to TYPE_DOUBLE this variable will contain first argument converted to double
* \param tmpDouble2 [out] if type set to TYPE_DOUBLE this variable will contain second argument converted to double
* \param tmpLong1 [out] if type set to TYPE_LONG this variable will contain first argument converted to long long
* \param tmpLong2 [out] if type set to TYPE_LONG this variable will contain second argument converted to long long
*
*/
void Interpreter::extractValuesForOperations(BytecodeLine* bytecode, char& type, double& tmpDouble1, double& tmpDouble2, long long& tmpLong1, long long& tmpLong2)
{
    if(isTypeVar(bytecode->arg1Type))
    {
        tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
        if(tmpArg1Var->isDouble())
        {
            tmpDouble1=tmpArg1Var->getDoubleValue(0);
            type=TYPE_DOUBLE;
        }
        else
        {
            tmpLong1=tmpArg1Var->getLongValue(0);
            type=TYPE_LONG;
        }
    }
    else
    {
        if(bytecode->arg1Type==TYPE_DOUBLE)
        {
            tmpDouble1=bytecode->arg1Double;
            type=TYPE_DOUBLE;
        }
        else
        {
            tmpLong1=bytecode->arg1Long;
            type=TYPE_LONG;
        }
    }


    if(isTypeVar(bytecode->arg2Type))
    {
        tmpArg2Var=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
        if(type==TYPE_DOUBLE)
        {
            tmpDouble2=tmpArg2Var->getDoubleValue(0);
        }
        else if(tmpArg2Var->isDouble())
        {
            tmpDouble2=tmpArg2Var->getDoubleValue(0);
            tmpDouble1=tmpLong1;
            type=TYPE_DOUBLE;
        }
        else
        {
            tmpLong2=tmpArg2Var->getLongValue(0);
        }
    }
    else
    {
        if(type==TYPE_DOUBLE)
        {
            tmpDouble2=bytecode->arg2Double;
        }
        else if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            tmpDouble1=tmpLong1;
            tmpDouble2=bytecode->arg2Double;
            type=TYPE_DOUBLE;
        }
        else
        {
            tmpLong2=bytecode->arg2Long;
        }
    }
}

/**
* \brief  This method parses a given Bytecode-Line and converts the first and second argument to double, long long or string (whatever fits better).
*
* \param bytecode [in] Bytecode-Line to parse
* \param type [out] type used, can be TYPE_DOUBLE, TYPE_LONG or TYPE_STRING
* \param tmpDouble1 [out] if type set to TYPE_DOUBLE this variable will contain first argument converted to double
* \param tmpDouble2 [out] if type set to TYPE_DOUBLE this variable will contain second argument converted to double
* \param tmpLong1 [out] if type set to TYPE_LONG this variable will contain first argument converted to long long
* \param tmpLong2 [out] if type set to TYPE_LONG this variable will contain second argument converted to long long
* \param tmpString1 [out] if type set to TYPE_STRING this variable will contain second argument converted to string
* \param tmpString2 [out] if type set to TYPE_STRING this variable will contain second argument converted to string
*
*/
void Interpreter::extractValues(BytecodeLine* bytecode,char& type,double& tmpDouble1, double& tmpDouble2,long long& tmpLong1, long long& tmpLong2, std::string& tmpString1, std::string& tmpString2)
{
    if(isTypeVar(bytecode->arg1Type))
    {
        tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
        if(tmpArg1Var->isDouble())
        {
            tmpDouble1=tmpArg1Var->getDoubleValue(0);
            type=TYPE_DOUBLE;
        }
        else if(tmpArg1Var->isString())
        {
            tmpString1=tmpArg1Var->getStringValue(0);
            type=TYPE_STRING;
        }
        else
        {
            tmpLong1=tmpArg1Var->getLongValue(0);
            type=TYPE_LONG;
        }
    }
    else
    {
        if(bytecode->arg1Type==TYPE_DOUBLE)
        {
            tmpDouble1=bytecode->arg1Double;
            type=TYPE_DOUBLE;
        }
        else if(bytecode->arg1Type==TYPE_STRING)
        {
            tmpString1=bytecode->arg1;
            type=TYPE_STRING;
        }
        else
        {
            tmpLong1=bytecode->arg1Long;
            type=TYPE_LONG;
        }
    }

    if(isTypeVar(bytecode->arg2Type))
    {
        tmpArg2Var=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
        if(type==TYPE_DOUBLE)
        {
            tmpDouble2=tmpArg2Var->getDoubleValue(0);
        }
        else if(type==TYPE_LONG && tmpArg2Var->isDouble())
        {
            tmpDouble2=tmpArg2Var->getDoubleValue(0);
            tmpDouble1=tmpLong1;
            type=TYPE_DOUBLE;
        }
        else if(type==TYPE_LONG && tmpArg2Var->isLong())
        {
            tmpLong2=tmpArg2Var->getLongValue(0);
        }
        else
        {
            tmpString2=tmpArg2Var->getStringValue(0);
        }
    }
    else
    {
        if(type==TYPE_DOUBLE)
        {
            tmpDouble2=bytecode->arg2Double;
        }
        else if(type==TYPE_LONG && bytecode->arg2Type==TYPE_DOUBLE)
        {
            tmpDouble1=tmpLong1;
            tmpDouble2=bytecode->arg2Double;
            type=TYPE_DOUBLE;
        }
        else if(type==TYPE_LONG && bytecode->arg2Type==TYPE_LONG)
        {
            tmpLong2=bytecode->arg2Long;
        }
        else
        {
            if(bytecode->arg2Type==TYPE_DOUBLE)
            {
                tmpString2=toString(bytecode->arg2Double);
            }
            else if(bytecode->arg2Type==TYPE_STRING)
            {
                tmpString2=bytecode->arg2;
            }
            else
            {
                tmpString2=toString(bytecode->arg2Long);
            }
        }
    }
}

/**
* \brief  This method parses a given Bytecode-Line and converts the first and second argument to long long.
*
* \param bytecode [in] Bytecode-Line to parse
* \param tmpLong1 [out] this variable will contain first argument converted to long long
* \param tmpLong2 [out] this variable will contain second argument converted to long long
*
*/
void Interpreter::extractLongsForOperations(BytecodeLine* bytecode, long long& tmpLong1, long long& tmpLong2)
{
    if(isTypeVar(bytecode->arg1Type))
    {
        tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
        tmpLong1=tmpArg1Var->getLongValue(0);
    }
    else
    {
        if(bytecode->arg1Type==TYPE_DOUBLE)
        {
            tmpLong1=(long long) bytecode->arg1Double;
        }
        else if(bytecode->arg1Type==TYPE_STRING)
        {
            tmpLong1=0;
        }
        else
        {
            tmpLong1=bytecode->arg1Long;
        }
    }

    if(isTypeVar(bytecode->arg2Type))
    {
        tmpArg2Var=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
        tmpLong2=tmpArg2Var->getLongValue(0);
    }
    else
    {
        if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            tmpLong2=(long long) bytecode->arg2Double;
        }
        else if(bytecode->arg2Type==TYPE_STRING)
        {
            tmpLong2=0;
        }
        else
        {
            tmpLong2=bytecode->arg2Long;
        }
    }
}

/**
* \brief  This method parses a given Bytecode-Line and converts the first or second argument (but not both at the same time) to long long.
*
* \param bytecode [in] Bytecode-Line to parse
* \param tmpLong1 [out] this variable will contain first or second argument converted to long long
* \param getArg1 [in] true if first argument should be converted, false if second should be converted
*
*/
void Interpreter::extractLongsForOperations(BytecodeLine* bytecode, long long& tmpLong1, const bool getArg1)
{
    if(getArg1)
    {
        if(isTypeVar(bytecode->arg1Type))
        {
            tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
            tmpLong1=tmpArg1Var->getLongValue(0);
        }
        else
        {
            if(bytecode->arg1Type==TYPE_DOUBLE)
            {
                tmpLong1=(long long) bytecode->arg1Double;
            }
            else if(bytecode->arg1Type==TYPE_STRING)
            {
                tmpLong1=0;
            }
            else
            {
                tmpLong1=bytecode->arg1Long;
            }
        }
    }
    else
    {
        if(isTypeVar(bytecode->arg2Type))
        {
            tmpArg2Var=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
            tmpLong1=tmpArg2Var->getLongValue(0);
        }
        else
        {
            if(bytecode->arg2Type==TYPE_DOUBLE)
            {
                tmpLong1=(long long) bytecode->arg2Double;
            }
            else if(bytecode->arg1Type==TYPE_STRING)
            {
                tmpLong1=0;
            }
            else
            {
                tmpLong1=bytecode->arg2Long;
            }
        }
    }
}

/**
* \brief  This method parses a given Bytecode-Line and converts the first and second argument to double.
*
* \param bytecode [in] Bytecode-Line to parse
* \param tmpDouble1 [out] this variable will contain first argument converted to double
* \param tmpDouble2 [out] this variable will contain second argument converted to double
*
*/
void Interpreter::extractDoublesForOperations(BytecodeLine* bytecode, double& tmpDouble1, double& tmpDouble2)
{
    if(isTypeVar(bytecode->arg1Type))
    {
        tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
        tmpDouble1=tmpArg1Var->getDoubleValue(0);
    }
    else
    {
        if(bytecode->arg1Type==TYPE_DOUBLE)
        {
            tmpDouble1=bytecode->arg1Double;
        }
        else if(bytecode->arg1Type==TYPE_STRING)
        {
            tmpDouble1=0.0;
        }
        else
        {
            tmpDouble1=bytecode->arg1Long;
        }
    }

    if(isTypeVar(bytecode->arg2Type))
    {
        tmpArg2Var=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
        tmpDouble2=tmpArg2Var->getDoubleValue(0);
    }
    else
    {
        if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            tmpDouble2=bytecode->arg2Double;
        }
        else if(bytecode->arg2Type==TYPE_STRING)
        {
            tmpDouble2=0.0;
        }
        else
        {
            tmpDouble2=bytecode->arg2Long;
        }
    }
}

/**
* \brief  This method parses a given Bytecode-Line and converts the first and second argument to string.
*
* \param bytecode [in] Bytecode-Line to parse
* \param tmpString1 [out] this variable will contain first argument converted to string
* \param tmpString2 [out] this variable will contain second argument converted to string
*
*/
void Interpreter::extractStringsForOperations(BytecodeLine* bytecode, std::string& tmpString1, std::string& tmpString2)
{
    if(isTypeVar(bytecode->arg1Type))
    {
        tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
        tmpString1=(tmpArg1Var->getStringValue(0));
    }
    else
    {
        if(bytecode->arg1Type==TYPE_DOUBLE)
        {
            tmpString1=toString(bytecode->arg1Double);
        }
        else if(bytecode->arg1Type==TYPE_STRING)
        {
            tmpString1=bytecode->arg1;
        }
        else
        {
            tmpString1=toString(bytecode->arg1Long);
        }
    }

    if(isTypeVar(bytecode->arg2Type))
    {
        tmpArg2Var=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
        tmpString2=(tmpArg2Var->getStringValue(0));
    }
    else
    {
        if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            tmpString2=toString(bytecode->arg2Double);
        }
        else if(bytecode->arg2Type==TYPE_STRING)
        {
            tmpString2=bytecode->arg2;
        }
        else
        {
            tmpString2=toString(bytecode->arg2Long);
        }
    }
}

/**
* \brief  This method adds the given arguments (for function call) of Bytecode-Line to a variable (arg1 and arg2 if arg1!=assignVar, otherwise only arg2).
*         Before calling a function you have to save all arguments (if there are more than one argument) in an extra variable.
*         After doing this you pass the variable (holding all arguments) to the function you want to call.
*
* \param assignVar this variable gets the parameter appended
* \param bytecode bytecode to parse
*
*/
void Interpreter::addToVariableAsParameter(AalVariable* assignVar, BytecodeLine* bytecode)
{
    if(isTypeVar(bytecode->assignVarType) && isTypeVar(bytecode->arg1Type) && bytecode->assignVarLong==bytecode->arg1Long && bytecode->assignVarType==bytecode->arg1Type )
    {
        if(isTypeVar(bytecode->arg2Type))
        {
            assignVar->appendAsParameter(varManager.getVar(bytecode->arg2Long,bytecode->arg2Type));
        }
        else if(bytecode->arg2Type==TYPE_STRING)
        {
            assignVar->appendAsParameter(bytecode->arg2);
        }
        else if(bytecode->arg2Type==TYPE_LONG)
        {
            assignVar->appendAsParameter(bytecode->arg2Long);
        }
        else if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            assignVar->appendAsParameter(bytecode->arg2Double);
        }
    }
    else
    {
        assignVar->clear();
        if(isTypeVar(bytecode->arg1Type))
        {
            assignVar->appendAsParameter(varManager.getVar(bytecode->arg1Long,bytecode->arg1Type));
        }
        else if(bytecode->arg1Type==TYPE_STRING)
        {
            assignVar->appendAsParameter(bytecode->arg1);
        }
        else if(bytecode->arg1Type==TYPE_LONG)
        {
            assignVar->appendAsParameter(bytecode->arg1Long);
        }
        else if(bytecode->arg1Type==TYPE_DOUBLE)
        {
            assignVar->appendAsParameter(bytecode->arg1Double);
        }

        if(isTypeVar(bytecode->arg2Type))
        {
            assignVar->appendAsParameter(varManager.getVar(bytecode->arg2Long,bytecode->arg2Type));
        }
        else if(bytecode->arg2Type==TYPE_STRING)
        {
            assignVar->appendAsParameter(bytecode->arg2);
        }
        else if(bytecode->arg2Type==TYPE_LONG)
        {
            assignVar->appendAsParameter(bytecode->arg2Long);
        }
        else if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            assignVar->appendAsParameter(bytecode->arg2Double);
        }
    }

    //for(int i=0;i<parameterStack.back()->size();i++)
//std::cout<<(*(parameterStack.back()))[i]->getStringValue()<<std::endl;
}

/**
* \brief  This method tries to call a userdefined function.
*
* \param returnVar this variable will hold the return value of function
* \param bytecode bytecode of userdefined function call
*
* \return false if no userdefined function with given name exists, otherwise true
*/
bool Interpreter::callUserFunction(AalVariable* returnVar, BytecodeLine* bytecode)
{
    BytecodeLine* tempLine;
    int currLineBefore=currLine;
    if(bytecode->arg1Type==TYPE_LONG)
    {
        currLine=bytecode->arg1Long;
    }
    else
    {
        privateClassTmpString1=STR_FUNC_PREFIX;
        privateClassTmpString1+=bytecode->arg1;
        auto it=parser.jmpLabels.find(privateClassTmpString1);
        if(it!=parser.jmpLabels.end())
        {
            currLine=it->second;
            bytecode->arg1Type=TYPE_LONG;
            bytecode->arg1Long=it->second;
        }
        else
        {
            bytecode->commandType=TOK_STANDARD_FUNC;
            return false;
        }
    }

    AalVariable* parameterVar=allocator.allocateAalVar();
    if(isTypeVar(bytecode->arg2Type))
    {
        AalVariable* passedVar=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);

        if(passedVar->arrayInfo!=nullptr || passedVar->getArgumentCount()<2)
        {
            parameterVar->appendAsParameter(passedVar);
        }
        else
        {
            parameterVar->makePointTo(passedVar);
        }

    }
    else if(bytecode->arg2Type!=TYPE_UNKNOWN)
    {
        if(bytecode->arg2Type==TYPE_LONG)
        {
            parameterVar->appendAsParameter(bytecode->arg2Long);
        }
        else if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            parameterVar->appendAsParameter(bytecode->arg2Double);
        }
        else
        {
            parameterVar->appendAsParameter(bytecode->arg2);
        }
    }


    //parameterVar->printVarRecursive();

    varManager.enterFunction(currLineBefore);
    int cLines=(int)parser.parsedLinesVector.size();
    int currIndex=0;
    Message msg;
    Window* currWindow;
    unsigned int iterations=0;
    while(currLine<cLines)
    {
        iterations++;
        if((iterations & 15) == 0 && ui->msgAvailable>0)
        {
            for(currWindow=ui->getNextWindow(); currWindow!=nullptr; currWindow=ui->getNextWindow())
            {
                if(ui->popMessageForWindow(currWindow,msg)==true)
                {
                    if(msg.action!=nullptr)
                    {
                        // call function
                        // std::cout<<"Clicked: "<<msg.currentEvent<<" "<<*((std::string*)msg.action)<<std::endl;
                        privateClassTmpString1 = hasher.oneWayFunction(*((std::string*)msg.action));
                        tempLine=new BytecodeLine;
                        tempLine->type=LINE_COMMAND;
                        tempLine->commandType=TOK_FUNC;
                        tempLine->arg1=privateClassTmpString1;
                        tempLine->arg1Type=TYPE_STRING;
                        callUserFunction(nullptr,tempLine);
                        delete tempLine;
                    }
                }
            }
        }

        bytecode=parser.parsedLinesVector[currLine];
#if DEBUG > 0
        std::cout<<"["<<currLine<<"] "<<bytecode->type<<" "<<bytecode->assignVar<<" "<<parameterVar<<std::endl;
#endif // DEBUG

        //varManager.printVars();
        if(parameterVar==nullptr)
        {
            if(bytecode->type!=LINE_RET)
            {
                if(bytecode->arg1!=STR_PARAM)
                {
                    interpretLine(bytecode);
                }
            }
            else
            {
                // set return value
                if(returnVar!=nullptr)
                {
                    if(isTypeVar(bytecode->arg1Type))
                    {
                        AalVariable *tmpReturnVar=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
                        returnVar->setValue(tmpReturnVar);
                    }
                    else if(bytecode->arg1Type!=TYPE_UNKNOWN)
                    {
                        if(bytecode->arg1Type==TYPE_LONG)
                        {
                            returnVar->setValue(bytecode->arg1Long,TYPE_LONG);
                        }
                        else if(bytecode->arg1Type==TYPE_DOUBLE)
                        {
                            returnVar->setValue(bytecode->arg1Double,TYPE_DOUBLE);
                        }
                        else if(bytecode->arg1Type==TYPE_STRING)
                        {
                            returnVar->setValue(bytecode->arg1,TYPE_STRING);
                        }
                    }
                }
                varManager.leaveFunction(currLine);
                break;
            }
        }
        else if(bytecode->type==LINE_ENDPARAM && parameterVar!=nullptr)
        {
            allocator.recycleAalVar(parameterVar);
            parameterVar=nullptr;
        }
        else if(bytecode->arg1==STR_PARAM)
        {
            //std::cout<<"varssign: "<<bytecode->assignVarLong<<std::endl;
            AalVariable* assignVar = varManager.getVar(bytecode->assignVarLong, TYPE_VARIABLE,true);
            if(bytecode->assignVarIsPointer==true)
            {
                switch(parameterVar->getType(currIndex))
                {
                case TYPE_STRING:
                    assignVar->setValue(parameterVar->getStringValue(currIndex),TYPE_STRING,0);
                    break;
                case TYPE_LONG:
                    assignVar->setValue(parameterVar->getLongValue(currIndex),TYPE_LONG,0);
                    break;
                case TYPE_DOUBLE:
                    assignVar->setValue(parameterVar->getDoubleValue(currIndex),TYPE_DOUBLE,0);
                    break;
                case TYPE_VARIABLE:
                    assignVar->makePointTo(parameterVar->getVariablePointer(currIndex));
                    break;
                default:
                    break;
                }
            }
            else
            {
                switch(parameterVar->getType(currIndex))
                {
                case TYPE_STRING:
                    assignVar->setValue(parameterVar->getStringValue(currIndex),TYPE_STRING,0);
                    break;
                case TYPE_LONG:
                    assignVar->setValue(parameterVar->getLongValue(currIndex),TYPE_LONG,0);
                    break;
                case TYPE_DOUBLE:
                    assignVar->setValue(parameterVar->getDoubleValue(currIndex),TYPE_DOUBLE,0);
                    break;
                case TYPE_VARIABLE:
                    assignVar->setValue(parameterVar->getVariablePointer(currIndex),0,0);
                    break;
                default:
                    break;
                }
            }

            //std::cout<<"param: "<<assignVar->getStringValue()<<" "<<parameterVar->getStringValue()<<std::endl;
            currIndex++;
            if(parameterVar->getArgumentCount() <= (unsigned)currIndex && parameterVar!=nullptr)
            {
                allocator.recycleAalVar(parameterVar);
                parameterVar=nullptr;
            }
        }
        currLine++;
    }
    return true;
}

/**
* \brief  This method tries to call a standard function.
*
* \param returnVar this variable will hold the return value of function
* \param bytecode bytecode of standard function call
*
* \return false if no standard function with given name exists, otherwise true
*/
bool Interpreter::callStandardFunction(AalVariable* returnVar, BytecodeLine* bytecode)
{
    AalVariable* parameterVar=allocator.allocateAalVar();
    if(isTypeVar(bytecode->arg2Type))
    {
        AalVariable* passedVar=varManager.getVar(bytecode->arg2Long, bytecode->arg2Type);
        if(passedVar->arrayInfo!=nullptr || passedVar->getArgumentCount()<2)
        {
            parameterVar->appendAsParameter(passedVar);
        }
        else
        {
            parameterVar->makePointTo(passedVar);
        }
    }
    else if(bytecode->arg2Type!=TYPE_UNKNOWN)
    {
        if(bytecode->arg2Type==TYPE_LONG)
        {
            parameterVar->appendAsParameter(bytecode->arg2Long);
        }
        else if(bytecode->arg2Type==TYPE_DOUBLE)
        {
            parameterVar->appendAsParameter(bytecode->arg2Double);
        }
        else if(bytecode->arg2Type==TYPE_STRING)
        {
            parameterVar->appendAsParameter(bytecode->arg2);
        }
    }

    bool ret=aalStandardFunctions->executeFunction(bytecode->arg1,parameterVar,returnVar,this);
    allocator.recycleAalVar(parameterVar);

    return ret;
}

/**
* \brief  This method interprets a command (e.g. TOK_OP_MOD, TOK_FUNC, ...) but not something like LINE_JMP, LINE_PUSH, ... (for these lines you must use Interpreter::interpretLine(BytecodeLine* bytecode)).
*
* \param bytecode Bytecode-Line to interpret command
*
*/
void Interpreter::interpretCommand(BytecodeLine* bytecode)
{
    AalVariable* assignVar;

    if(bytecode->commandType==TOK_BRACKET_ARRAY_CREATION)
    {
        assignVar=varManager.getVar(bytecode->assignVarLong, bytecode->assignVarType,true);
    }
    else
    {
        assignVar=varManager.getVar(bytecode->assignVarLong, bytecode->assignVarType,false);
    }


    switch(bytecode->commandType)
    {
    case TOK_PUNCT_COMMA:
        addToVariableAsParameter(assignVar,bytecode);
        break;
    case TOK_FUNC:
        callUserFunction(assignVar,bytecode);
        break;
    case TOK_STANDARD_FUNC:
        callStandardFunction(assignVar,bytecode);
        break;
    case TOK_OP_MOD:
        extractLongsForOperations(bytecode, privateClassTmpLong1, privateClassTmpLong2);
        assignVar->mod(privateClassTmpLong1,privateClassTmpLong2);
        break;
    case TOK_OP_ADD:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        //std::cout<<"add: "<<privateClassTmpLong1<<"+"<<privateClassTmpLong2<<std::endl;
        if(privateClassType==TYPE_DOUBLE)
        {
            assignVar->add(privateClassTmpDouble1,privateClassTmpDouble2);
        }
        else
        {
            assignVar->add(privateClassTmpLong1,privateClassTmpLong2);
        }
        break;
    case TOK_OP_SUB:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        if(privateClassType==TYPE_DOUBLE)
        {
            assignVar->sub(privateClassTmpDouble1,privateClassTmpDouble2);
        }
        else
        {
            assignVar->sub(privateClassTmpLong1,privateClassTmpLong2);
        }
        break;
    case TOK_OP_MUL:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        if(privateClassType==TYPE_DOUBLE)
        {
            assignVar->mul(privateClassTmpDouble1,privateClassTmpDouble2);
        }
        else
        {
            assignVar->mul(privateClassTmpLong1,privateClassTmpLong2);
        }
        break;
    case TOK_OP_DIV:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        if(privateClassType==TYPE_DOUBLE)
        {
            assignVar->div(privateClassTmpDouble1,privateClassTmpDouble2);
        }
        else
        {
            assignVar->div(privateClassTmpLong1,privateClassTmpLong2);
        }
        break;
    case TOK_OP_SHL:
        extractLongsForOperations(bytecode, privateClassTmpLong1, privateClassTmpLong2);
        assignVar->shl(privateClassTmpLong1,privateClassTmpLong2);
        break;
    case TOK_OP_SHR:
        extractLongsForOperations(bytecode, privateClassTmpLong1, privateClassTmpLong2);
        assignVar->shr(privateClassTmpLong1,privateClassTmpLong2);
        break;
    case TOK_OP_POW:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        if(privateClassType==TYPE_DOUBLE)
        {
            assignVar->pow(privateClassTmpDouble1,privateClassTmpDouble2);
        }
        else
        {
            assignVar->pow(privateClassTmpLong1,privateClassTmpLong2);
        }
        break;
    case TOK_OP_JOIN:
    {
        extractStringsForOperations(bytecode, privateClassTmpString1, privateClassTmpString2);
        if(isTypeVar(bytecode->assignVarType) && isTypeVar(bytecode->arg1Type) && bytecode->assignVarLong==bytecode->arg1Long && bytecode->arg1Type==bytecode->assignVarType)
        {
            assignVar->append(privateClassTmpString2);
        }
        else
        {
            assignVar->join(privateClassTmpString1, privateClassTmpString2);
        }
    }
    break;
    case TOK_COMP_EQINCASE:
        extractValues(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2, privateClassTmpString1, privateClassTmpString2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 == privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 == privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_STRING:
        {
            if(iequals(privateClassTmpString1,privateClassTmpString2))
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
        }
        break;
        default:
            break;
        }

        break;
    case TOK_COMP_EQCASE:
        extractValues(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2, privateClassTmpString1, privateClassTmpString2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 == privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 == privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_STRING:
        {

            if(privateClassTmpString1 == privateClassTmpString2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
        }
        break;
        default:
            break;
        }
        break;
    case TOK_COMP_LESS:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 < privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 < privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        default:
            break;
        }
        break;
    case TOK_COMP_LOWEREQ:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 <= privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 <= privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        default:
            break;
        }
        break;
    case TOK_COMP_GREATEREQ:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 >= privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 >= privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        default:
            break;
        }
        break;
    case TOK_COMP_NOTEQ:
        extractValues(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2, privateClassTmpString1, privateClassTmpString2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 != privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 != privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_STRING:
        {
            if(privateClassTmpString1!=privateClassTmpString2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
        }
        break;
        default:
            break;
        }
        break;
    case TOK_COMP_GREATER:
        extractValuesForOperations(bytecode, privateClassType, privateClassTmpDouble1, privateClassTmpDouble2, privateClassTmpLong1, privateClassTmpLong2);
        switch(privateClassType)
        {
        case TYPE_DOUBLE:
            if(privateClassTmpDouble1 > privateClassTmpDouble2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        case TYPE_LONG:
            if(privateClassTmpLong1 > privateClassTmpLong2)
            {
                assignVar->setTrue();
            }
            else
            {
                assignVar->setFalse();
            }
            break;
        default:
            break;
        }
        break;
    case TOK_BRACKET_ARRAY_CREATION:
        assignVar->clear();
    // Notice missing break, this was done on purpose!
    case TOK_BRACKET_ARRAY_APPEND:
        extractLongsForOperations(bytecode, privateClassTmpLong1);
        assignVar->appendDimension(privateClassTmpLong1);
        break;
    case TOK_BRACKET_ARRAY_ACCESS:
        extractLongsForOperations(bytecode, privateClassTmpLong2, false);
        tmpArg1Var=varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
        if(bytecode->assignVarIsPointer==true)
        {
            assignVar->makePointTo(tmpArg1Var->getVariablePointer(privateClassTmpLong2));
        }
        else
        {
            assignVar->setValue(tmpArg1Var->getVariablePointer(privateClassTmpLong2));
        }
        break;
    default:

        break;
    }
}

/**
* \brief  This method interprets the loaded file Line by Line.
*/
void Interpreter::interpretAllLines()
{
    Window* currWindow;
    BytecodeLine tempLine;
    Message msg;
    ui->startAsyncMessageUpdate();

    currLine=0;
    int cLines=(int)parser.parsedLinesVector.size();
    unsigned int iterations=0;
    while(currLine<cLines)
    {
        if((iterations & 15) == 0 && ui->msgAvailable>0)
        {
            for(currWindow=ui->getNextWindow(); currWindow!=nullptr; currWindow=ui->getNextWindow())
            {
                if(ui->popMessageForWindow(currWindow,msg)==true)
                {
                    if(msg.action!=nullptr)
                    {
                        // call function
                        privateClassTmpString1 = hasher.oneWayFunction(*((std::string*)msg.action));
                        tempLine.type=LINE_COMMAND;
                        tempLine.commandType=TOK_FUNC;
                        tempLine.arg1=privateClassTmpString1;
                        tempLine.arg1Type=TYPE_STRING;
                        callUserFunction(nullptr,&tempLine);
                    }
                }
            }
        }
#if DEBUG > 0
        std::cout<<"["<<currLine<<"] "<<parser.parsedLinesVector[currLine]->type<<" "<<parser.parsedLinesVector[currLine]->assignVar<<std::endl;
#endif // DEBUG
        interpretLine(parser.parsedLinesVector[currLine]);
        currLine++;
        iterations++;
    }
}

/**
* \brief  This method interprets exactly one line of bytecode.
*
* \param bytecode Bytecode-Line to interpret
*
*/
void Interpreter::interpretLine(BytecodeLine* bytecode)
{
    switch(bytecode->type)
    {
    case LINE_ASSIGN:
    {
        AalVariable* assignVar = varManager.getVar(bytecode->assignVarLong, bytecode->assignVarType);
        if(isTypeVar(bytecode->arg1Type))
        {
            AalVariable* arg1 = varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
            assignVar->setValue(arg1);
        }
        else
        {
            if(bytecode->arg1Type==TYPE_LONG)
            {
                assignVar->setValue(bytecode->arg1Long, TYPE_LONG, 0);
            }
            else if(bytecode->arg1Type==TYPE_DOUBLE)
            {
                assignVar->setValue(bytecode->arg1Double, TYPE_DOUBLE, 0);
            }
            else
            {
                assignVar->setValue(bytecode->arg1, TYPE_STRING, 0);
            }
        }
    }
    break;
    case LINE_JMP:
        if(bytecode->arg1Type==TYPE_LONG)
        {
            currLine=bytecode->arg1Long-1;
        }
        else
        {
            auto it=parser.jmpLabels.find(bytecode->arg1);
            if(it!=parser.jmpLabels.end())
            {
                currLine=it->second-1;
            }
        }
        break;
    case LINE_TRUE_JMP:
    {
        bool isJmpTrue=false;
        AalVariable* arg1;
        if(isTypeVar(bytecode->arg1Type))
        {
            AalVariable* arg1 = varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
            isJmpTrue=arg1->isTrue();
        }
        else
        {
            if(bytecode->arg1Type==TYPE_LONG)
            {
                isJmpTrue=(bytecode->arg1Long>0);
            }
            else if(bytecode->arg1Type==TYPE_DOUBLE)
            {
                isJmpTrue=(bytecode->arg1Double>0);
            }
            else if(bytecode->arg1Type==TYPE_STRING)
            {
                isJmpTrue=(bytecode->arg1.size()>0);
            }
        }

        if(isJmpTrue)
        {
            if(bytecode->arg2Type==TYPE_LONG)
            {
                currLine=bytecode->arg2Long-1;
            }
            else
            {
                auto it=parser.jmpLabels.find(bytecode->arg2);
                if(it!=parser.jmpLabels.end())
                {
                    currLine=it->second-1;
                }
            }
        }
    }
    break;
    case LINE_FALSE_JMP:
    {
        bool isJmpFalse=false;
        AalVariable* arg1;
        if(isTypeVar(bytecode->arg1Type))
        {
            AalVariable* arg1 = varManager.getVar(bytecode->arg1Long, bytecode->arg1Type);
            isJmpFalse=arg1->isFalse();
        }
        else
        {
            if(bytecode->arg1Type==TYPE_LONG)
            {
                isJmpFalse=(bytecode->arg1Long<=0);
            }
            else if(bytecode->arg1Type==TYPE_DOUBLE)
            {
                isJmpFalse=(bytecode->arg1Double<=0);
            }
            else if(bytecode->arg1Type==TYPE_STRING)
            {
                isJmpFalse=(bytecode->arg1.size()==0);
            }
        }

        if(isJmpFalse)
        {
            if(bytecode->arg2Type==TYPE_LONG)
            {
                currLine=bytecode->arg2Long-1;
            }
            else
            {
                auto it=parser.jmpLabels.find(bytecode->arg2);
                if(it!=parser.jmpLabels.end())
                {
                    currLine=it->second-1;
                }
            }
        }
    }
    break;
    case LINE_COMMAND:
        interpretCommand(bytecode);
        break;
    case LINE_PUSH:
        varManager.enterNextHierarchyLevel();
        break;
    case LINE_POP:
        varManager.leaveCurrentHierarchyLevel();
        break;
    case LINE_RET:
        varManager.leaveFunction(currLine);
        break;
    case LINE_USING:
        if(bytecode->arg1==STR_E_ONEWAYHASHER)
        {
            hasher.e=std::stoll(bytecode->arg2);
        }
        else if(bytecode->arg1==STR_N_ONEWAYHASHER)
        {
            hasher.n=std::stoll(bytecode->arg2);
        }
        break;
    default:
        break;
    }
}
