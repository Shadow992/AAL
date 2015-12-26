#include "AalVariable.h"
#include "AllocationHelper.h"

AalVariable::AalVariable()
{

}

AalVariable::~AalVariable()
{
    // delete all (*value)[...]
}

/**
* \brief  This method clears the given array by deleting all subdimensions and reseting all internal variables
*         Do not use this method if you do not know whether given parameter is an array or a variable.
*
* \param var array to clear
*
*/
void AalVariable::clearArray(AalVariable* var)
{
    AalVariable* currVar;
	if(var->value!=nullptr)
	{
		for(unsigned int i=0;var->value!=nullptr && i<var->value->size();i++)
		{
            currVar=(AalVariable*) (*(var->value))[i];
			if(currVar->arrayInfo!=nullptr)
			{
				clearArray(currVar);
			}
			else
			{
				clear(currVar);
			}
		}
	}

	if(var->isBaseArray==CHAR_FALSE)
	{
		var->arrayInfo=nullptr;
	}

	var->valueType=nullptr;
	var->value=nullptr;
	var->isBaseArray=CHAR_FALSE;
	var->pointsTo=nullptr;
}

/**
* \brief  This method clears a given array by deleting all subdimensions and reseting all internal variables
*         This method can handle arrays and variables.
*/
void AalVariable::clearArray()
{
	if(arrayInfo==nullptr)
	{
		clear();
		return;
	}

	if(isBaseArray==CHAR_FALSE)
		return;

	clearArray(this);
	for(int i=arrayInfo->usedAalVarChunks.size()-1;i>=0;i--)
	{
		delete[] arrayInfo->usedAalVarChunks[i];
	}

    for(int i=arrayInfo->usedValueChunks.size()-1;i>=0;i--)
	{
		delete[] arrayInfo->usedValueChunks[i];
		delete[] arrayInfo->usedValueTypeChunks[i];
	}
    delete arrayInfo;

	arrayInfo=nullptr;
	isBaseArray=CHAR_FALSE;
	pointsTo=nullptr;
	value=nullptr;
	valueType=nullptr;
}

/**
* \brief  This method clears the given array, variable or pointer given.
*         This method can handle arrays, variables and pointers to clear.
*/
void AalVariable::clear()
{
	if (pointsTo==nullptr)
	{
		if(arrayInfo==nullptr)
		{
			clear(this);
		}
		else
		{
			clearArray();
		}
	}
	else
	{
		arrayInfo=nullptr;
		isBaseArray=CHAR_FALSE;
		pointsTo=nullptr;
		value=nullptr;
		valueType=nullptr;
	}
}

/**
* \brief  This method clears a given variable.
*
* \param var variable to clear
*
*/
void AalVariable::clear(AalVariable* var)
{
	if(var->value==nullptr)
		return;

	for(unsigned int i=0;var->value!=nullptr && i<var->value->size();i++)
	{
	    char type=(*(var->valueType))[i];
		switch(type)
		{
			case TYPE_DOUBLE:
				allocator.recycleDouble((double*)(*(var->value))[i]);
				break;
			case TYPE_LONG:
				allocator.recycleLongLong((long long*)(*(var->value))[i]);
				break;
			case TYPE_STRING:
				allocator.recycleString((std::string*)(*(var->value))[i]);
				break;
			case TYPE_VARIABLE:
				allocator.recycleAalVar((AalVariable*)(*(var->value))[i]);
				break;
            case TYPE_INTERNAL_CLASS:
                allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)(*(var->value))[i]);
                break;
			default:
				break;
		}
	}

	allocator.recycleCharVector(var->valueType);
	allocator.recycleVoidVector(var->value);

	if(var->isBaseArray==CHAR_FALSE)
	{
		var->arrayInfo=nullptr;
	}
	var->valueType=nullptr;
	var->value=nullptr;
	var->isBaseArray=CHAR_FALSE;
	var->pointsTo=nullptr;
}

/**
* \brief  This method clears only the values of a given variable.
*
* \param var variable to clear
*
*/
void AalVariable::clearValues()
{
	if(value==nullptr)
		return;

	for(unsigned int i=0;value!=nullptr && i<value->size();i++)
	{
		switch((*(valueType))[i])
		{
			case TYPE_DOUBLE:
				allocator.recycleDouble((double*)(*(value))[i]);
				break;
			case TYPE_LONG:
				allocator.recycleLongLong((long long*)(*(value))[i]);
				break;
			case TYPE_STRING:
				allocator.recycleString((std::string*)(*(value))[i]);
				break;
			case TYPE_VARIABLE:
				((AalVariable*)(*(value))[i])->clearValues();
				break;
            case TYPE_INTERNAL_CLASS:
                allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)(*(value))[i]);
                break;
			default:
				break;
		}
	}

    if(arrayInfo==nullptr)
    {
        allocator.recycleCharVector(valueType);
        allocator.recycleVoidVector(value);
        value=nullptr;
        valueType=nullptr;
    }
}

/**
* \brief  This method clears the given array, variable or pointer completely without recycling.
*         Only use this function if you are 100% sure what you are doing.
*
*/
void AalVariable::clearCompletely()
{
	if (pointsTo==nullptr)
	{
		if(arrayInfo==nullptr)
		{
			clearCompletely(this);
		}
		else
		{
			clearArray();
		}
	}
}

/**
* \brief  This method clears the given array, variable or pointer completely without recycling.
*         Only use this function if you are 100% sure what you are doing.
*
* \param var variable to clear
*
*/
void AalVariable::clearCompletely(AalVariable* var)
{
	if(var->value==nullptr)
		return;

	for(unsigned int i=0;var->value!=nullptr && i<var->value->size();i++)
	{
		switch((*(var->valueType))[i])
		{
			case TYPE_DOUBLE:
				delete ((double*)(*(var->value))[i]);
			break;
			case TYPE_LONG:
				delete ((long long*)(*(var->value))[i]);
			break;
			case TYPE_STRING:
				delete ((std::string*)(*(var->value))[i]);
			break;
			case TYPE_VARIABLE:
				delete (AalVariable*)(*(var->value))[i];
			break;
            case TYPE_INTERNAL_CLASS:
                delete (std::shared_ptr<InternalClassWrapper>*)(*(var->value))[i];
            break;
			default:
			break;
		}
	}
	delete var->valueType;
	delete var->value;

	if(var->isBaseArray==CHAR_FALSE)
	{
		var->arrayInfo=nullptr;
	}

	var->valueType=nullptr;
	var->value=nullptr;
	var->isBaseArray=CHAR_FALSE;
	var->pointsTo=nullptr;
}

