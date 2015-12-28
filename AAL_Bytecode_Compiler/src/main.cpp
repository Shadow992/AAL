#include "commonFuncs.h"
#include "Token.h"
#include "Tokenizer.h"
#include "TokenTypes.h"
#include "Parser.h"
#include "SyntaxValidator.h"
#include "PreProcessor.h"
#include "Compiler.h"
#include "Optimizer.h"
#include "FunctionAndClassChecker.h"
#include "defines.h"
#include "BytecodeParser.h"
#include "BytecodeOptimizer.h"
#include "OneWayHasher.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <unordered_map>

/*
Some quick and dirty solution if you want to build all in one and at once

#include "Optimizer.cpp"
#include "commonFuncs.cpp"
#include "Token.cpp"
#include "Tokenizer.cpp"
#include "Parser.cpp"
#include "SyntaxValidator.cpp"
#include "PreProcessor.cpp"
#include "Compiler.cpp"
#include "FunctionAndClassChecker.cpp"
#include "BytecodeParser.cpp"
#include "BytecodeOptimizer.cpp"
#include "OneWayHasher.cpp"
#include "DynamicBitSet.cpp"
*/

int main( int argc, const char* argv[] )
{
    Compiler tmp2;
    #if SHOW_TIMINGS == 1
        auto startAll = std::chrono::steady_clock::now();
        //auto startNumbers = std::chrono::steady_clock::now();
    #endif

    std::string srcFile=argv[1];
    std::string destFile=argv[2];
    std::string includeDir=argv[3];
    std::string toParse=readStringFile(srcFile);
    srcFile+=".tmp.bin";
    tmp2.compile(toParse,srcFile,includeDir);


    std::string compiledBytecode=readStringFile(srcFile);
    BytecodeParser byteParser;
    byteParser.parseBytecode(compiledBytecode,false);

    BytecodeOptimizer byteOptimzer;
    byteOptimzer.removeJmpLabels(byteParser.parsedLines,byteParser.jmpLabels);
    byteParser.writeToFile(destFile,false);


    #if SHOW_TIMINGS == 1
        auto endAll = std::chrono::steady_clock::now();
        auto diffAll = endAll - startAll;
        std::cout << "Overall processing: " << std::chrono::duration <double, std::milli> (diffAll).count() << "ms" << std::endl;
    #endif
	return 0;
}
