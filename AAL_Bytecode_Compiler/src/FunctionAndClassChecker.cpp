#include "FunctionAndClassChecker.h"

FunctionAndClassChecker::FunctionAndClassChecker()
{
    knownFunctions["consolewrite"]=true;
    knownFunctions["consoleread"]=true;
    knownFunctions["timer"]=true;
    knownFunctions["goto"]=true;
	knownFunctions["ubound"]=true;
	knownFunctions["sizeof"]=true;
    knownFunctions["charat"]=true;
    knownFunctions["sleep"]=true;

    knownFunctions["createwindow"]=true;
    knownFunctions["createbutton"]=true;
    knownFunctions["createlabel"]=true;
    knownFunctions["createcheckbox"]=true;
    knownFunctions["createinputbox"]=true;
    knownFunctions["setonevent"]=true;
    knownFunctions["readtext"]=true;
    knownFunctions["settext"]=true;
    knownFunctions["ischecked"]=true;

    knownFunctions["strlen"]=true;
    knownFunctions["strsubstr"]=true;
    knownFunctions["strcontains"]=true;

    knownFunctions["clickleftbackground"]=true;
    knownFunctions["charat"]=true;
    knownFunctions["loadimagefrombmp"]=true;
    knownFunctions["capturewindow"]=true;
    knownFunctions["getpixelcolor"]=true;
    knownFunctions["setpixelcolor"]=true;
    knownFunctions["searchforpixel"]=true;
    knownFunctions["searchforimage"]=true;
    knownFunctions["searchforsimilarimage"]=true;
    knownFunctions["searchforpixelnexttoposition"]=true;
    knownFunctions["searchforimagenexttoposition"]=true;
    knownFunctions["getwindowhandle"]=true;
    knownFunctions["saveimage"]=true;

	knownFunctions["_debug_showarrayinfo"]=true;
	knownFunctions["_debug_dump"]=true;


    knownClasses["stack"]=true;
}

FunctionAndClassChecker::~FunctionAndClassChecker()
{
    //dtor
}

void FunctionAndClassChecker::addClass(const std::string& str)
{
    std::string lowerStr=str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    knownClasses[lowerStr]=true;
}

void FunctionAndClassChecker::addFunction(const std::string& str)
{
    std::string lowerStr=str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    knownFunctions[lowerStr]=true;
}


bool FunctionAndClassChecker::isAvailable(const std::string& str)
{
    std::string lowerStr=str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    if(knownFunctions.find(lowerStr) != knownFunctions.end() || knownClasses.find(lowerStr) != knownClasses.end())
    {
        return true;
    }
    return false;
}

bool FunctionAndClassChecker::isFunctionAvailable(const std::string& str)
{
    std::string lowerStr=str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    if(knownFunctions.find(lowerStr) != knownFunctions.end())
    {
        return true;
    }
    return false;
}

bool FunctionAndClassChecker::isClassAvailable(const std::string& str)
{
    std::string lowerStr=str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    if(knownClasses.find(lowerStr) != knownClasses.end())
    {
        return true;
    }
    return false;
}