/**
* \brief  This method constructs and inits the variable. If variable was already constructed, it first will be cleared.
*
* \param type type of constructed variable (only TYPE_LONG, TYPE_STRING and TYPE_DOUBLE are allowed)
*
*/
void AalVariable::construct(char type)
{
    clear();
	value=allocator.allocateVoidVector();
	valueType=allocator.allocateCharVector();

	switch(type)
    {
		case TYPE_DOUBLE:
			value->push_back(allocator.allocateDouble());
			valueType->push_back(type);
        break;
		case TYPE_LONG:
			value->push_back(allocator.allocateLongLong());
			valueType->push_back(type);
        break;
        case TYPE_INTERNAL_CLASS:
			value->push_back(allocator.allocateSharedPtr());
			valueType->push_back(type);
        break;
		default:
			value->push_back(allocator.allocateString());
			valueType->push_back(TYPE_STRING);
        break;
    }
}

unsigned int AalVariable::getArgumentCount()
{
    if(value!=nullptr)
    {
        return value->size();
    }
    return 0;
}

unsigned int AalVariable::getValueSize()
{
    if(value!=nullptr)
    {
        return value->size();
    }
    return 0;
}

void* AalVariable::getVoidPointer(const int idx)
{
	if(value==nullptr)
		return nullptr;

    return (*value)[idx];
}

/**
* \brief  This method returns the variable at index idx of an array (only useable for arrays).
*         E.g. call getVariablePointer(3) on Array with size 10 will return aal_array[3]
*
* \param idx index of variable to return
*
* \return aal_array[idx]
*/
AalVariable* AalVariable::getVariablePointer(const int idx)
{
	if(value==nullptr)
		return this;

    return (AalVariable*) (*value)[idx];
}

/**
* \brief  This method sets this to point to given var.
*         Works similar to C/C++: AALVariable* aal_this = &aal_var
* \param var variable to point to
*
*/
void AalVariable::makePointTo(AalVariable* var)
{
    if(var->value==nullptr)
    {
        var->construct(TYPE_LONG);
    }
    value=var->value;
    valueType=var->valueType;
    pointsTo=var;
    arrayInfo=var->arrayInfo;
    isBaseArray=var->isBaseArray;
}

/**
* \brief  This method appends value to variable so it is possible to use this appended value as parameter for function call. Value is copied.
*
* \param val value to append
*
*/
void AalVariable::appendAsParameter(double val)
{
	if(value==nullptr)
	{
		value=allocator.allocateVoidVector();
		valueType=allocator.allocateCharVector();
	}

	double* var=allocator.allocateDouble();
	*var=val;
	value->push_back(var);
	valueType->push_back(TYPE_DOUBLE);
}

/**
* \brief  This method appends value to variable so it is possible to use this appended value as parameter for function call. Value is copied.
*
* \param val value to append
*
*/
void AalVariable::appendAsParameter(long long val)
{
	if(value==nullptr)
	{
		value=allocator.allocateVoidVector();
		valueType=allocator.allocateCharVector();
	}

	long long* var=allocator.allocateLongLong();
	*var=val;
	value->push_back(var);
	valueType->push_back(TYPE_LONG);
}

/**
* \brief  This method appends value to variable so it is possible to use this appended value as parameter for function call. Value is copied.
*
* \param val value to append
*
*/
void AalVariable::appendAsParameter(const std::string& val)
{
	if(value==nullptr)
	{
		value=allocator.allocateVoidVector();
		valueType=allocator.allocateCharVector();
	}

	std::string* var=allocator.allocateString();
	*var=val;
	value->push_back(var);
	valueType->push_back(TYPE_STRING);
}

/**
* \brief  This method appends value to variable so it is possible to use this appended value as parameter for function call. Value is not copied, only points to.
*
* \param val value to append
*
*/
void AalVariable::appendAsParameter(AalVariable* val)
{
	if(value==nullptr)
	{
		value=allocator.allocateVoidVector();
		valueType=allocator.allocateCharVector();
	}

	AalVariable* var=allocator.allocateAalVar();
	var->makePointTo(val);
	value->push_back(var);
	valueType->push_back(TYPE_VARIABLE);
}

/**
* \brief  Method to convert variable to array. Only used for internal functions.
*/
void AalVariable::createArray(unsigned int arraySize, ArrayInformationStruct* arrayStruct,int& lastUsedVectorIdx,int& lastUsedIdx, AalVariable* usedVars,std::vector<char>* usedCharVectors,std::vector<void*>* usedVoidVectors)
{
	//clear();
	usedVoidVectors[lastUsedVectorIdx].resize(arraySize);
	usedCharVectors[lastUsedVectorIdx].resize(arraySize);
	value=&(usedVoidVectors[lastUsedVectorIdx]);
	valueType=&(usedCharVectors[lastUsedVectorIdx]);
	for(unsigned int i=0;i<arraySize;i++)
	{
		(*value)[i]=&(usedVars[lastUsedIdx]);
		(*valueType)[i]=TYPE_VARIABLE;
		lastUsedIdx++;
	}
	arrayInfo=arrayStruct;
	lastUsedVectorIdx++;
}

