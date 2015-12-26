#include "InternalClassWrapper.h"

InternalClassWrapper::InternalClassWrapper(void* classPtr, InternalClassType t)
{
    classPointer=classPtr;
    type=t;
}

InternalClassWrapper::~InternalClassWrapper()
{
    switch(type)
    {
    case INTERNAL_IMAGE:
        delete (Image*)classPointer;
        break;
    case INTERNAL_CONTROL:
        // You MUST NOT delete controls/windows
        //delete (Control*)classPointer;
        break;
    case INTERNAL_WINDOW:
        // You MUST NOT delete controls/windows
        //delete (Window*)classPointer;
        break;
    }
}
