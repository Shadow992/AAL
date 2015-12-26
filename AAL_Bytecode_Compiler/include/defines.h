#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

// set DEBUG to 0 for no debugging information
// set it to 1 for rough information
// set it to 2 for detailed information
// set it to 3 for more detailed information
// set it to 4 ...
// ...
// set it to 10 for most detailed information
#define DEBUG 0

// if set to 1 to show benchmark times
#define SHOW_TIMINGS 1

// set to 1 to use multi threading
#define USE_MULTITHREADING 1

// if set to 1, windows specific functions are removed (so you can compile it under linux too)
#define ONLY_PARSE 1

// if std::to_string is not defined (or buggy) set this to 1
#define USE_TO_STRING_PATCH 1

#if USE_MULTITHREADING == 1
	#include <thread>
	#include <mutex>
#endif

#if SHOW_TIMINGS == 1
	#include <chrono>
#endif

#if ONLY_PARSE == 0
    #include <windows.h>
#else
    #define LPTSTR std::string
    #define LPCTSTR const char*
#endif

#endif // DEFINES_H_INCLUDED