/**
* \brief  This method appends one dimension to variable or creates array if no dimension exists yet.
*         E.g. call appendDimension(40) on aal_var[10] will create aal_var[10][40]
*
* \param arraySize size of dimension to append
*
*/
void AalVariable::appendDimension(unsigned int arraySize)
{
	if(pointsTo==nullptr)
	{
		ArrayInformationStruct* arrayStruct;
		if(arrayInfo==nullptr)
		{
		    clear();
			arrayStruct=new ArrayInformationStruct;
			arrayStruct->overallArraySize=1;
		}
		else
		{
			arrayStruct=arrayInfo;
		}

		unsigned int overallSizeBefore=arrayStruct->overallArraySize;
		arrayStruct->overallArraySize*=arraySize;
		int lastUsedIdx=0;
		int lastUsedVectorIdx=0;

		AalVariable* usedVars=new AalVariable[arrayStruct->overallArraySize];
		std::vector<char>* usedCharVectors=new std::vector<char>[overallSizeBefore];
		std::vector<void*>* usedVoidVectors=new std::vector<void*>[overallSizeBefore];

		arrayStruct->usedAalVarChunks.push_back(usedVars);
		arrayStruct->usedValueChunks.push_back(usedVoidVectors);
		arrayStruct->usedValueTypeChunks.push_back(usedCharVectors);

		appendDimension(this,arraySize,arrayStruct,lastUsedVectorIdx,lastUsedIdx,usedVars,usedCharVectors,usedVoidVectors);
		arrayInfo=arrayStruct;
		isBaseArray=CHAR_TRUE;
	}
}

/**
* \brief  This method appends one dimension to variable or creates array if no dimension exists yet.
*         Only used for internal methods.
*
* \method appendDimension
*/
void AalVariable::appendDimension(AalVariable* var, unsigned int arraySize, ArrayInformationStruct* arrayStruct,int& lastUsedVectorIdx,int& lastUsedIdx,AalVariable* usedVars,std::vector<char>* usedCharVectors,std::vector<void*>* usedVoidVectors)
{
	if(var->arrayInfo==nullptr)
	{
		var->createArray(arraySize,arrayStruct,lastUsedVectorIdx,lastUsedIdx,usedVars,usedCharVectors,usedVoidVectors);
	}
	else
	{
		for(unsigned int i=0;i<var->value->size();i++)
		{
			if((*(var->valueType))[i]==TYPE_VARIABLE)
			{
				var->appendDimension((AalVariable*) (*(var->value))[i], arraySize,arrayStruct,lastUsedVectorIdx,lastUsedIdx,usedVars,usedCharVectors,usedVoidVectors);
			}
		}
	}
}

/**
* \brief  This method resolves pointers and returns last variable in pointer chain.
*         E.g. calling getAalEndVar(var1) on this pointer chain var1 -> var2 -> var3 -> var4 returns var4.
*
* \param var variable to start resolving from
*
* \return pResolvedVar
*
*/
AalVariable* AalVariable::getAalEndVar(AalVariable* var)
{
	if(var->arrayInfo!=nullptr)
	{
		return var;
	}
	if (var->valueType==nullptr)
		var->construct();
    else if((*(var->valueType))[0] == TYPE_VARIABLE)
    {
        return getAalEndVar((AalVariable*)(*(var->value))[0]);
    }
    return var;
}

/**
* \brief  This method changes the type of 2 given values to the same value. This method is kind a deprecated so better do not use it.
*
* \param var1 pointer to value of first variable
* \param valueType1 type of first value
* \param var2 pointer to value of second variable
* \param valueType2 type of second value
*
*/
void AalVariable::makeCompareable(void*& var1, char& valueType1,void*& var2, char& valueType2)
{
    if(valueType2==TYPE_VARIABLE)
    {
        AalVariable* tmp=(AalVariable*)(var2);
        makeCompareable(var1, valueType1, (*(tmp->value))[0], (*(tmp->valueType))[0]);
        return;
    }

    if(valueType1==TYPE_VARIABLE)
    {
        AalVariable* tmp=(AalVariable*)(var1);
        makeCompareable((*(tmp->value))[0], (*(tmp->valueType))[0], var2, valueType2);
        return;
    }

    // do some casts so both variables have got same type
    switch(valueType1)
    {
    case TYPE_DOUBLE:
        if(valueType2==TYPE_STRING)
        {
            double tmpVar=stod(*((std::string*)(var1)));
            allocator.recycleString((std::string*)var1);
            var1 = allocator.allocateDouble();
            *((double*)var1)=tmpVar;
            valueType2=TYPE_DOUBLE;
        }
        else if(valueType2==TYPE_LONG)
        {
            long long tmp=*((long long*)(var2));
			allocator.recycleLongLong((long long*)var2);
            var2 = allocator.allocateDouble();
            *((double*)var2)=(double)(tmp);
            valueType2=TYPE_DOUBLE;
        }
        break;
    case TYPE_LONG:
        if(valueType2==TYPE_STRING)
        {
            long long tmpVar=stoll(*((std::string*)(var2)));
			allocator.recycleString((std::string*)var2);
            var2 = allocator.allocateLongLong();
            *((long long*)var2)=tmpVar;
            valueType2=TYPE_LONG;
        }
        else if(valueType2==TYPE_DOUBLE)
        {
            double tmp=*((double*)(var1));
			allocator.recycleDouble((double*)var1);
            var1 = allocator.allocateDouble();
            *((double*)var1)=(double)tmp;
            valueType1=TYPE_DOUBLE;
        }
        break;
    case TYPE_STRING:
        if(valueType2==TYPE_LONG)
        {
            long long tmpVar=stoll(*((std::string*)(var1)));
            allocator.recycleString((std::string*)var1);;
            var1 = allocator.allocateLongLong();
            *((long long*)var1)=tmpVar;
            valueType1=TYPE_LONG;
        }
        else if(valueType2==TYPE_DOUBLE)
        {
            double tmpVar=stod(*((std::string*)(var1)));
            allocator.recycleString((std::string*)var1);;
            var1 = allocator.allocateDouble();
            *((double*)var1)=tmpVar;
            valueType1=TYPE_DOUBLE;
        }
        break;
    }
}

/**
* \brief  This method returns the type at given position.
*
* \param idx index of valueType to return
*
* \return type of value atd idx
*
*/
char AalVariable::getType(const int idx)
{
	return (*valueType)[idx];
}

