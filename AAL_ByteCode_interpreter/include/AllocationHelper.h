#ifndef ALLOCATIONHELPER_H
#define ALLOCATIONHELPER_H

#include <string>
#include <vector>
#include <map>
#include "Hashtable.h"
#include "../src/Hashtable.cpp"
#include "AalVariable.h"

/**
* \brief This class makes allocation faster and more efficient by allocating big chunks or/and reusing old allocated blocks.
*        Do never ever use "new" or "delete" for memory allocated with this class otherwise you will get memory leaks/segmentation-faults for sure.
*/
class AllocationHelper
{
    public:
        AllocationHelper();
        virtual ~AllocationHelper();

        double* allocateDouble();
        long long* allocateLongLong();
        std::string* allocateString();
		AalVariable* allocateAalVar();
		std::vector<void*>* allocateVoidVector();
		std::vector<char>* allocateCharVector();
		Hashtable<AalVariable>* allocateMap();
		std::vector<Hashtable<AalVariable>*>* allocateVectorMap();
		std::vector<AalVariable*>* allocateVecVar();
        std::vector<std::vector<AalVariable*>*>* allocateVecVecVar();
        std::shared_ptr<void>* allocateSharedPtr();

		void recycleAalVar(AalVariable* ptr);
		void recycleString(std::string*);
		void recycleDouble(double*);
		void recycleLongLong(long long*);
		void recycleSharedPtr(std::shared_ptr<void>*);
		void recycleCharVector(std::vector<char>*);
		void recycleVoidVector(std::vector<void*>*);

		void recycleMap(Hashtable<AalVariable>*);
		void recycleVectorMap(std::vector<Hashtable<AalVariable>*>*);

		void recycleVecVecVar(std::vector<std::vector<AalVariable*>*>*);
		void recycleVecVar(std::vector<AalVariable*>*);

    private:
        NO_INLINE double* _allocateDouble();
        NO_INLINE long long* _allocateLongLong();
        NO_INLINE std::string* _allocateString();
		NO_INLINE AalVariable* _allocateAalVar();
		NO_INLINE std::vector<void*>* _allocateVoidVector();
		NO_INLINE std::vector<char>* _allocateCharVector();
		NO_INLINE Hashtable<AalVariable>* _allocateMap();
		NO_INLINE std::vector<Hashtable<AalVariable>*>* _allocateVectorMap();
		NO_INLINE std::vector<AalVariable*>* _allocateVecVar();
        NO_INLINE std::vector<std::vector<AalVariable*>*>* _allocateVecVecVar();
        NO_INLINE std::shared_ptr<void>* _allocateSharedPtr();

        std::vector<double*> allocatedDoubles;
        std::vector<std::string*> allocatedStrings;
        std::vector<long long*> allocatedLongs;
		std::vector<AalVariable*> allocatedAalVars;
		std::vector<std::vector<void*>*> allocatedVoidVectors;
		std::vector<std::vector<char>*> allocatedCharVectors;
        std::vector<std::shared_ptr<void>*> allocatedSharedPtr;

		std::vector<std::vector<Hashtable<AalVariable>*>*> vectorMapsToFree;
		std::vector<Hashtable<AalVariable>*> mapsToFree;

		std::vector<Hashtable<AalVariable>*> unusedMaps;
		std::vector<std::vector<Hashtable<AalVariable>*>*> unusedVectorMaps;

        std::vector<std::vector<std::vector<AalVariable*>*>*> unusedVecVecVars;
        std::vector<std::vector<std::vector<AalVariable*>*>*> vecVecVarsToFree;
        std::vector<std::vector<AalVariable*>*> unusedVecVars;
        std::vector<std::vector<AalVariable*>*> vecVarsToFree;

		unsigned int mapAllocationsAtOnce=1024;
		unsigned int vectorMapAllocationsAtOnce=1024;
        unsigned int vecVecVarAllocationsAtOnce=1024;
        unsigned int vecVarAllocationsAtOnce=1024;

        unsigned int vecVecVarLastUsedObject=vecVecVarAllocationsAtOnce;
        unsigned int vecVarLastUsedObject=vecVarAllocationsAtOnce;
		unsigned int mapLastUsedObject=mapAllocationsAtOnce;
		unsigned int vectorMapLastUsedObject=vectorMapAllocationsAtOnce;
		unsigned int maximumPreAllocs=1024;


        #if SHOW_MEM_ALLOCATIONS >= 1
            unsigned int currentAllocatedBlocks=0;
        #endif

        #if SHOW_MEM_ALLOCATIONS >= 3
            std::map<std::string,int> usedAdress;
        #endif
};

extern AllocationHelper allocator;
#endif // ALLOCATIONHELPER_H
