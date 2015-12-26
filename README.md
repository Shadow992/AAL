# AAL
Code of Advanced Automation Language (AAL)

What is AAL?
AAL tries to give the possibility to do basic and also complex automation tasks based on an easy syntax and semantic.
So AAL tries not to be as similar to AutoIt as possible but tries to get the best out of AutoIt and mix it with things that are kind a missing or done worse in AutoIt, e.g.:
Zitat:
- Missing class support (only possible with UDF and then still quite strange to use)
- Quite slow
- Very Easy to decompile and nearly impossible to break existing decompilers
- No possibility to use real multithreading
- Missing or very complicated hardware near programming (e.g. complicated pointer and structs)
- Sometimes strange behaviour and inconsistent keywords (e.g. "While" and "WEnd" but "If" and "EndIf" or "For" and "Next").
- Kind of missing inline ASM
- Missing possibility to compile as DLL

AAL is open-source and I will try to keep it as long open-source as possible.
So you can lear from the project. That is also why I tried to document many things as good as possible.
You should not only learn from this project, but I am also happy to see people helping together to reach the goal of "being a mighty AutoIt alternative".
You do not only have to be familar with coding, I am searching for all kind of help.

What do I have to take care of when using AAL in alpha stage?
The compiler is not able to detect all kind of errors. It only can detect some basic errors in Code (around 60%).
So it is possible to write a code where the compiler is not complaining at all, but the Interpreter still crashes because the compiled Code is not valid. This means whenever you think there is a bug, you have to double check if really every line of your code is correct.
This will be fixed in some feature versions but for alpha stage we have to live with this.

What else to know?
There are 2 versions of AAL, a Debug version and a Realease version.
The Debug-Version will write some information to the file "compiler.debug.txt" and "interpreter.debug.txt".
So whenever you think you found a bug or you want to understand what is going on, use the debug version.
The debug version is of course much slower than the release version, but helps to debug the generated code.
