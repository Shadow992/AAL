#ifndef BYTECODEGENERATOR_H
#define BYTECODEGENERATOR_H

#include "Token.h"
#include <stdlib.h>
#include <string>

class BytecodeGenerator
{
    public:
        BytecodeGenerator();
        virtual ~BytecodeGenerator();

        std::string getByteCode(TokenTypes type);
    private:
};

#endif // BYTECODEGENERATOR_H