/**
* \brief  This method checks if value of variable at index idx is a double value.
*
* \param idx index of value to check
*
* \return true if is double, false othrwise (e.g. when it is TYPE_LONG)
*
*/
bool AalVariable::isDouble(const int idx)
{
    return (*valueType)[idx]==TYPE_DOUBLE;

    /*switch((*valueType)[idx])
    {
    case TYPE_DOUBLE:
        return true;
    case TYPE_STRING:
    {
        std::string* tmpStr =(std::string*) (*value)[idx];
        if(tmpStr->find(CHR_DOT)!=std::string::npos)
        {
            return true;
        }
    }
    default:
       return false;
    }
    return false;*/
}

/**
* \brief  This method checks if value of variable at index idx is of type TYPE_LONG.
*
* \param idx index of value to check
*
* \return true if is long, false othrwise (e.g. when it is TYPE_DOUBLE)
*
*/
bool AalVariable::isLong(const int idx)
{
	return (*valueType)[idx]==TYPE_LONG;
}

/**
* \brief  This method checks if value of variable at index idx is of type TYPE_STRING.
*
* \param idx index of value to check
*
* \return true if is long, false othrwise (e.g. when it is TYPE_STRING)
*
*/
bool AalVariable::isString(const int idx)
{
	return (*valueType)[idx]==TYPE_STRING;
}

/**
* \brief  This method converts type at position idx to TYPE_DOUBLE and converts the corresponding value.
*
* \param idx index of value to change
*
*/
void AalVariable::convertToDouble(const int idx)
{
    switch((*valueType)[idx])
    {
    case TYPE_LONG:
    {
        void* var1=(*value)[idx];
        long long tmp=*((long long*)(var1));
		allocator.recycleLongLong((long long*)var1);
        var1 = allocator.allocateDouble();
        *((double*)var1)=(double)tmp;
        (*valueType)[idx]=TYPE_DOUBLE;
        (*value)[idx]=var1;
    }
    break;
    case TYPE_STRING:
    {
        void* var1=(*value)[idx];

        double tmpVar=strtod(((std::string*)var1)->c_str(),nullptr);

        allocator.recycleString((std::string*)var1);
        var1 = allocator.allocateDouble();
        *((double*)var1)=tmpVar;
        (*valueType)[idx]=TYPE_DOUBLE;
        (*value)[idx]=var1;
    }
    break;
    case TYPE_INTERNAL_CLASS:
    {
        void* var1=(*value)[idx];
		allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)var1);
        var1 = allocator.allocateDouble();
        (*valueType)[idx]=TYPE_DOUBLE;
        (*value)[idx]=var1;
    }
    break;
    default:
        break;
    }
}

/**
* \brief  This method converts type at position idx to TYPE_LONG and converts the corresponding value.
*
* \param idx index of value to change
*
*/
void AalVariable::convertToLong(const int idx)
{
    switch((*valueType)[idx])
    {
    case TYPE_DOUBLE:
    {
        void* var1=(*value)[idx];
        double tmp=*((double*)(var1));
		allocator.recycleDouble((double*)var1);
        var1 = allocator.allocateLongLong();
        *((long long*)var1)=(long long)tmp;
        (*valueType)[idx]=TYPE_LONG;
        (*value)[idx]=var1;
    }
    break;
    case TYPE_STRING:
    {
        void* var1=(*value)[idx];

        long long tmpVar=strtoll(((std::string*)var1)->c_str(),nullptr,0);

        allocator.recycleString((std::string*)var1);
        var1 = allocator.allocateLongLong();
        *((long long*)var1)=tmpVar;
        (*valueType)[idx]=TYPE_LONG;
        (*value)[idx]=var1;
    }
    break;
    case TYPE_INTERNAL_CLASS:
    {
        void* var1=(*value)[idx];
		allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)var1);
        var1 = allocator.allocateLongLong();
        (*valueType)[idx]=TYPE_LONG;
        (*value)[idx]=var1;
    }
    break;
    default:
        return;
    }
}

/**
* \brief  This method converts type at position idx to TYPE_STRING and converts the corresponding value.
*
* \param idx index of value to change
*
*/
void AalVariable::convertToString(const int idx)
{
    switch((*valueType)[idx])
    {
    case TYPE_DOUBLE:
    {
        void* var1=(*value)[idx];
        double tmp=*((double*)(var1));
        allocator.recycleDouble((double*)var1);
        var1 = allocator.allocateString();
        *((std::string*)var1)=toString(tmp);
        (*valueType)[idx]=TYPE_STRING;
        (*value)[idx]=var1;
    }
    break;
    case TYPE_LONG:
    {
        void* var1=(*value)[idx];
        long long tmp=*((long long*)(var1));
		allocator.recycleLongLong((long long*)var1);
        var1 = allocator.allocateString();
        *((std::string*)var1)=toString(tmp);
        (*valueType)[idx]=TYPE_STRING;
        (*value)[idx]=var1;
    }
    break;
    case TYPE_INTERNAL_CLASS:
    {
        void* var1=(*value)[idx];
		allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)var1);
        var1 = allocator.allocateString();
        (*valueType)[idx]=TYPE_STRING;
        (*value)[idx]=var1;
    }
    break;
    default:
        return;
    }
}

/**
* \brief  This method converts type at position idx to type passed. It will erase the value which was in variable at position idx.
*
* \param idx index of value to change
* \param type new type of value
*
*/
void AalVariable::setType(const int idx,char type)
{
	if(isBaseArray==CHAR_TRUE)
		clearArray();

	if (valueType==nullptr)
		construct(type);

    if(type==(*valueType)[idx])
    {
        if(type==TYPE_VARIABLE)
        {
            clear(((AalVariable*)(*value)[idx]));
        }
        return;
    }


    switch((*valueType)[idx])
    {
    case TYPE_DOUBLE:
		allocator.recycleDouble((double*)(*value)[idx]);
    break;
    case TYPE_LONG:
		allocator.recycleLongLong((long long*)(*value)[idx]);
    break;
    case TYPE_STRING:
		allocator.recycleString((std::string*)(*value)[idx]);
    break;
	case TYPE_VARIABLE:
		allocator.recycleAalVar((AalVariable*)(*value)[idx]);
		break;
    case TYPE_INTERNAL_CLASS:
        allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)(*(value))[idx]);
        break;
    default:
        break;
    }

    switch(type)
    {
    case TYPE_DOUBLE:
    {
        (*value)[idx] = allocator.allocateDouble();
    }
    break;
    case TYPE_LONG:
    {
        (*value)[idx] = allocator.allocateLongLong();
    }
    break;
    case TYPE_STRING:
    {
        (*value)[idx] = allocator.allocateString();
    }
    break;
    default:
        break;
    }
    (*valueType)[idx]=type;
}

