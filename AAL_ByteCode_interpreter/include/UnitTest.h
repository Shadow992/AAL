#ifndef UNITTEST_H
#define UNITTEST_H

#include "AllocationHelper.h"
#include "commonFuncs.h"
#include "TokenTypes.h"
#include "defines.h"
#include "BytecodeParser.h"
#include "AalVariable.h"
#include "Interpreter.h"
#include "VariableManagement.h"
#include "Hashtable.h"
#include <assert.h>

/**
* \brief A small class which do some very basic tests to ensure some basic functionality
*/
class UnitTest
{
    public:
        UnitTest();
        virtual ~UnitTest();
        void doAllTests();
        void testBytecodeParser();
        void testVariableManagement();
        void testAALVariable();
        void testAALArray();
        void testHashTable();
    protected:
    private:
};

#endif // UNITTEST_H
