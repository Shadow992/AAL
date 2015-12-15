#ifndef BYTECODEOPTIMIZER_H
#define BYTECODEOPTIMIZER_H

#include "TokenTypes.h"
#include "commonFuncs.h"
#include "BytecodeParser.h"
#include <stdlib.h>
#include <string>
#include <list>
#include <iostream>
#include "defines.h"

class BytecodeOptimizer
{
    public:
        BytecodeOptimizer();
        virtual ~BytecodeOptimizer();

        void removeJmpLabels(std::list<BytecodeLine*> &parsedLines, std::map<std::string, int>& jmpLabels);

    private:
};

#endif // BYTECODEOPTIMIZER_H