/**
* \brief  This method will set value at position idx to new given value.
*
* \param val new value to set
* \param type new type of value
* \param idx index of value to set
*
*/
void AalVariable::setValue(const std::string& val, char type,int idx)
{
	if(isBaseArray==CHAR_TRUE)
		clearArray();

	if (valueType==nullptr)
		construct(type);

	AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

    void* var1=(*(realVar->value))[idx];
    bool isString=false;

    switch((*(realVar->valueType))[idx])
    {
    case TYPE_DOUBLE:
    {
		allocator.recycleDouble((double*)var1);
    }
    break;
    case TYPE_LONG:
    {
        allocator.recycleLongLong((long long*)var1);
    }
    break;
    case TYPE_STRING:
        isString=true;
        break;
    case TYPE_INTERNAL_CLASS:
        allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)var1);
        break;
    }

    switch(type)
    {
    case TYPE_DOUBLE:
    {
        var1 = allocator.allocateDouble();
        *((double*)var1)=strtod(val.c_str(),nullptr);
        (*(realVar->valueType))[idx]=TYPE_DOUBLE;
    }
    break;
    case TYPE_LONG:
    {
        var1 = allocator.allocateLongLong();
        *((long long*)var1)=strtoll(val.c_str(),nullptr,0);
        (*(realVar->valueType))[idx]=TYPE_LONG;
    }
    break;
    case TYPE_STRING:
    {
        if(!isString)
        {
            var1 = allocator.allocateString();
			(*(realVar->valueType))[idx]=TYPE_STRING;
        }
        *((std::string*)var1)=val;
    }
    break;
    default:
        break;
    }
    (*(realVar->value))[idx]=var1;
}

/**
* \brief  This method will set value at position idx to new given value.
*
* \param val new value to set
* \param type new type of value
* \param idx index of value to set
*
*/
void AalVariable::setValue(const long long val, char type,int idx)
{
	if(isBaseArray==CHAR_TRUE)
		clearArray();

	if (valueType==nullptr)
		construct(type);

	AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

    void* var1=(*(realVar->value))[idx];
    bool isLong=false;

    switch((*(realVar->valueType))[idx])
    {
    case TYPE_DOUBLE:
    {
		allocator.recycleDouble((double*)var1);
    }
    break;
    case TYPE_LONG:
        isLong=true;
    break;
    case TYPE_STRING:
        allocator.recycleString((std::string*)var1);
        break;
    case TYPE_INTERNAL_CLASS:
        allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)var1);
        break;
    }

    switch(type)
    {
    case TYPE_DOUBLE:
    {
        var1 = allocator.allocateDouble();
        *((double*)var1)=(double)val;
        (*(realVar->valueType))[idx]=TYPE_DOUBLE;
    }
    break;
    case TYPE_LONG:
    {
        if(!isLong)
        {
            var1 = allocator.allocateLongLong();
			(*(realVar->valueType))[idx]=TYPE_LONG;
        }
        *((long long*)var1)=val;
    }
    break;
    case TYPE_STRING:
    {
        var1 = allocator.allocateString();
        (*(realVar->valueType))[idx]=TYPE_STRING;
        *((std::string*)var1)=toString(val);
    }
    break;
    default:
        break;
    }
    (*(realVar->value))[idx]=var1;
}

/**
* \brief  This method will set value at position idx to new given value.
*
* \param val new value to set
* \param type new type of value
* \param idx index of value to set
*
*/
void AalVariable::setValue(const double val, char type,int idx)
{
	if(isBaseArray==CHAR_TRUE)
		clearArray();

	if (valueType==nullptr)
		construct(type);

	AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

    void* var1=(*(realVar->value))[idx];
    bool isDouble=false;

    switch((*(realVar->valueType))[idx])
    {
    case TYPE_DOUBLE:
    {
		isDouble=true;
    }
    break;
    case TYPE_LONG:
    {
        allocator.recycleLongLong((long long*)var1);
    }
    break;
    case TYPE_STRING:
        allocator.recycleString((std::string*)var1);
        break;
    case TYPE_INTERNAL_CLASS:
        allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)var1);
        break;
    }

    switch(type)
    {
    case TYPE_DOUBLE:
    {
        if(!isDouble)
        {
            var1 = allocator.allocateDouble();
            (*(realVar->valueType))[idx]=TYPE_DOUBLE;
        }
        *((double*)var1)=val;
    }
    break;
    case TYPE_LONG:
    {
        var1 = allocator.allocateLongLong();
        *((long long*)var1)=(long long)val;
        (*(realVar->valueType))[idx]=TYPE_LONG;
    }
    break;
    case TYPE_STRING:
    {
        var1 = allocator.allocateString();
        (*(realVar->valueType))[idx]=TYPE_STRING;
        *((std::string*)var1)=toString(val);
    }
    break;
    default:
        break;
    }
    (*(realVar->value))[idx]=var1;
}

