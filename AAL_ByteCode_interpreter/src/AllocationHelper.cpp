#include "AllocationHelper.h"

AllocationHelper allocator;

AllocationHelper::AllocationHelper()
{
	allocatedStrings.reserve(1024);
	allocatedLongs.reserve(1024);
	allocatedDoubles.reserve(1024);
	allocatedVoidVectors.reserve(1024);
	allocatedCharVectors.reserve(1024);
	allocatedAalVars.reserve(1024);
	allocatedAalVars.push_back(new AalVariable);
	allocatedAalVars.push_back(new AalVariable);
	allocatedAalVars.push_back(new AalVariable);
	allocatedAalVars.push_back(new AalVariable);
	allocatedAalVars.push_back(new AalVariable);
}

AllocationHelper::~AllocationHelper()
{
    for(unsigned int i=0;i<allocatedAalVars.size();i++)
	{
		allocatedAalVars[i]->clear();
		delete allocatedAalVars[i];
	}

	for(unsigned int i=0;i<allocatedStrings.size();i++)
	{
		delete allocatedStrings[i];
	}

	for(unsigned int i=0;i<allocatedLongs.size();i++)
	{
		delete allocatedLongs[i];
	}

	for(unsigned int i=0;i<allocatedDoubles.size();i++)
	{
		delete allocatedDoubles[i];
	}

    for(unsigned int i=0;i<allocatedSharedPtr.size();i++)
	{
		delete allocatedSharedPtr[i];
	}

	for(unsigned int i=0;i<allocatedCharVectors.size();i++)
	{
		delete allocatedCharVectors[i];
	}

	for(unsigned int i=0;i<allocatedVoidVectors.size();i++)
	{
		delete allocatedVoidVectors[i];
	}

	for(unsigned int i=0;i<mapsToFree.size();i++)
	{
		delete[] mapsToFree[i];
	}

	for(unsigned int i=0;i<vectorMapsToFree.size();i++)
	{
		delete[] vectorMapsToFree[i];
	}

    #if SHOW_MEM_ALLOCATIONS >= 1
        std::cout<<std::endl<<"Not deleted Blocks left: "<< currentAllocatedBlocks <<std::endl<<std::endl;
    #endif

    #if SHOW_MEM_ALLOCATIONS == 3
        for(auto it=usedAdress.begin();it!=usedAdress.end();++it)
        {
            if(it->second!=0)
            {
                std::cout<<it->second<<" "<<it->first<<" block(s) not deleted!"<<std::endl;
            }
        }
	#endif

}


NO_INLINE std::shared_ptr<void>* AllocationHelper::_allocateSharedPtr()
{
	return new std::shared_ptr<void>;
}

