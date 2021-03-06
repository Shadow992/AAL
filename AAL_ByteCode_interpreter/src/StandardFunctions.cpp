#include "StandardFunctions.h"

StandardFunctions::StandardFunctions()
{
    // Adding functions to functionMap, so we can call them easily later on
    // Notice that functions MUST be lowercase (to enable case insensitive searching)
    functionMap["consoleread"]=consoleRead;
    functionMap["consolewrite"]=consoleWrite;
    functionMap["sizeof"]=Ubound;
    functionMap["ubound"]=Ubound;
    functionMap["time"]=timer;
    functionMap["timer"]=timer;
    functionMap["charat"]=charAt;
    functionMap["sleep"]=aalSleep;

    functionMap["strlen"]=stringLength;
    functionMap["strsubstr"]=stringSubstring;
    functionMap["strcontains"]=stringContains;

    functionMap["createwindow"]=guiCreateWindow;
    functionMap["createbutton"]=guiCreateButton;
    functionMap["createlabel"]=guiCreateLabel;
    functionMap["createcheckbox"]=guiCreateCheckbox;
    functionMap["createinputbox"]=guiCreateInputbox;
    functionMap["setonevent"]=guiSetOnEvent;

    functionMap["readtext"]=guiReadText;
    functionMap["settext"]=guiSetText;
    functionMap["ischecked"]=guiIsChecked;

    functionMap["getwindowhandle"]=getWindowHandle;
    functionMap["clickleftbackground"]=clickLeftBackground;
    functionMap["loadimagefrombmp"]=loadImageFromBMP;
    functionMap["capturewindow"]=captureWindow;
    functionMap["saveimage"]=saveImage;
    functionMap["getpixelcolor"]=getPixelColor;
    functionMap["setpixelcolor"]=setPixelColor;
    functionMap["searchforpixel"]=searchForPixel;
    functionMap["searchforimage"]=searchForImage;
    functionMap["searchforsimilarimage"]=searchForSimilarImage;
    functionMap["searchforpixelnexttoposition"]=searchForPixelNextToPosition;
    functionMap["searchforimagenexttoposition"]=searchForImageNextToPosition;

    functionMap["_debug_dump"]=debug_PrintVariableRecursive;
    functionMap["_debug_showarrayinfo"]=debug_ShowArrayInfo;
}

StandardFunctions::~StandardFunctions()
{
    //dtor
}

/**
*
* \brief This method executes a given standard AAL-Function.
*
* \param functionName name of function to execute
* \param parameterVar argument-list to pass to function
* \param returnVar variable to save return value of function to
*
* \return true if standard function exists, otherwise false
*/
bool StandardFunctions::executeFunction(std::string functionName,AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    auto it=functionMap.find(functionName);
    if(it!=functionMap.end())
    {
        // found function, call it now
        it->second(parameterVar,returnVar,currInterpreter);
    }
    else
    {
        // function not found
        return false;
    }
    return true;
}

AalVariable* getRealParamVar(AalVariable* parameterVar, int i, int& realIndex)
{
    if(parameterVar->getType(i)==TYPE_VARIABLE)
    {
        realIndex=0;
        return parameterVar->getVariablePointer(i);
    }
    realIndex=i;
    return parameterVar;
}

int aalSleep(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    // title,x,y,width,height,style,styleEx
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    int msToSleep=tmpParamVar->getLongValue(realIdx);

    Sleep(msToSleep);

    return 0;
}

int guiCreateWindow(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    // title,x,y,width,height,style,styleEx
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::string* title=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    int x=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    int y=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,3,realIdx);
    int width=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,4,realIdx);
    int height=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,5,realIdx);
    int style=tmpParamVar->getLongValue(realIdx);

    Window* w=currInterpreter->ui->createWindowAsync(*title,x,y,width,height,style);

    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);
    ptr->reset(new InternalClassWrapper(w,INTERNAL_WINDOW));

    return 0;
}

int guiCreateButton(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    // window,title,x,y,width,height,style,styleEx
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* title=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    int x=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,3,realIdx);
    int y=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,4,realIdx);
    int width=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,5,realIdx);
    int height=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,6,realIdx);
    int style=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,7,realIdx);
    int styleEx=tmpParamVar->getLongValue(realIdx);

    InternalClassWrapper* wrapper=internalPtr->get();
    Window* w=(Window*)wrapper->classPointer;
    Control* c=w->createButtonAsync(*title,x,y,width,height,style,styleEx);

    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);
    ptr->reset(new InternalClassWrapper(c,INTERNAL_CONTROL));

    return 0;
}