/**
* \brief  This method copies an array from src to dest (normally only internal used).
*
* \param src array to copy into dest
* \param dest array to copy to
* \param arrayInfoStruct information structure required for creating arrays
*
*/
void AalVariable::copyArray(AalVariable* src, AalVariable* dest,ArrayInformationStruct* arrayInfoStruct)
{
    if(src->value!=nullptr)
    {
        if(src->arrayInfo!=nullptr)
        {
            unsigned int arraySize=src->value->size();

            dest->value->resize(arraySize);
            dest->valueType->resize(arraySize);

            AalVariable* usedVars=new AalVariable[arraySize];
            std::vector<char>* usedCharVectors=nullptr;
            std::vector<void*>* usedVoidVectors=nullptr;
            char* setVectors=new char[arraySize];

            dest->arrayInfo=arrayInfoStruct;
            arrayInfoStruct->usedAalVarChunks.push_back(usedVars);

            unsigned int finalArraySize=0;
            for(unsigned int i=0;i<arraySize;i++)
            {
                setVectors[i]=0;
                if((*(src->valueType))[i]==TYPE_VARIABLE && ((AalVariable*)(*(src->value))[i])->arrayInfo!=nullptr )
                {
                    finalArraySize++;
                    setVectors[i]=1;
                }
            }

            if(finalArraySize>0)
            {
                usedCharVectors=new std::vector<char>[finalArraySize];
                usedVoidVectors=new std::vector<void*>[finalArraySize];
                arrayInfoStruct->usedValueChunks.push_back(usedVoidVectors);
                arrayInfoStruct->usedValueTypeChunks.push_back(usedCharVectors);
            }
            unsigned int a=0;

            for(unsigned int i=0;i<arraySize;i++)
            {
                if(setVectors[i]==1)
                {
                    usedVars[i].value=&(usedVoidVectors[a]);
                    usedVars[i].valueType=&(usedCharVectors[a]);
                    a++;
                }
                (*(dest->value))[i]=&(usedVars[i]);
                (*(dest->valueType))[i]=TYPE_VARIABLE;

                copyArray((AalVariable*) (*(src->value))[i],&(usedVars[i]),arrayInfoStruct);
            }

            delete[] setVectors;
            return;
        }
        else
        {
            dest->setValue(src);
        }
    }
    arrayInfoStruct->overallArraySize++;
}

/**
* \brief  This method copies an array from src to dest (=this).
*
* \param src array to copy into this
* \param idx index of src array to get value from (-1 for complete array, 0 for array[0], ...)
* \param idx2 index of this array to copy value to (-1 for complete array, 0 for array[0], ...)
*
*/
void AalVariable::setArrayValue(AalVariable* src, int idx,int idx2)
{
    clearValues();

    ArrayInformationStruct* arrayInfoStruct;

    if(arrayInfo==nullptr)
    {
        //clearValues();
        // our assign variable values got cleared
		// X Dimension array
        arrayInfoStruct=new ArrayInformationStruct;
        if(pointsTo==nullptr)
        {
            arrayInfo=arrayInfoStruct;
            isBaseArray=CHAR_TRUE;
        }
        else
        {
            pointsTo->isBaseArray=CHAR_TRUE;
            pointsTo->arrayInfo=arrayInfoStruct;
        }
		arrayInfoStruct->overallArraySize=0;
    }
    else
    {

        // TODO: Add this later? Needed? Working?
        //--------------------------------------
        /*if(idx2>-1)
        {
            dest->setType(idx2,TYPE_VARIABLE);
            dest=(AalVariable*)(*(dest->value))[idx2];
        }

        if(idx>-1)
        {
            if((*(src->valueType))[idx]==TYPE_VARIABLE)
            {
                src=(AalVariable*)(*(src->value))[idx];
            }
        }*/
        arrayInfoStruct=arrayInfo;
    }

    if(value==nullptr)
    {
        valueType=new std::vector<char>[1];
        value=new std::vector<void*>[1];

        arrayInfoStruct->usedValueChunks.push_back(value);
        arrayInfoStruct->usedValueTypeChunks.push_back(valueType);

        if(pointsTo==nullptr)
        {
            value=&(value[0]);
            valueType=&(valueType[0]);
        }
        else
        {
            pointsTo->value=&(value[0]);
            pointsTo->valueType=&(valueType[0]);
        }
    }

    copyArray(src,this,arrayInfoStruct);
}



/**
* \brief  This method will set value at position idx to new given value (can also handle array).
*
* \param src variable to copy value
* \param idxSrc index of source value
* \param idxDest index of destination value to set
*
*/
void AalVariable::setValue(AalVariable* src, int idxDest, int idxSrc)
{
    if(isBaseArray==CHAR_TRUE)
    {
        clearArray();
    }

    if(src->arrayInfo!=nullptr)
    {
        setArrayValue(src,idxSrc,idxDest);
        return;
    }

	if(src->valueType==nullptr)
		src->construct();

	if (valueType==nullptr)
		construct((*(src->valueType))[0]);

	AalVariable* realDest;
	if((*valueType)[idxSrc]==TYPE_VARIABLE)
	{
		realDest=getAalEndVar((AalVariable*)(*value)[idxDest]);
		idxSrc=0;
	}
	else
	{
		realDest=this;
	}

	char type=(*(src->valueType))[idxSrc];

    if(type==TYPE_VARIABLE)
	{
		src=getAalEndVar((AalVariable*)(*(src->value))[idxSrc]);
		idxSrc=0;
		type=(*(src->valueType))[idxSrc];
	}

    void* destValue=(*(realDest->value))[idxDest];

    switch((*(realDest->valueType))[idxDest])
    {
		case TYPE_DOUBLE:
		{
			allocator.recycleDouble((double*)destValue);
		}
		break;
		case TYPE_LONG:
		{
			allocator.recycleLongLong((long long*)destValue);
		}
		break;
		case TYPE_STRING:
		{
			allocator.recycleString((std::string*)destValue);
		}
		break;
        case TYPE_INTERNAL_CLASS:
            allocator.recycleSharedPtr((std::shared_ptr<InternalClassWrapper>*)destValue);
            break;
        break;
    }

    switch(type)
    {
		case TYPE_DOUBLE:
		{
			destValue = allocator.allocateDouble();
			*((double*)destValue)=*((double*) (*(src->value))[idxSrc]);
			(*(realDest->valueType))[idxDest]=TYPE_DOUBLE;
		}
		break;
		case TYPE_LONG:
		{
			destValue = allocator.allocateLongLong();
			*((long long*)destValue)=*((long long*) (*(src->value))[idxSrc]);
			(*(realDest->valueType))[idxDest]=TYPE_LONG;
		}
		break;
		case TYPE_STRING:
		{
			destValue = allocator.allocateString();
			*((std::string*)destValue)=*((std::string*) (*(src->value))[idxSrc]);
			(*(realDest->valueType))[idxDest]=TYPE_STRING;
		}
		break;
        case TYPE_INTERNAL_CLASS:
            *((std::shared_ptr<InternalClassWrapper>*) destValue)=*((std::shared_ptr<InternalClassWrapper>*) (*(src->value))[idxSrc]);
			(*(realDest->valueType))[idxDest]=TYPE_INTERNAL_CLASS;
            break;
        break;
		default:
			break;
    }
    (*(realDest->value))[idxDest]=destValue;
}

