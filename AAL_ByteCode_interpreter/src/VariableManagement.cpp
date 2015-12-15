#include "VariableManagement.h"

VariableManagement::VariableManagement()
{
	globalVars.resize(1000,nullptr);
	generatedPtrVars.resize(1000,nullptr);
	generatedTmpVars.resize(1000,nullptr);
}

VariableManagement::~VariableManagement()
{
    for(unsigned int i=0;i<globalVars.size();i++)
    {
        if(globalVars[i]!=nullptr)
        {
            allocator.recycleAalVar(globalVars[i]);
        }
    }

    for(unsigned int i=0;i<generatedTmpVars.size();i++)
    {
        if(generatedTmpVars[i]!=nullptr)
        {
            allocator.recycleAalVar(generatedTmpVars[i]);
        }
    }

    for(unsigned int i=0;i<generatedPtrVars.size();i++)
    {
        if(generatedPtrVars[i]!=nullptr)
        {
            allocator.recycleAalVar(generatedPtrVars[i]);
        }
    }

    for(unsigned int i=0;i<hierachyVars.size();i++)
    {
        if(hierachyVars[i]!=nullptr)
        {
            allocator.recycleVectorMap(hierachyVars[i]);
        }
    }
}

/**
* \brief  This method prints all current saved variables (mainly used for debugging)
*/
void VariableManagement::printVars()
{
    std::cout<<"Globals:\n";
    std::cout<<"Not Temp:\n";
    for(unsigned int i=0;i<globalVars.size();i++)
    {
        if(globalVars[i]!=nullptr)
        {
            std::cout<<"["<<i<<"] "<<globalVars[i]->getStringValue()<<std::endl;
        }
    }
    std::cout<<"Temp:\n";
    for(unsigned int i=0;i<generatedTmpVars.size();i++)
    {
        if(generatedTmpVars[i]!=nullptr)
        {
            std::cout<<"["<<i<<"] "<<generatedTmpVars[i]->getStringValue()<<std::endl;
        }
    }

    std::cout<<"Ptr:\n";
    for(unsigned int i=0;i<generatedPtrVars.size();i++)
    {
        if(generatedPtrVars[i]!=nullptr)
        {
            std::cout<<"["<<i<<"] "<<generatedPtrVars[i]->getStringValue()<<std::endl;
        }
    }


    std::cout<<"Hierarchy:\n";
    Hashtable<AalVariable>* tmpMap;
    for(int i=0;i<=currHierarchyLevel;i++)
    {
        tmpMap=hierachyVars[i]->back();
        if(tmpMap!=nullptr)
        {
            unsigned int capacity=tmpMap->capacity();
            AalVariable* tmpVar;
            for(unsigned int i2=0; i2 < capacity ;i2++)
            {
                tmpVar=tmpMap->getByIndex(i2);
                if(tmpVar!=nullptr)
                {
                    std::cout<<"["<<tmpMap->getNumByIndex(i2)<<","<<i<<"] "<<tmpVar->getStringValue()<<std::endl;
                }
            }
        }
    }
    std::cout<<"\n";
}

/**
* \brief  This method returns a pointer to the requested variable (or creates it if not existing yet). It will return the correct value for current hierarchy level.
*
* \param str Name of variable to get value of (without prefix, e.g. "$20" and not "*$20")
* \param type Type of variable, can be TYPE_PTR_VARIABLE, TYPE_TMP_VARIABLE or TYPE_VARIABLE)
* \param createNewVar If this parameter is set to true it will always create a new variable and overwrite old variable
*
* \return Returns pointer to saved variable
*
*/
AalVariable* VariableManagement::getVar(const std::string& str, const char type, bool createNewVar)
{
    long long tmp=stoll(str.substr(2));
    return getVar(tmp,type,createNewVar);
}

