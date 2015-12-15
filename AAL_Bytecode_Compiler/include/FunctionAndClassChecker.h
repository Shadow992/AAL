#ifndef FUNCTIONANDCLASSCHECKER_H
#define FUNCTIONANDCLASSCHECKER_H

#include <map>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <math.h>

#include "TokenTypes.h"
#include "commonFuncs.h"
#include "Token.h"

class FunctionAndClassChecker
{
    public:
        FunctionAndClassChecker();
        virtual ~FunctionAndClassChecker();

        bool isFunctionAvailable(const std::string&);
        bool isClassAvailable(const std::string&);
        bool isAvailable(const std::string& str);

        void addClass(const std::string&);
        void addFunction(const std::string&);
    private:
        std::map<std::string, bool> knownClasses;
        std::map<std::string, bool> knownFunctions;
};

#endif // FUNCTIONANDCLASSCHECKER_H
