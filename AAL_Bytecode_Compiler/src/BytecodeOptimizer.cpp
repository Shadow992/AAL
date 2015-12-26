#include "BytecodeOptimizer.h"

BytecodeOptimizer::BytecodeOptimizer()
{

}

BytecodeOptimizer::~BytecodeOptimizer()
{
    //dtor
}

/**
 *  \brief This function replaces all jump labels in AAL-Bytecode with the corresponding line number
 *  
 *  \param parsedLines Parsed AAL-Bytecode to remove jmp labels from
 *  \param jmpLabels Map of all found labels and their line number in code
 */
void BytecodeOptimizer::removeJmpLabels(std::list<BytecodeLine*>& parsedLines, std::map<std::string, int>& jmpLabels)
{
	#if SHOW_TIMINGS == 1
		auto start = std::chrono::steady_clock::now();
	#endif

	BytecodeLine* currLine;
	auto lastIt=parsedLines.begin();
	for(auto it=parsedLines.begin();it!=parsedLines.end();++it)
	{
		currLine=*it;
		if(currLine->type==LINE_LABEL)
		{
			if((currLine->arg1)[0]!='_')
			{
				if(it==lastIt)
				{
					parsedLines.erase(it);
					it=parsedLines.begin();
				}
				else
				{
					parsedLines.erase(it);
					it=lastIt;
				}
			}
		}
		else if(currLine->type==LINE_JMP)
		{
			if((currLine->arg1)[0]!='_')
			{
				currLine->arg1=toString(jmpLabels[currLine->arg1]);
			}
		}
		else if(currLine->type==LINE_TRUE_JMP || currLine->type==LINE_FALSE_JMP)
		{
			if((currLine->arg2)[0]!='_')
			{
				currLine->arg2=toString(jmpLabels[currLine->arg2]);
			}
		}
		lastIt=it;
	}

	#if SHOW_TIMINGS == 1
		auto end = std::chrono::steady_clock::now();
		auto diff = end - start;
		std::cout << "Removing Jmp-Labels: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
		start = std::chrono::steady_clock::now();
	#endif
}
