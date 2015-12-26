#ifndef AALVARIABLE_H
#define AALVARIABLE_H

#include "defines.h"
#include "commonFuncs.h"
#include "TokenTypes.h"
#include "InternalClassWrapper.h"

#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <memory>

const std::string STR_TRUE="1";
const std::string STR_FALSE="0";
const std::string STR_EMPTY="";
const std::string STR_DOT=".";
const char CHR_TRUE='1';
const char CHR_FALSE='0';
const char CHR_EMPTY='\0';
const char CHR_DOT='.';
const char CHAR_TRUE=1;
const char CHAR_FALSE=0;

struct ArrayInformationStruct;

/**
* \brief This class is used for representing a variable in AAL-Bytecode (e.g. $var is an object of AalVariable class)
*/
class AalVariable
{
    public:
        AalVariable();
        virtual ~AalVariable();

		char getType(const int idx);
        void makeCompareable(void*& var1, char& valueType1,void*& var2, char& valueType2);
        AalVariable* getAalEndVar(AalVariable* var);
        AalVariable* getVariablePointer(const int idx=0);
        bool isDouble(const int idx=0);
		bool isLong(const int idx=0);
		bool isString(const int idx=0);
        void convertToDouble(const int idx=0);
        void convertToLong(const int idx=0);
        void convertToString(const int idx=0);
        long long* getLongPointer(const int idx=0);
        double* getDoublePointer(const int idx=0);
        std::string* getStringPointer(const int idx=0);
        void setValue(const std::string& val, char type, int idx=0);
		void setValue(AalVariable* var, int idx=0,int idx2=0);
		void setArrayValue(AalVariable* var, int idx=0,int idx2=0);
        void setType(const int idx,char type);
		static void clear(AalVariable* var);
		void clear();
		void clearCompletely(AalVariable* var);
		void clearCompletely();
		void clearArray(AalVariable* var);
		void clearArray();
		void clearValues();
        void* getVoidPointer(const int idx);

        std::shared_ptr<InternalClassWrapper>* getSharedPointer(int idxDest);
		void construct(char type=TYPE_STRING);
        void makePointTo(AalVariable* var);

        std::string getStringValue(int idx=0);
        double getDoubleValue(int idx=0);
        long long getLongValue(int idx=0);

		bool isTrue(int idx=0);
		bool isFalse(int idx=0);

        void join(const std::string&, const std::string&);
        void append(const std::string&);
        void add(const long long, const long long);
        void sub(const long long, const long long);
        void mul(const long long, const long long);
        void div(const long long, const long long);
        void pow(const long long, const long long);
		void shl(const long long, const long long);
        void shr(const long long, const long long);
        void mod(const long long, const long long);

        void add(const double, const double);
        void sub(const double, const double);
        void mul(const double, const double);
        void div(const double, const double);
        void pow(const double, const double);

		bool compEqCase(const long long);
		bool compEqIncase(const long long);
		bool less(const long long);

        bool compEqCase(const double);
		bool compEqIncase(const double);
		bool less(const double);

        bool compEqCase(const std::string&);
		bool compEqIncase(const std::string&);
		bool less(const std::string&);

        void setValue(const long long val, char type,int idx=0);
        void setValue(const double val, char type,int idx=0);
        void setTrue();
        void setFalse();

		void appendDimension(AalVariable* var, unsigned int arraySize, ArrayInformationStruct* arrayStruct,int& lastUsedVectorIdx,int& lastUsedIdx,AalVariable* usedVars,std::vector<char>* usedCharVectors,std::vector<void*>* usedVoidVectors);
		void appendDimension(unsigned int arraySize);
		void createArray(unsigned int arraySize, ArrayInformationStruct* arrayStruct,int& lastUsedVectorIdx,int& lastUsedIdx, AalVariable* usedVars,std::vector<char>* usedCharVectors,std::vector<void*>* usedVoidVectors);
        void copyArray(AalVariable* src, AalVariable* dest,ArrayInformationStruct* arrayInfoStruct);

		void printVarRecursive(AalVariable* var, int depth);
		void printVarRecursive();


		void appendAsParameter(double);
		void appendAsParameter(long long);
		void appendAsParameter(const std::string&);
		void appendAsParameter(AalVariable*);
        unsigned int getArgumentCount();
		unsigned int getValueSize();

        ArrayInformationStruct* arrayInfo=nullptr; /**< Holding nullptr if variable is no array, otherwise holding some array information*/
    private:
        std::vector<void*>* value=nullptr;
        std::vector<char>* valueType=nullptr;
		AalVariable* pointsTo=nullptr;
		char isBaseArray=CHAR_FALSE;
};

/**
* \brief This struct is holding array information of an AalVariable (only used if AalVariable is an array, e.g. after calling AalVariable::appendDimension(unsigned int arraySize)).
*/
struct ArrayInformationStruct
{
	unsigned int overallArraySize; /**< The size of the complete array (e.g. $var[10][10] would have got overallArraySize=10*10=100) */
	std::vector<AalVariable*> usedAalVarChunks; /**< Holding the allocated variables used for this array */
	std::vector<std::vector<void*>*> usedValueChunks; /**< Holding the allocated value-vectors used for this array */
	std::vector<std::vector<char>*> usedValueTypeChunks; /**< Holding the allocated valueType-vectors used for this array */
};

#endif // AALVARIABLE_H