/**
* \brief  This method returns a pointer to the requested variable (or creates it if not existing yet). It will return the correct variable for current hierarchy level.
*
* \param tmp Name of variable as unsigned long long and without leading number. So "$21" would be "1", "$123" would be "23".
* \param type Type of variable, can be TYPE_PTR_VARIABLE, TYPE_TMP_VARIABLE or TYPE_VARIABLE)
* \param createNewVar If this parameter is set to true it will always create a new variable and overwrite old variable
*
* \return Returns pointer to saved variable
*
*/
AalVariable* VariableManagement::getVar(unsigned long long tmp, const char type, bool createNewVar)
{
	//std::cout<<"var "<<tmp<<" "<<type<<"\n";
    if(type==TYPE_VARIABLE)
    {
        AalVariable* globalRetVar=nullptr;
        if(globalVars.size()>tmp && globalVars[tmp]!=nullptr)
        {
            //std::cout<<"found: "<<globalVars[tmp]->getStringValue(0)<<std::endl;
            globalRetVar=globalVars[tmp];
            if(currFunctionDepth==0 && createNewVar==false)
            {
                return globalRetVar;
            }
        }

		if(currHierarchyLevel==-1)
		{
		    if(globalRetVar!=nullptr)
            {
                globalRetVar->clear();
            }

			if(globalVars.size()<=tmp)
			{
				globalVars.resize(tmp<<1,nullptr);
			}

			AalVariable* tmpVar=allocator.allocateAalVar();
			globalVars[tmp]=tmpVar;
			return tmpVar;
		}

        //std::cout<<"found "<<tmp<<"\n";
        Hashtable<AalVariable>* tmpMap;
        if(createNewVar==false)
        {
            // check if variable was declared in some hierarchy levels before
            for(int i=currHierarchyLevel;i>=0;i--)
            {
                tmpMap=hierachyVars[i]->back();
                if(tmpMap!=nullptr)
                {
                    AalVariable* var=tmpMap->get(tmp);
                    if(var!=nullptr)
                    {
                        return var;
                    }
                }
            }
        }

        if(createNewVar==false && globalVars.size()>tmp && globalVars[tmp]!=nullptr)
        {
            //std::cout<<"found: "<<globalVars[tmp]->getStringValue(0)<<std::endl;
            return globalVars[tmp];
        }

        // so we did not find our variable at any hierarchy level
        // just create it now
        AalVariable* tmpVar=allocator.allocateAalVar();
        tmpMap=hierachyVars[currHierarchyLevel]->back();
        //std::cout<<"currHierarchyLevel: "<<currHierarchyLevel<<std::endl;
        if(tmpMap==nullptr)
        {
            tmpMap=allocator.allocateMap();
            hierachyVars[currHierarchyLevel]->back()=tmpMap;
        }
        AalVariable* retVar=tmpMap->set(tmp,tmpVar);
        if(retVar!=nullptr)
        {
            retVar->clear();
        }
        return tmpVar;

    }
	else if(type==TYPE_TMP_VARIABLE)
	{
        if(currHierarchyLevel==-1)
        {
            AalVariable* globalRetVar=nullptr;
            if(generatedTmpVars.size()>tmp && generatedTmpVars[tmp]!=nullptr)
            {
                globalRetVar=generatedTmpVars[tmp];
                if(createNewVar==false)
                {
                    return globalRetVar;
                }
            }

            if(globalRetVar!=nullptr)
            {
                globalRetVar->clear();
            }

            if(generatedTmpVars.size()<=tmp)
            {
                generatedTmpVars.resize(tmp<<1,nullptr);
            }
            AalVariable* tmpVar=allocator.allocateAalVar();
            generatedTmpVars[tmp]=tmpVar;
            return tmpVar;

        }
        else
		{
            tmp|=1ULL<<63;
            Hashtable<AalVariable>* tmpMap;
            if(createNewVar==false)
            {
                // check if variable was declared in some hierarchy levels before
                tmpMap=hierachyVars[currHierarchyLevel]->back();
                if(tmpMap!=nullptr)
                {
                    AalVariable* var=tmpMap->get(tmp);
                    if(var!=nullptr)
                    {
                        return var;
                    }
                }
            }

			// so we did not find our variable at any hierarchy level
			// just create it now
			AalVariable* tmpVar=allocator.allocateAalVar();
			tmpMap=hierachyVars[currHierarchyLevel]->back();
			if(tmpMap==nullptr)
			{
				tmpMap=allocator.allocateMap();
				hierachyVars[currHierarchyLevel]->back()=tmpMap;
			}
			AalVariable* retVar=tmpMap->set(tmp,tmpVar);
            if(retVar!=nullptr)
            {
                retVar->clear();
            }
			return tmpVar;
		}
	}
	else if(type==TYPE_PTR_VARIABLE)
	{
        if(currHierarchyLevel==-1)
        {
            AalVariable* globalRetVar=nullptr;

            if(generatedPtrVars.size()>tmp && generatedPtrVars[tmp]!=nullptr)
            {
                globalRetVar=generatedPtrVars[tmp];
                if(createNewVar==false)
                {
                    return globalRetVar;
                }

            }

            if(globalRetVar!=nullptr)
            {
                globalRetVar->clear();
            }

            if(generatedPtrVars.size()<=tmp)
            {
                generatedPtrVars.resize(tmp+2,nullptr);
            }

            AalVariable* tmpVar=allocator.allocateAalVar();
            generatedPtrVars[tmp]=tmpVar;
            return tmpVar;

        }
        else
		{
            tmp|=1ULL<<62;
            Hashtable<AalVariable>* tmpMap;
            if(createNewVar==false)
            {
                // check if variable was declared in some hierarchy levels before
                tmpMap=hierachyVars[currHierarchyLevel]->back();
                if(tmpMap!=nullptr)
                {
                    AalVariable* var=tmpMap->get(tmp);
                    if(var!=nullptr)
                    {
                        return var;
                    }
                }
            }

			// so we did not find our variable at any hierarchy level
			// just create it now
			AalVariable* tmpVar=allocator.allocateAalVar();
			tmpMap=hierachyVars[currHierarchyLevel]->back();
			if(tmpMap==nullptr)
			{
				tmpMap=allocator.allocateMap();
				hierachyVars[currHierarchyLevel]->back()=tmpMap;
			}
			AalVariable* retVar=tmpMap->set(tmp,tmpVar);
            if(retVar!=nullptr)
            {
                retVar->clear();
            }
			return tmpVar;
		}
	}

	return nullptr;
}

