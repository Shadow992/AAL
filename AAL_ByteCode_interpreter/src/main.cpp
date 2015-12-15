#include "AllocationHelper.h"
#include "commonFuncs.h"
#include "TokenTypes.h"
#include "defines.h"
#include "BytecodeParser.h"
#include "AalVariable.h"
#include "Interpreter.h"
#include "VariableManagement.h"
#include "Hashtable.h"
#include "UnitTest.h"
#include "StandardFunctions.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <map>

/*
#include "commonFuncs.cpp"
#include "BytecodeParser.cpp"
#include "AalVariable.cpp"
#include "AllocationHelper.cpp"
#include "Interpreter.cpp"
#include "VariableManagement.cpp"
#include "Hashtable.cpp"
#include "UnitTest.cpp"
#include "StandardFunctions.cpp"
*/

/** \mainpage Advanced Automation Language - Interpreter Part (AAL)
 *
 * \section reasons Reasons for developing AAL
 *
 * AAL was developed because I really like the way AutoIt is working and what is possible with AutoIt.
 * But there always have been several things that I really did not like, e.g:
 * - Missing class support (only possible with UDF and then still quite strange to use)
 * - Quite slow
 * - Very Easy to decompile and nearly impossible to break existing decompilers
 * - No possibility to use real multithreading
 * - Missing or very complicated hardware near programming (e.g. complicated pointer and structs)
 * - Sometimes strange behaviour and inconsistent keywords (e.g. "While" and "WEnd" but "If" and "EndIf" or "For" and "Next").
 * - Kind of missing inline ASM
 * - Missing possibility to compile as DLL
 *
 * There are even more things but these thing were the most annoying to me.
 *
 * \section explain_all_working How does AAL work?
 *
 * AAL consists of two software projects.
 * - AAL-Compiler: This software converts a given AAL code into an AAL-Bytedcode
 * - AAL-Interpreter: This software interprets the generated AAL-Bytecode
 *
 * I use this approach to speed up interpreting of AAL-Code.
 * By interpreting bytecode and not the full language itself we can move many complex things and tasks to the compiler.
 * The compiler can be quite slow so it is not important to use good and fast algorithms but only good algorithms.
 * This enabled me to write quite complex algorithms in a well understandable way, at least for the compiler part.
 *
 * \section faq FAQ
 *
 * \subsection autoit Can we execute AutoIt-Scripts without changing anything in AAL?
 * AAL aims to be very similar to AutoIt, so many scripts (especially small and quite general one) may work without any changes to the AutoIt-Code.
 * Even when there may be many functions, in the final product version, that are working as the equivalent ones in AutoIt, there may still be some functions behaving different.
 * The goal is not to create a better AutoIt-Version nor is it to create a copy of AutoIt.
 * So if there are functions missing or named different, it was done on purpose and will (if there are no good reasons to change this) not be modified.
 * The goal is to create an AutoIt alternative with better/more/consistent functions/syntax/semantic and not to copy AutoIt!
 *
 * \subsection not_ready When will AAL be a serious competitor to AutoIt?
 * At the moment (11. November, 2015; v0.0.1) there are many things left to do.
 * However I guess it will take several months of work until AAL is able to replace AutoIt completly.
 * The goal is also not to replace AutoIt completly but to give an alternative which can be used especially for time critical and hardware near problems.
 *
 * \subsection correct_code_error Why do I get some strange errors (e.g. "segmentation-fault" or "XYZ throw an error of ABC"), though my AAL-Code is correct for 100% and also works with AutoIt?
 * If this happens and the AAL-Code is not using any special commands/assumptions, it is likely that you found a bug in AAL-Software.
 * This will happen quite frequently in alpha and maybe in beta phase. If such an error occured it would be extremly nice, if you could send the error and the AAL-Code to me (e-mail: m.rupprecht@gmx.net).
 * Thanks in advance.
 *
 * \subsection bug_found I found a bug, what should I do now?
 * It would be really great if you could send me an email to "m.rupprecht@gmx.net" (without the quotes).
 * The best would be if you send me the AAL-Code, the compiled file and a screenshot of the error + some details (e.g. How long did it run? Does this error occur always? ...)
 * I am happy about every kind of help. :)
 *
 * \subsection want_to_help I want to help by making this project better!
 * There are several ways to help me making progress:
 * - I am always happy to see guys helping me finding bugs/errors/strange things. So if you want to help me not just with some short texts/functions, you can write me an email and I will help you getting familiar with code. I will not explain fundamental C/C++ things (so I will not act as a kind of teacher/tutor but will help to answer all unclear codes and explain the basic ideas of the algorithms)
 * - If you are having not enough time to work regulary at the project but you still want to contribute, you can also send me some pieces of C/C++ code (e.g. PictureSearch-Algorithms, faster implementation of existing functions, ...) and I will add them on occasion.
 * - If cou can not program C/C++ at all, you can also help to write new functions by using AAL-Code (this of course is, especially in alpha stage, a risky method because if something does not work, you can not be sure if it is your code or the AAL-Interpreter/Compiler).
 * - You can also help by finding bugs/making suggestions.
 * - If you are not able to program at all (neither C/C++ nor AutoIt/AAL), you can still help by finding undocumented functions/writing mistakes/bad english.
 * - Another possibility to help is by trnaslating english texts into other languages (esepcially into german).
 * - Last but not least, I am also looking for a website and a website layout. I would be able to do it by myself (and I guess I will do the backend by myself) but I really hate HTML/CSS/JS.
 *
 * You see you can contribute on many different ways. So if you really want to help, I am sure you can somehow.
 *
 * \subsection time_taken How long took it to develop the first alpha version?
 * I have started nearly every year for about 5 years another approach to remove the disadvantages of AutoIt.
 * I tried many different ways. I tried programming an AutoIt to C/C++ and then to ASM Compiler.
 * This seemed to be the easiest version. Maybe it also would have been the easiest method, but some other people did this before and my approach stopped in develepoment quite early due to too less experience and knowledge.
 * Then another approach was to write a complete AutoIt to ASM Compiler. This is extremly hard and that is why I discarded this idea after some time.
 * I also tried to write my own AutoIt-Interpreter (based on my other project called Speedfuscator) but the code of my "Speedfuscator" were that horrible and that inconsistent that I decided to start from scratch by using some state of the art methods.
 * I started this project around 1. Juli, 2015. I collected concepts, thought about many things and had a closer look at different "How a Compiler works"-Books (I already flew over some of these books some time ago).
 * Finally, I started programming at 30. August 2015.
 *
 * \section credits Credits and Contributions
 * \subsection marcel_rupprecht Marcel Rupprecht (aka Shadow992)
 * - First Ideas, steps and implementation
 * - Basic AAL-Compiler and AAL-Interpreter
 * - Basic documentation
 * - Active developer
 *
 * \subsection elitepvpers Community of elitepvpers.com
 * - Helped by inspiring
 * - Gave me some hints to improve some algorithms and datasructures
 * - I really like this community, so I had to list this site here. :D
 */