/**
* \brief  This method sets variable to true.
*/
void AalVariable::setTrue()
{
	if(valueType==nullptr)
		construct(TYPE_LONG);

    switch((*valueType)[0])
    {
    case TYPE_DOUBLE:
        setValue(1.0,TYPE_DOUBLE,0);
        break;
    case TYPE_STRING:
        setValue(STR_TRUE,TYPE_STRING,0);
        break;
    default:
        setValue(1LL,TYPE_LONG,0);
        break;
    }
}

/**
* \brief  This method sets variable to false.
*/
void AalVariable::setFalse()
{
	if(valueType==nullptr)
		construct(TYPE_LONG);

    switch((*valueType)[0])
    {
    case TYPE_DOUBLE:
        setValue(0.0,TYPE_DOUBLE,0);
        break;
    case TYPE_STRING:
        setValue(STR_FALSE,TYPE_STRING,0);
        break;
    default:
        setValue(0LL,TYPE_LONG,0);
        break;
    }
}

/**
* \brief  This method checks if value at position idx is false.
*
* \param idx index to check
*
* \return true if value is false, otherwise false
*
*/
bool AalVariable::isFalse(int idx)
{
	AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

	switch((*(realVar->valueType))[idx])
    {
		case TYPE_DOUBLE:
		{
			return *((double*)(*(realVar->value))[idx]) == 0.0;
		}
		break;
		case TYPE_LONG:
		{
			return *((long long*)(*(realVar->value))[idx]) == 0LL;
		}
		break;
		case TYPE_STRING:
		{
			return *((std::string*)(*(realVar->value))[idx]) == STR_FALSE;
		}
		break;
		case TYPE_INTERNAL_CLASS:
		    return *((std::shared_ptr<InternalClassWrapper>*)(*(realVar->value))[idx])==nullptr;
        break;
    }
    return false;
}

/**
* \brief  This method checks if value at position idx is true.
*
* \param idx index to check
*
* \return true if value is true, otherwise false
*
*/
bool AalVariable::isTrue(int idx)
{
	AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

	switch((*(realVar->valueType))[idx])
    {
		case TYPE_DOUBLE:
			return *((double*)(*(realVar->value))[idx]) != 0.0;
		case TYPE_LONG:
			return *((long long*)(*(realVar->value))[idx]) != 0LL;
		case TYPE_STRING:
			return *((std::string*)(*(realVar->value))[idx]) != STR_FALSE;
        case TYPE_INTERNAL_CLASS:
		    return *((std::shared_ptr<InternalClassWrapper>*)(*(realVar->value))[idx])!=nullptr;
        break;
    }
    return false;
}

/**
* \brief  This method returns pointer to value in variable at position idx. If value is not of TYPE_LONG it will be converted to this type.
*
* \param idx index of value
*
* \return pointer to value at index idx (Care! You will get the pointer so any changes will also change the value in variable)
*
*/
long long* AalVariable::getLongPointer(const int idx)
{
	if (valueType==nullptr)
		construct(TYPE_LONG);
    else if(valueType->at(idx)!=TYPE_LONG)
       convertToLong(idx);

    return (long long*)(*value)[idx];
}

/**
* \brief  This method returns pointer to value in variable at position idx. If value is not of TYPE_DOUBLE it will be converted to this type.
*
* \param idx index of value
*
* \return pointer to value at index idx (Care! You will get the pointer so any changes will also change the value in variable)
*
*/
double* AalVariable::getDoublePointer(const int idx)
{
	if (valueType==nullptr)
		construct(TYPE_DOUBLE);
    else if(valueType->at(idx)!=TYPE_DOUBLE)
        convertToDouble(idx);

    return (double*)(*value)[idx];
}

std::shared_ptr<InternalClassWrapper>* AalVariable::getSharedPointer(int idxDest)
{
    if (valueType==nullptr)
		construct(TYPE_INTERNAL_CLASS);

    void* destValue=(*(value))[idxDest];
    switch((*(valueType))[idxDest])
    {
		case TYPE_DOUBLE:
		{
			allocator.recycleDouble((double*)destValue);
		}
		break;
		case TYPE_LONG:
		{
			allocator.recycleLongLong((long long*)destValue);
		}
		break;
		case TYPE_STRING:
		{
			allocator.recycleString((std::string*)destValue);
		}
		break;
        case TYPE_INTERNAL_CLASS:
            return (std::shared_ptr<InternalClassWrapper>*)destValue;
            break;
        break;
    }

    (*(value))[idxDest]=allocator.allocateSharedPtr();
    (*(valueType))[idxDest]=TYPE_INTERNAL_CLASS;
    return (std::shared_ptr<InternalClassWrapper>*)(*(value))[idxDest];
}

/**
* \brief  This method returns pointer to value in variable at position idx. If value is not of TYPE_STRING it will be converted to this type.
*
* \param idx index of value
*
* \return pointer to value at index idx (Care! You will get the pointer so any changes will also change the value in variable)
*/
std::string* AalVariable::getStringPointer(const int idx)
{
	if (valueType==nullptr)
		construct(TYPE_STRING);
    else if(valueType->at(idx)!=TYPE_STRING)
        convertToString(idx);
    return (std::string*)(*value)[idx];
}

