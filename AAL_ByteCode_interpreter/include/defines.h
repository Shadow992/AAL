#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

// set DEBUG to 0 for no debugging information
// set it to 1 for rough information
// set it to 2 for detailed information
// set it to 3 for more detailed information
// set it to 4 ...
// ...
// set it to 10 for most detailed information
#define DEBUG 1

// set 1 to show benchmark times
#define SHOW_TIMINGS 1

// set to 1 to use multi threading
#define USE_MULTITHREADING 1

// if set to 1, windows specific functions are removed (so you can compile it under linux too)
#define ONLY_PARSE 0

// if set to 1, you will see all not deleted blocks on program exit
// if set to 2, you will see all allocation/deletion requests for AllocationHelper
// if set to 3, you will only see allocation that was not freed
#define SHOW_MEM_ALLOCATIONS 3

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
    #define LPCTSTR char*
#endif

// Different types that can be used
#define TYPE_DOUBLE 'd'
#define TYPE_LONG 'l'
#define TYPE_STRING 's'
#define TYPE_ARRAY 'a'
#define TYPE_VARIABLE 'v'
#define TYPE_TMP_VARIABLE 't'
#define TYPE_PTR_VARIABLE 'p'
#define TYPE_UNKNOWN 0
#define TYPE_MACRO 'm'
#define TYPE_INTERNAL_CLASS 'i'
#define TYPE_INTERNAL_AAL_CLASS 'I'
#define TYPE_AAL_CLASS 'c'


#if defined(__GNUC__) || defined(__GNUG__)
	/* GNU GCC/G++. --------------------------------------------- */
    #define NO_INLINE __attribute__((noinline))
#elif defined(_MSC_VER)
	/* Microsoft Visual Studio. --------------------------------- */
    #define NO_INLINE __declspec(noinline)
    #pragma comment(lib, "gdi32.lib")
#endif


#endif // DEFINES_H_INCLUDED
