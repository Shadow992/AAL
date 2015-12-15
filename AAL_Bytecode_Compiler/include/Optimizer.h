#ifndef OPTIMIZER_H_INCLUDED
#define OPTIMIZER_H_INCLUDED

#include "TokenTypes.h"
#include "commonFuncs.h"
#include "Token.h"
#include <list>
#include <stdlib.h>
#include <math.h>

#if USE_MULTITHREADING == 1
	#include <thread>
	#include <mutex>
#endif

class Optimizer
{
	public:
	Optimizer(){};
	~Optimizer(){};
	void doBasicCalculations(Token* currToken);
    void doBasicCalculationsThreaded(Token* rootToken, std::list<Token*>::iterator it, int cThreads);
    void extractVariableAssignments(Token* currToken, std::vector<Token*>& extracted);
    void optimize(Token* rootToken, int level);

    private:
        int const THREAD_COUNT=8;
};


#endif // OPTIMIZER_H_INCLUDED
