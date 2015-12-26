#include "Window.h"

Window::Window()
{
    //ctor
}

Window::Window(HWND hwnd,UserInterface* parent,std::mutex* syn, std::mutex* msgSyn)
{
    currentHandle=hwnd;
    parentUI=parent;
    synchronizer=syn;
    msgSynchronizer=msgSyn;
}

Window::~Window()
{

}

Control* Window::createControlAsync(GUIType type, const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx, const std::string& controlClass)
{
    GuiControlCreationStruct* windowStruct=new GuiControlCreationStruct;
    windowStruct->type=type;
    windowStruct->title=title;
    windowStruct->x=x;
    windowStruct->y=y;
    windowStruct->width=width;
    windowStruct->height=height;
    windowStruct->style=style;
    windowStruct->styleEx=styleEx;
    windowStruct->parentWindow=this;
    windowStruct->guiClass=controlClass;

    synchronizer->lock();
    parentUI->requestedControls.push_back(windowStruct);
    synchronizer->unlock();

    std::future<void*> fut = windowStruct->prom.get_future();
    Control* tmp=(Control*)fut.get();
    delete windowStruct;
    return tmp;
}

Control* Window::createControl(GuiControlCreationStruct* controlInfo)
{
    if(controlInfo->style==0)
    {
         controlInfo->style=(WS_CHILD|WS_VISIBLE);
    }

    return createControl(controlInfo->type,controlInfo->title, controlInfo->x, controlInfo->y, controlInfo->width, controlInfo->height, controlInfo->style, controlInfo->styleEx, controlInfo->guiClass);
}

Control* Window::createControl(GUIType type, const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx, const std::string& controlClass)
{
    HWND control;
    switch(type)
    {
    case GUI_BUTTON:
        control = CreateWindowEx(styleEx, "BUTTON", (LPCSTR) title.c_str(), style, x, y, width, height, currentHandle, (HMENU) lastUsedId, parentUI->currInstance, 0);
        break;
    case GUI_INPUTBOX:
        control = CreateWindowEx(styleEx, "EDIT",  (LPCSTR)title.c_str(), WS_BORDER | style, x, y, width, height, currentHandle, (HMENU) lastUsedId, parentUI->currInstance, 0);
        break;
    case GUI_LABEL:
        control = CreateWindowEx(styleEx, "STATIC", (LPCSTR)title.c_str(), style, x, y, width, height, currentHandle, (HMENU) lastUsedId, parentUI->currInstance, 0);
        break;
    case GUI_CHECKBOX:
        control = CreateWindowEx(styleEx, "BUTTON", (LPCSTR)title.c_str(), BS_CHECKBOX | style, x, y, width, height, currentHandle, (HMENU) lastUsedId, parentUI->currInstance, 0);
        break;
    default:
        control = CreateWindowEx(styleEx, (LPCSTR)controlClass.c_str(), (LPCSTR)title.c_str(), style, x, y, width, height, currentHandle, (HMENU) lastUsedId, parentUI->currInstance, 0);
    }
    Control* createdControl=new Control(control,this,synchronizer,msgSynchronizer,type);
    controls[lastUsedId]=createdControl;
    lastUsedId++;
    return createdControl;

}

Control* Window::createCheckboxAsync(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControlAsync(GUI_CHECKBOX,  title,  x,  y,  width,  height,  style,  styleEx);
}

Control* Window::createButtonAsync(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControlAsync(GUI_BUTTON,  title,  x,  y,  width,  height,  style,  styleEx);
}

Control* Window::createInputBoxAsync(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControlAsync(GUI_INPUTBOX,  title,  x,  y,  width,  height,  style,  styleEx);
}

Control* Window::createLabelAsync(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControlAsync(GUI_LABEL,  title,  x,  y,  width,  height,  style,  styleEx);
}

Control* Window::createButton(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControl(GUI_BUTTON,title,x,y,width,height,style,styleEx);
}

Control* Window::createCheckbox(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControl(GUI_CHECKBOX,title,x,y,width,height,style,styleEx);
}

Control* Window::createInputBox(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControl(GUI_INPUTBOX,title,x,y,width,height,style,styleEx);
}

Control* Window::createLabel(const std::string& title, int x, int y, int width, int height, DWORD style, DWORD styleEx)
{
    return createControl(GUI_LABEL,title,x,y,width,height,style,styleEx);
}

bool Window::removeControl(Control*)
{

}

bool Window::showControl(Control*, bool)
{

}

bool Window::popLatestMessage(Message& msg)
{
    return parentUI->popMessageForWindow(this,msg);
}

Control* Window::getControlById(int id)
{
    auto it=controls.find(id);
    if(it==controls.end())
    {
        return nullptr;
    }
    return it->second;
}