int guiCreateCheckbox(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    // window,title,x,y,width,height,style,styleEx
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* title=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    int x=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,3,realIdx);
    int y=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,4,realIdx);
    int width=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,5,realIdx);
    int height=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,6,realIdx);
    int style=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,7,realIdx);
    int styleEx=tmpParamVar->getLongValue(realIdx);

    InternalClassWrapper* wrapper=internalPtr->get();
    Window* w=(Window*)wrapper->classPointer;
    Control* c=w->createCheckboxAsync(*title,x,y,width,height,style,styleEx);

    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);
    ptr->reset(new InternalClassWrapper(c,INTERNAL_CONTROL));

    return 0;
}

int guiCreateLabel(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    // window,title,x,y,width,height,style,styleEx
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* title=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    int x=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,3,realIdx);
    int y=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,4,realIdx);
    int width=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,5,realIdx);
    int height=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,6,realIdx);
    int style=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,7,realIdx);
    int styleEx=tmpParamVar->getLongValue(realIdx);

    InternalClassWrapper* wrapper=internalPtr->get();
    Window* w=(Window*)wrapper->classPointer;
    Control* c=w->createLabelAsync(*title,x,y,width,height,style,styleEx);

    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);
    ptr->reset(new InternalClassWrapper(c,INTERNAL_CONTROL));

    return 0;
}

int guiCreateInputbox(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    // window,title,x,y,width,height,style,styleEx
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* title=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    int x=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,3,realIdx);
    int y=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,4,realIdx);
    int width=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,5,realIdx);
    int height=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,6,realIdx);
    int style=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,7,realIdx);
    int styleEx=tmpParamVar->getLongValue(realIdx);

    InternalClassWrapper* wrapper=internalPtr->get();
    Window* w=(Window*)wrapper->classPointer;
    Control* c=w->createInputBoxAsync(*title,x,y,width,height,style,styleEx);

    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);
    ptr->reset(new InternalClassWrapper(c,INTERNAL_CONTROL));

    return 0;
}

int guiReadText(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Control* c=(Control*)wrapper->classPointer;

    returnVar->setValue(c->getText(),TYPE_STRING);

    return 0;
}


int guiSetText(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Control* c=(Control*)wrapper->classPointer;

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* newTitle=tmpParamVar->getStringPointer(realIdx);

    c->setText(*newTitle);

    return 0;
}


int guiIsChecked(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Control* c=(Control*)wrapper->classPointer;

    returnVar->setValue((long long)c->isChecked(),TYPE_LONG);

    return 0;
}

int guiSetOnEvent(AalVariable* parameterVar, AalVariable* returnVar,Interpreter* currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* internalPtr=tmpParamVar->getSharedPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Control* c=(Control*)wrapper->classPointer;

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* onEvent=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    std::string* onEventFunction=tmpParamVar->getStringPointer(realIdx);

    std::transform(onEvent->begin(),onEvent->end(),onEvent->begin(),tolower);
    if(*onEvent=="onclick")
    {
       c->addOnEventAction(ON_CLICK,new std::string(*onEventFunction));
    }

    return 0;
}

void clickMouse(HWND hwnd, int x, int y)
{
    SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM((short)x, (short)y));
    Sleep(5);
    SendMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM((short)x, (short)y));
}

int stringContains(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::string* str=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string* str2=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    long long pos=tmpParamVar->getLongValue(realIdx);

    std::size_t foundPos=str->find(*str2,pos);
    if(foundPos==std::string::npos)
    {
        returnVar->setValue(-1LL,TYPE_LONG);
    }
    else
    {
        returnVar->setValue((long long) foundPos,TYPE_LONG);
    }

    return 0;
}

