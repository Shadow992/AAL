#ifndef INTERNALCLASSWRAPPER_H
#define INTERNALCLASSWRAPPER_H

#include "Image.h"

enum InternalClassType{INTERNAL_IMAGE, INTERNAL_WINDOW, INTERNAL_CONTROL};

class InternalClassWrapper
{
    public:
        InternalClassWrapper(void* classPtr, InternalClassType t);
        virtual ~InternalClassWrapper();

        void* classPointer;
        InternalClassType type;
    protected:
    private:
};

#endif // INTERNALCLASSWRAPPER_H