/**
* \brief  This method enters the next hierarchy level (e.g. when entering an "If .. Then ... Endif")
*/
void VariableManagement::enterNextHierarchyLevel()
{
    currHierarchyLevel++;
    while((int) hierachyVars.size() <= currHierarchyLevel)
    {
        hierachyVars.push_back(allocator.allocateVectorMap());
    }
	hierachyVars[currHierarchyLevel]->push_back(nullptr);
}

/**
* \brief  This method leaves current hierarchy level (e.g. when leaving an "If .. Then ... Endif")
*/
void VariableManagement::leaveCurrentHierarchyLevel()
{
    Hashtable<AalVariable>* tmpMap=hierachyVars[currHierarchyLevel]->back();
	if(tmpMap!=nullptr)
	{
		std::vector<int>* setIdx=tmpMap->getSetIdx();
		unsigned int capacity=setIdx->size();
		AalVariable* tmpVar;
		for(unsigned int i=0; i < capacity ;i++)
		{
			tmpVar=tmpMap->getByIndex(setIdx->at(i));
            allocator.recycleAalVar(tmpVar);
		}
		allocator.recycleMap(tmpMap);
	}
    hierachyVars[currHierarchyLevel]->pop_back();
    currHierarchyLevel--;
}

/**
* \brief  This method enters next function level (e.g. when entering "Func ... EndFunc")
*
* \param currLine [in] Save the line from where the call came. So we can later on process further.
*/
void VariableManagement::enterFunction(const int currLine)
{
    informationStack.push_back(currHierarchyLevel);
    informationStack.push_back(currLine);
    currHierarchyLevel=-1;
    enterNextHierarchyLevel();
    currFunctionDepth++;
}

/**
* \brief  This method leaves current function level (e.g. when leaving "Func ... EndFunc")
*
* \param currLine [out] Set the line from where the function call came.
*/
void VariableManagement::leaveFunction(int& currLine)
{
    if(currFunctionDepth<=0)
        return;

    while(currHierarchyLevel>-1)
    {
        leaveCurrentHierarchyLevel();
    }

    currLine=informationStack.back();
    informationStack.pop_back();
    currHierarchyLevel=informationStack.back();
    informationStack.pop_back();
    currFunctionDepth--;
}