int stringSubstring(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::string* str=tmpParamVar->getStringPointer(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    long long length=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    long long pos=tmpParamVar->getLongValue(realIdx);

    returnVar->setValue(str->substr(pos,length),TYPE_STRING);

    return 0;
}

int stringLength(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::string* str=tmpParamVar->getStringPointer(realIdx);

    returnVar->setValue((long long) str->length(),TYPE_LONG);

    return 0;
}

int clickLeftBackground(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    // Pixel* searchPixelNextToPosition(int x, int y, unsigned char red,unsigned char green,unsigned char blue, unsigned char variance);
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    int x=tmpParamVar->getLongValue(realIdx);
    tmpParamVar=getRealParamVar(parameterVar,2,realIdx);
    int y=tmpParamVar->getLongValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    HWND arg0=(HWND)tmpParamVar->getLongValue(realIdx);
    if(!IsWindow(arg0))
    {
        arg0=nullptr;
    }

    clickMouse(arg0,x,y);

    return 0;
}

int searchForPixelNextToPosition(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    // Pixel* searchPixelNextToPosition(int x, int y, unsigned char red,unsigned char green,unsigned char blue, unsigned char variance);
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }

    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* img=(Image*)wrapper->classPointer;

    long long args[6];
    for(unsigned int i=1; i<parameterVar->getValueSize() && i < 7; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-1] = tmpParamVar->getLongValue(realIdx);
    }
    Pixel* tmpPixel=img->searchPixelNextToPosition(args[0],args[1],args[2],args[3],args[4],args[5]);

    AalVariable* tmpVar=allocator.allocateAalVar();
    tmpVar->appendDimension(2);

    if(tmpPixel!=nullptr)
    {
        tmpVar->getVariablePointer(0)->setValue((long long)tmpPixel->x,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue((long long)tmpPixel->y,TYPE_LONG,0);
    }
    else
    {
        tmpVar->getVariablePointer(0)->setValue(-1LL,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue(-1LL,TYPE_LONG,0);
    }

    returnVar->setValue(tmpVar);

    allocator.recycleAalVar(tmpVar);

    return 0;
}

int saveImage(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }
    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* img=(Image*)wrapper->classPointer;

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    img->saveAsBMP(tmpParamVar->getStringValue(realIdx));

    return 0;
}

int searchForImageNextToPosition(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    // searchForImageNextToPosition(int x, int y, const Image& searchImage, unsigned char variance);
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }
    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* imgSrc=(Image*)wrapper->classPointer;

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 2;
    }
    internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    wrapper=internalPtr->get();
    Image* imgSearch=(Image*)wrapper->classPointer;


    long long args[3];
    for(unsigned int i=2; i<parameterVar->getValueSize() && i < 5; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-2] = tmpParamVar->getLongValue(realIdx);
    }

    Pixel* tmpPixel=imgSrc->searchForImageNextToPosition(args[0],args[1],*imgSearch,args[2]);

    AalVariable* tmpVar=allocator.allocateAalVar();
    tmpVar->appendDimension(2);

    if(tmpPixel!=nullptr)
    {
        tmpVar->getVariablePointer(0)->setValue((long long)tmpPixel->x,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue((long long)tmpPixel->y,TYPE_LONG,0);
    }
    else
    {
        tmpVar->getVariablePointer(0)->setValue(-1LL,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue(-1LL,TYPE_LONG,0);
    }

    returnVar->setValue(tmpVar);

    allocator.recycleAalVar(tmpVar);

    return 0;
}

int searchForSimilarImage(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    // float findSimilarImageHillClimbing(int startX, int startY,int w, int h,Image& checkImage, int accuracy , float overlap, int &bestX, int &bestY);
    // AAL: searchForSimilarImage(img1,img2, startX, startY, w,  h, accuracy,overlap )
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }
    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* imgSrc=(Image*)wrapper->classPointer;

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 2;
    }

    internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    wrapper=internalPtr->get();
    Image* imgSearch=(Image*)wrapper->classPointer;

    long long args[5];
    for(unsigned int i=2; i<parameterVar->getValueSize() && i < 7; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-2] = tmpParamVar->getLongValue(realIdx);
    }

    tmpParamVar=getRealParamVar(parameterVar,7,realIdx);
    float overlap = tmpParamVar->getDoubleValue(realIdx);
    int bestX;
    int bestY;
    float bestFitness=imgSrc->findSimilarImageHillClimbing(args[0],args[1],args[2],args[3],*imgSearch,args[4],overlap,bestX,bestY);

    AalVariable* tmpVar=allocator.allocateAalVar();
    tmpVar->appendDimension(3);

    tmpVar->getVariablePointer(0)->setValue((long long)bestX,TYPE_LONG,0);
    tmpVar->getVariablePointer(1)->setValue((long long)bestY,TYPE_LONG,0);
    tmpVar->getVariablePointer(2)->setValue((double)bestFitness,TYPE_DOUBLE,0);

    returnVar->setValue(tmpVar);

    allocator.recycleAalVar(tmpVar);

    return 0;
}