void doTesting()
{
    UnitTest t;
    std::cout<<"Start internal basic tests...\n\n";
    t.doAllTests();
    std::cout<<"Internal basic tests ended\n\n";
}

int main( int argc, const char* argv[] )
{
    // Uncomment this to do some really really basic testing
    //doTesting();
    //return 1;

    #if SHOW_TIMINGS == 1
        auto startAll = std::chrono::steady_clock::now();
    #endif

	std::string toParse=readStringFile("out.aal1.bin");
	Interpreter interpret(toParse);

    #if SHOW_TIMINGS == 1
        auto start = std::chrono::steady_clock::now();
    #endif


	interpret.interpretAllLines();
    //interpret.varManager.globalVars[0]->printVarRecursive();

	#if SHOW_TIMINGS == 1
        auto end = std::chrono::steady_clock::now();
        auto diff = end - start;
        std::cout << "Interpreting: " << std::chrono::duration <double, std::milli> (diff).count() << "ms" << std::endl;
    #endif

    #if SHOW_TIMINGS == 1
        auto endAll = std::chrono::steady_clock::now();
        auto diffAll = endAll - startAll;
        std::cout << "Overall processing: " << std::chrono::duration <double, std::milli> (diffAll).count() << "ms" << std::endl;
    #endif

    //system("PAUSE");

	return 0;
}