std::shared_ptr<void>* AllocationHelper::allocateSharedPtr()
{
	std::shared_ptr<void>* ptr;
	if(!allocatedLongs.empty())
	{
		ptr=allocatedSharedPtr.back();
		allocatedSharedPtr.pop_back();
	}
	else
	{
		ptr=_allocateSharedPtr();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (std::shared_ptr<void>): "<<ptr<<std::endl;
	#endif

    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::shared_ptr<void>"]++;
	#endif

	return ptr;
}

/**
* \brief This method allocates memory for a long long and returns it (similar to "return new long long;")
*
* \return Allocated long long
*
*/
long long* AllocationHelper::allocateLongLong()
{
	long long* ptr;
	if(!allocatedLongs.empty())
	{
		ptr=allocatedLongs.back();
		allocatedLongs.pop_back();
	}
	else
	{
		ptr=_allocateLongLong();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (long long): "<<ptr<<std::endl;
	#endif

    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["long long"]++;
	#endif

	return ptr;
}

NO_INLINE long long* AllocationHelper::_allocateLongLong()
{
	return new long long;
}

/**
* \brief This method allocates memory for a double and returns it (similar to "return new long long;")
*
* \return Allocated double
*
*/
double* AllocationHelper::allocateDouble()
{
	double* ptr;
	if(!allocatedDoubles.empty())
	{
		ptr=allocatedDoubles.back();
		allocatedDoubles.pop_back();
	}
	else
	{
		ptr=_allocateDouble();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (double): "<<ptr<<std::endl;
	#endif

	#if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["double"]++;
	#endif
	return ptr;
}

NO_INLINE double* AllocationHelper::_allocateDouble()
{
	return new double;
}

/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
std::string* AllocationHelper::allocateString()
{
	std::string* ptr;
	if(!allocatedStrings.empty())
	{
		ptr=allocatedStrings.back();
		allocatedStrings.pop_back();
	}
	else
	{
		ptr=_allocateString();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (string): "<<ptr<<std::endl;
	#endif
	#if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["string"]++;
	#endif

	return ptr;
}

NO_INLINE std::string* AllocationHelper::_allocateString()
{
	return new std::string;
}

/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated std::vector<char>
*
*/
std::vector<char>* AllocationHelper::allocateCharVector()
{
	std::vector<char>* ptr;
	if(!allocatedCharVectors.empty())
	{
		ptr=allocatedCharVectors.back();
		allocatedCharVectors.pop_back();
	}
	else
	{
		ptr=_allocateCharVector();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (std::vector<char>): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<char>"]++;
	#endif

	return ptr;
}

NO_INLINE std::vector<char>* AllocationHelper::_allocateCharVector()
{
	return new std::vector<char>;
}

/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
std::vector<void*>* AllocationHelper::allocateVoidVector()
{
	std::vector<void*>* ptr;
	if(!allocatedVoidVectors.empty())
	{
		ptr=allocatedVoidVectors.back();
		allocatedVoidVectors.pop_back();
	}
	else
	{
		ptr=_allocateVoidVector();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (std::vector<void*>): "<<ptr<<std::endl;
	#endif
	#if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<void*>"]++;
	#endif
	return ptr;
}

NO_INLINE std::vector<void*>* AllocationHelper::_allocateVoidVector()
{
	return new std::vector<void*>;
}

/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
AalVariable* AllocationHelper::allocateAalVar()
{
	AalVariable* ptr;
	if(!allocatedAalVars.empty())
	{
		ptr=allocatedAalVars.back();
		allocatedAalVars.pop_back();
	}
	else
	{
	    ptr=_allocateAalVar();
	}
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (var): "<<ptr<<std::endl;
	#endif
	#if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["var"]++;
	#endif
	return ptr;
}

NO_INLINE AalVariable* AllocationHelper::_allocateAalVar()
{
    return new AalVariable;
}


/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
Hashtable<AalVariable>* AllocationHelper::allocateMap()
{
	Hashtable<AalVariable>* ptrMap;
	if(!unusedMaps.empty())
	{
		ptrMap=unusedMaps.back();
		unusedMaps.pop_back();
	}
	else
	{
        ptrMap=_allocateMap();
	}

    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (Hashtable<AalVariable>): "<<ptrMap<<std::endl;
	#endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["Hashtable<AalVariable>"]++;
	#endif
	return ptrMap;
}

NO_INLINE Hashtable<AalVariable>* AllocationHelper::_allocateMap()
{
	Hashtable<AalVariable>* ptrMap;
    if(mapLastUsedObject<mapAllocationsAtOnce)
    {
        ptrMap=&(mapsToFree.back()[mapLastUsedObject]);
        mapLastUsedObject++;
    }
    else
    {
        if(mapAllocationsAtOnce<200000)
        {
            mapAllocationsAtOnce<<=1;
            unusedMaps.reserve(mapAllocationsAtOnce);
        }
        ptrMap=new Hashtable<AalVariable>[mapAllocationsAtOnce];
        mapsToFree.push_back(ptrMap);
        mapLastUsedObject=1;
        ptrMap=&(ptrMap[0]);
    }
	return ptrMap;
}

/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
std::vector<Hashtable<AalVariable>*>* AllocationHelper::allocateVectorMap()
{
	std::vector<Hashtable<AalVariable>*>* ptrVectorMap;
	if(!unusedVectorMaps.empty())
	{
        ptrVectorMap=unusedVectorMaps.back();
		unusedVectorMaps.pop_back();
	}
	else
	{
		ptrVectorMap=_allocateVectorMap();
	}

    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (std::vector<Hashtable<AalVariable>*>): "<<ptrVectorMap<<std::endl;
	#endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<Hashtable<AalVariable>*>"]++;
	#endif

	return ptrVectorMap;
}

NO_INLINE std::vector<Hashtable<AalVariable>*>* AllocationHelper::_allocateVectorMap()
{
	std::vector<Hashtable<AalVariable>*>* ptrVectorMap;

    if(vectorMapLastUsedObject>=vectorMapAllocationsAtOnce)
    {
        if(vectorMapAllocationsAtOnce<200000)
        {
            vectorMapAllocationsAtOnce<<=1;
            unusedVectorMaps.reserve(vectorMapAllocationsAtOnce);
        }
        ptrVectorMap=new std::vector<Hashtable<AalVariable>*>[vectorMapAllocationsAtOnce];
        vectorMapsToFree.push_back(ptrVectorMap);
        vectorMapLastUsedObject=1;
        ptrVectorMap=&(ptrVectorMap[0]);
    }
    else
    {
        ptrVectorMap=&(vectorMapsToFree.back()[vectorMapLastUsedObject]);
        vectorMapLastUsedObject++;
    }

	return ptrVectorMap;
}


/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
std::vector<AalVariable*>* AllocationHelper::allocateVecVar()
{
	std::vector<AalVariable*>* ptrVecVar;
	if(!unusedVecVars.empty())
	{
        ptrVecVar=unusedVecVars.back();
		unusedVecVars.pop_back();
	}
	else
	{
	    ptrVecVar=_allocateVecVar();
	}

    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (std::vector<AalVariable*>): "<<ptrVecVar<<std::endl;
	#endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<AalVariable*>"]++;
	#endif

	return ptrVecVar;
}


NO_INLINE std::vector<AalVariable*>* AllocationHelper::_allocateVecVar()
{
	std::vector<AalVariable*>* ptrVecVar;

    if(vecVarLastUsedObject>=vecVarAllocationsAtOnce)
    {
        if(vecVarAllocationsAtOnce<200000)
        {
            vecVarAllocationsAtOnce<<=1;
            unusedVecVars.reserve(vecVarAllocationsAtOnce);
        }
        ptrVecVar=new std::vector<AalVariable*>[vecVarAllocationsAtOnce];
        vecVarsToFree.push_back(ptrVecVar);
        vecVarLastUsedObject=1;
        ptrVecVar=&(ptrVecVar[0]);
    }
    else
    {
        ptrVecVar=&(vecVarsToFree.back()[vecVarLastUsedObject]);
        vecVarLastUsedObject++;
    }

	return ptrVecVar;
}



/**
* \brief This method allocates memory (similar to "new" keyword)
*
* \return Allocated memory
*
*/
std::vector<std::vector<AalVariable*>*>* AllocationHelper::allocateVecVecVar()
{
	std::vector<std::vector<AalVariable*>*>* ptrVecVecVar;
	if(unusedVecVecVars.empty())
	{
        ptrVecVecVar=unusedVecVecVars.back();
		unusedVecVecVars.pop_back();
	}
	else
	{
        ptrVecVecVar=_allocateVecVecVar();
	}

    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks++;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"alloced (std::vector<std::vector<AalVariable*>*>): "<<ptrVecVecVar<<std::endl;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<std::vector<AalVariable*>*>"]++;
	#endif

	return ptrVecVecVar;
}

NO_INLINE std::vector<std::vector<AalVariable*>*>* AllocationHelper::_allocateVecVecVar()
{
	std::vector<std::vector<AalVariable*>*>* ptrVecVecVar;

    if(vecVecVarLastUsedObject>=vecVecVarAllocationsAtOnce)
    {
        if(vecVecVarAllocationsAtOnce<200000)
        {
            vecVecVarAllocationsAtOnce<<=1;
            unusedVecVecVars.reserve(vecVecVarAllocationsAtOnce);
        }
        ptrVecVecVar=new std::vector<std::vector<AalVariable*>*>[vecVecVarAllocationsAtOnce];
        vecVecVarsToFree.push_back(ptrVecVecVar);
        vecVecVarLastUsedObject=1;
        ptrVecVecVar=&(ptrVecVecVar[0]);
    }
    else
    {
        ptrVecVecVar=&(vecVecVarsToFree.back()[vecVecVarLastUsedObject]);
        vecVecVarLastUsedObject++;
    }

	return ptrVecVecVar;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleAalVar(AalVariable* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (var): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["var"]--;
	#endif

	ptr->clear();
	if(allocatedAalVars.size()<maximumPreAllocs)
	{
		allocatedAalVars.push_back(ptr);
		return;
	}
	delete ptr;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleString(std::string* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (string): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["string"]--;
	#endif

	if(allocatedStrings.size()<maximumPreAllocs)
	{
		ptr->clear();
		allocatedStrings.push_back(ptr);
		return;
	}
	delete ptr;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleDouble(double* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (double): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["double"]--;
	#endif

	if(allocatedDoubles.size()<maximumPreAllocs)
	{
		allocatedDoubles.push_back(ptr);
		return;
	}
	delete ptr;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleLongLong(long long* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (long long): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["long long"]--;
	#endif
	if(allocatedLongs.size()<maximumPreAllocs)
	{
		allocatedLongs.push_back(ptr);
		return;
	}
	delete ptr;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleVoidVector(std::vector<void*>* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (void vec): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<void*>"]--;
	#endif

	if(allocatedVoidVectors.size()<maximumPreAllocs)
	{
		ptr->clear();
		allocatedVoidVectors.push_back(ptr);
		return;
	}
	delete ptr;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleCharVector(std::vector<char>* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (char vec): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<char>"]--;
	#endif

	if(allocatedCharVectors.size()<maximumPreAllocs)
	{
		ptr->clear();
		allocatedCharVectors.push_back(ptr);
		return;
	}
	delete ptr;
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleMap(Hashtable<AalVariable>* ptr)
{
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (Hashtable<AalVariable>): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["Hashtable<AalVariable>"]--;
	#endif

	ptr->clear();
	unusedMaps.push_back(ptr);
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleVectorMap(std::vector<Hashtable<AalVariable>*>* ptr)
{
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (std::vector<Hashtable<AalVariable>*>): "<<ptr<<std::endl;
    #endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<Hashtable<AalVariable>*>"]--;
	#endif

	ptr->clear();
	unusedVectorMaps.push_back(ptr);
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleVecVecVar(std::vector<std::vector<AalVariable*>*>* ptr)
{
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (std::vector<std::vector<AalVariable*>*>): "<<ptr<<std::endl;
    #endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<std::vector<AalVariable*>"]--;
	#endif

	ptr->clear();
	unusedVecVecVars.push_back(ptr);
}

/**
* \brief This method deletes allocated memory (similar to "delete" keyword)
*/
void AllocationHelper::recycleVecVar(std::vector<AalVariable*>* ptr)
{
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif
	#if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (std::vector<AalVariable*>): "<<ptr<<std::endl;
    #endif

    #if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::vector<AalVariable*>"]--;
	#endif

	ptr->clear();
	unusedVecVars.push_back(ptr);
}

void AllocationHelper::recycleSharedPtr(std::shared_ptr<void>* ptr)
{
    #if SHOW_MEM_ALLOCATIONS == 2
        std::cout<<"deleted (std::shared_ptr<void>): "<<ptr<<std::endl;
    #endif
    #if SHOW_MEM_ALLOCATIONS >= 1
        currentAllocatedBlocks--;
	#endif

	#if SHOW_MEM_ALLOCATIONS == 3
        usedAdress["std::shared_ptr<void>"]--;
	#endif

	if(allocatedSharedPtr.size()<maximumPreAllocs)
	{
	    ptr->reset();
		allocatedSharedPtr.push_back(ptr);
		return;
	}
	delete ptr;
}