int searchForImage(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    // Pixel* searchForImage(int startX, int startY, const Image& searchImage, unsigned char variance);
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }
    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* imgSrc=(Image*)wrapper->classPointer;

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 2;
    }
    internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    wrapper=internalPtr->get();
    Image* imgSearch=(Image*)wrapper->classPointer;

    long long args[3];
    for(unsigned int i=2; i<parameterVar->getValueSize() && i < 5; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-1] = tmpParamVar->getLongValue(realIdx);
    }
    Pixel* tmpPixel=imgSrc->searchForImage(args[0],args[1],*imgSearch,args[2]);

    AalVariable* tmpVar=allocator.allocateAalVar();
    tmpVar->appendDimension(2);

    if(tmpPixel!=nullptr)
    {
        tmpVar->getVariablePointer(0)->setValue((long long)tmpPixel->x,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue((long long)tmpPixel->y,TYPE_LONG,0);
    }
    else
    {
        tmpVar->getVariablePointer(0)->setValue(-1LL,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue(-1LL,TYPE_LONG,0);
    }

    returnVar->setValue(tmpVar);

    allocator.recycleAalVar(tmpVar);

    return 0;
}

int searchForPixel(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }

    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* img=(Image*)wrapper->classPointer;

    long long args[6];
    for(unsigned int i=1; i<parameterVar->getValueSize() && i < 7; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-1] = tmpParamVar->getLongValue(realIdx);
    }
    Pixel* tmpPixel=img->searchForPixel(args[0],args[1],args[2],args[3],args[4],args[5]);

    AalVariable* tmpVar=allocator.allocateAalVar();
    tmpVar->appendDimension(2);

    if(tmpPixel!=nullptr)
    {
        tmpVar->getVariablePointer(0)->setValue((long long)tmpPixel->x,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue((long long)tmpPixel->y,TYPE_LONG,0);
    }
    else
    {
        tmpVar->getVariablePointer(0)->setValue(-1LL,TYPE_LONG,0);
        tmpVar->getVariablePointer(1)->setValue(-1LL,TYPE_LONG,0);
    }

    returnVar->setValue(tmpVar);

    allocator.recycleAalVar(tmpVar);

    return 0;
}

int setPixelColor(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }

    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* img=(Image*)wrapper->classPointer;

    long long args[5];
    for(unsigned int i=1; i<parameterVar->getValueSize() && i < 6; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-1] = tmpParamVar->getLongValue(realIdx);
    }

    img->setPixelColor(args[0],args[1],args[2],args[3],args[4]);

    return 0;
}

int getPixelColor(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    if(tmpParamVar->getType(realIdx)!=TYPE_INTERNAL_CLASS)
    {
        return 1;
    }

    std::shared_ptr<InternalClassWrapper>* internalPtr=(std::shared_ptr<InternalClassWrapper>*)tmpParamVar->getVoidPointer(realIdx);
    InternalClassWrapper* wrapper=internalPtr->get();
    Image* img=(Image*)wrapper->classPointer;

    long long args[2];
    for(unsigned int i=1; i<parameterVar->getValueSize() && i < 3; i++)
    {
        tmpParamVar=getRealParamVar(parameterVar,i,realIdx);
        args[i-1] = tmpParamVar->getLongValue(realIdx);
    }
    Pixel* tmpPixel=img->getPixel(args[0],args[1]);

    AalVariable* tmpVar=allocator.allocateAalVar();
    tmpVar->appendDimension(3);

    tmpVar->getVariablePointer(0)->setValue((long long)tmpPixel->color.red,TYPE_LONG,0);
    tmpVar->getVariablePointer(1)->setValue((long long)tmpPixel->color.green,TYPE_LONG,0);
    tmpVar->getVariablePointer(2)->setValue((long long)tmpPixel->color.blue,TYPE_LONG,0);

    returnVar->setValue(tmpVar);

    allocator.recycleAalVar(tmpVar);

    return 0;
}

int loadImageFromBMP(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);

    Image* imgPtr=new Image;
    ptr->reset(new InternalClassWrapper(imgPtr,INTERNAL_IMAGE));
    imgPtr->readAndParseBMPFile(tmpParamVar->getStringValue(realIdx));

    return 0;
}