/**
* \brief  This method returns value in variable at position idx. If value is not of TYPE_LONG it will be converted to this type.
*
* \param idx index of value
*
* \return value at index idx (only value)
*/
long long AalVariable::getLongValue(int idx)
{
	if (valueType==nullptr)
		construct(TYPE_LONG);

    AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

    char type=realVar->valueType->at(idx);
	switch(type)
    {
		case TYPE_DOUBLE:
			return *((double*)(*(realVar->value))[idx]);
		case TYPE_LONG:
			return *((long long*)(*(realVar->value))[idx]);
		case TYPE_STRING:
		{
		    std::string *tmpStr=((std::string*)(realVar->value)->at(idx));
            return strtoll(tmpStr->c_str(),nullptr,0);
		}
    }
    return 0LL;
}

/**
* \brief  This method returns value in variable at position idx. If value is not of TYPE_DOUBLE it will be converted to this type.
*
* \param idx index of value
*
* \return value at index idx (only value)
*/
double AalVariable::getDoubleValue(int idx)
{
	if (valueType==nullptr)
		construct(TYPE_LONG);

    AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

	switch((*(realVar->valueType))[idx])
    {
		case TYPE_DOUBLE:
		{
			return *((double*)(*(realVar->value))[idx]);
		}
		break;
		case TYPE_LONG:
		{
			return *((long long*)(*(realVar->value))[idx]);
		}
		break;
		case TYPE_STRING:
		{
            std::string *tmpStr=((std::string*)(realVar->value)->at(idx));
            return strtod(tmpStr->c_str(),nullptr);
		}
        break;
    }
    return 0;
}

/**
* \brief  This method returns value in variable at position idx. If value is not of TYPE_STRING it will be converted to this type.
*
* \param idx index of source value
*
* \return value at index idx (only value)
*/
std::string AalVariable::getStringValue(int idx)
{
	if (valueType==nullptr)
		construct(TYPE_LONG);

    AalVariable* realVar;
	if((*valueType)[idx]==TYPE_VARIABLE)
	{
		realVar=getAalEndVar((AalVariable*)(*value)[idx]);
		idx=0;
	}
	else
	{
		realVar=this;
	}

	switch((*(realVar->valueType))[idx])
    {
		case TYPE_DOUBLE:
		{
			return toString(*((double*)(*(realVar->value))[idx]));
		}
		break;
		case TYPE_LONG:
		{
			return toString(*((long long*)(*(realVar->value))[idx]));
		}
		break;
		case TYPE_STRING:
		{
			return *((std::string*)(*(realVar->value))[idx]);
		}
        break;
    }
    return STR_EMPTY;
}

/**
* \brief  This method appends string to variable.
*/
void AalVariable::append(const std::string& first)
{
    *(getStringPointer(0))+=first;
}

/**
* \brief  This method joins two strings and sets the value of variable to this joined string.
*/
void AalVariable::join(const std::string& first, const std::string& other)
{
    *(getStringPointer(0)) = first + other;
}

/**
* \brief  This method adds two values and sets the result.
*/
void AalVariable::add(const long long first, const long long other)
{
    *(getLongPointer(0)) = first + other;
}

/**
* \brief  This method subs two values and sets the result.
*/
void AalVariable::sub(const long long first, const long long other)
{
    *(getLongPointer(0)) = first - other;
}

/**
* \brief  This method multiplies two values and sets the result.
*/
void AalVariable::mul(const long long first, const long long other)
{
    *(getLongPointer(0)) = first * other;
}

/**
* \brief  This method divides two values and sets the result (result will be converted to TYPE_DOUBLE if needed).
*/
void AalVariable::div(const long long first, const long long other)
{
	if(other==0)
		return;

    if(first%other != 0)
    {
        div((double)first, (double)other);
    }
    else
    {
        *(getLongPointer(0)) = first / other ;
    }
}

/**
* \brief  This method adds two values and sets the result.
*/
void AalVariable::add(const double first, const double other)
{
    *(getDoublePointer(0)) = first + other;
}

/**
* \brief  This method subs two values and sets the result.
*/
void AalVariable::sub(const double first, const double other)
{
    *(getDoublePointer(0)) = first - other;
}

/**
* \brief  This method multiplies two values and sets the result.
*/
void AalVariable::mul(const double first, const double other)
{
    *(getDoublePointer(0)) = first * other;
}

/**
* \brief  This method divides two values and sets the result.
*/
void AalVariable::div(const double first, const double other)
{
    *(getDoublePointer(0)) = first / other ;
}

/**
* \brief  This method powers two values and sets the result.
*/
void AalVariable::pow(const double first, const double other)
{
    *(getLongPointer(0)) = std::pow(first , other);
}

/**
* \brief  This method shifts left two values and sets the result.
*/
void AalVariable::shl(const long long first, const long long other)
{
    *(getDoublePointer(0)) = first << other;
}

/**
* \brief  This method shifts right two values and sets the result.
*/
void AalVariable::shr(const long long first, const long long other)
{
    *(getLongPointer(0)) = first >> other;
}

/**
* \brief  This method powers two values and sets the result.
*/
void AalVariable::pow(const long long first, const long long other)
{
    *(getLongPointer(0)) = std::pow(first , other);
}

/**
* \brief  This method modulo two values and sets the result.
*/
void AalVariable::mod(const long long first, const long long other)
{
    *(getLongPointer(0)) = first % other;
}

/**
* \brief  This method prints a given var recursively (used for arrays, mostly used for debugging/internal purpose, recursive helper function for printVarRecursive(), better only call printVarRecursive()).
*/
void AalVariable::printVarRecursive(AalVariable* var, int depth)
{
	if(var->value==nullptr)
		var->construct();

	for(unsigned int i=0;i<var->value->size();i++)
	{
		for(int j=0;j<depth;j++)
		{
			std::cout<<"-";
		}

		if((*(var->valueType))[i]==TYPE_VARIABLE)
		{
			std::cout<<"["<<i<<"]"<<" "<< (*(var->valueType))[i]<< std::endl;
			var->printVarRecursive((AalVariable*) (*(var->value))[i],depth+1);
		}
		else
		{
			std::cout<<" "<< (*(var->valueType))[i]<< " " << var->getStringValue(i) << std::endl;
		}
	}
}

/**
* \brief  This method prints a given var recursively (used for arrays, mostly used for debugging/internal purpose).
*/
void AalVariable::printVarRecursive()
{
	printVarRecursive(this, 1);
}