int getWindowHandle(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    int realIdx;
    AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
    std::string arg0=tmpParamVar->getStringValue(realIdx);

    tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
    std::string arg1=tmpParamVar->getStringValue(realIdx);

    HWND hWnd = FindWindowEx(nullptr,nullptr,(LPCSTR)arg0.c_str(),(LPCSTR)arg1.c_str());

    returnVar->setValue((long long)hWnd,TYPE_LONG);

    return 0;
}

int captureWindow(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    HWND arg0;
    if(parameterVar->getValueSize()==0)
    {
        arg0=nullptr;
    }
    else
    {
        int realIdx;
        AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
        arg0=(HWND)tmpParamVar->getLongValue(realIdx);
        if(arg0==nullptr || !IsWindow(arg0))
        {
            arg0=nullptr;
        }
    }

    returnVar->printVarRecursive();
    std::shared_ptr<InternalClassWrapper>* ptr = returnVar->getSharedPointer(0);
    Image* imgPtr=new Image;
    ptr->reset(new InternalClassWrapper(imgPtr,INTERNAL_IMAGE));

    imgPtr->captureWindowFromHandle(arg0);

    return 0;
}


int charAt(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    if(parameterVar->getValueSize()>1)
    {
        int realIdx;
        AalVariable* tmpParamVar=getRealParamVar(parameterVar,0,realIdx);
        std::string* tmpStr=tmpParamVar->getStringPointer(realIdx);

        tmpParamVar=getRealParamVar(parameterVar,1,realIdx);
        long long pos=tmpParamVar->getLongValue(realIdx);
        if(tmpStr->size()>pos && pos>0)
        {
            returnVar->setValue(tmpStr[pos],TYPE_STRING);
        }
        else
        {
            returnVar->setValue(-1LL,TYPE_LONG);
            return 1;
        }
    }
    return 0;
}

int debug_PrintVariableRecursive(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    switch(parameterVar->getType(0))
    {
    case TYPE_VARIABLE:
    {
        AalVariable* arg0=parameterVar->getVariablePointer(0);
        arg0->printVarRecursive();
        break;
    }
    default:
        return 1;
    }

    return 0;
}

int debug_ShowArrayInfo(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    switch(parameterVar->getType(0))
    {
    case TYPE_VARIABLE:
    {
        AalVariable* arg0=parameterVar->getVariablePointer(0);
        if(arg0->arrayInfo==nullptr)
        {
            std::cout<<"arrayinfo = nullptr\n";
        }
        else
        {
            std::cout<<"arrayinfo = {\noverallArraySize="<<arg0->arrayInfo->overallArraySize
                     <<",\n usedAalVarChunks_size="<< arg0->arrayInfo->usedAalVarChunks.size()
                     <<",\n usedValueChunks_size="<< arg0->arrayInfo->usedValueChunks.size()
                     <<",\n usedValueTypeChunks_size="<< arg0->arrayInfo->usedValueTypeChunks.size() <<" \n}\n";
        }
        break;
    }
    default:
        return 1;
    }


    return 0;
}

int Ubound(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    switch(parameterVar->getType(0))
    {
    case TYPE_VARIABLE:
    {
        AalVariable* arg0=parameterVar->getVariablePointer(0);
        returnVar->setValue((long long)arg0->getValueSize(),TYPE_LONG);
        break;
    }
    default:
        returnVar->setValue(0LL,TYPE_LONG);
        return 1;
    }
    return 0;
}

int consoleRead(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    std::string readLine;
    std::cin >> readLine;
    returnVar->setValue(readLine,TYPE_STRING);
    return 0;
}

int consoleWrite(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    switch(parameterVar->getType(0))
    {
    case TYPE_STRING:
        std::cout << parameterVar->getStringValue(0);
        break;
    case TYPE_LONG:
        std::cout << parameterVar->getLongValue(0);
        break;
    case TYPE_DOUBLE:
        std::cout << parameterVar->getDoubleValue(0);
        break;
    case TYPE_VARIABLE:
        std::cout << parameterVar->getVariablePointer(0)->getStringValue(0);
        break;
    default:
        break;
    }
    return 0;
}

int timer(AalVariable* parameterVar, AalVariable* returnVar,Interpreter * currInterpreter)
{
    if(parameterVar->getArgumentCount()==1 && parameterVar->getLongValue(0)==1)
    {
        returnVar->setValue((long long)time(nullptr),TYPE_LONG);
    }
    else
    {
        double ms = (std::chrono::duration <double, std::milli> (std::chrono::system_clock::now().time_since_epoch())).count();
        returnVar->setValue(ms,TYPE_DOUBLE);
    }

    return 0;
}
