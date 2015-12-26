#ifndef CLASSPROTOTYPE_H_INCLUDED
#define CLASSPROTOTYPE_H_INCLUDED

#include <string>
#include <Windows.h>
#include <future>

class UserInterface;
class Control;
class Window;

enum GUIType {GUI_WINDOW ,GUI_BUTTON, GUI_INPUTBOX, GUI_LABEL, GUI_CHECKBOX};
enum GUIEvent {ON_CLICK, ON_HOVER};

struct Message
{
	Control* affectedControl;
	GUIEvent currentEvent;
	void* action;
};

struct GuiControlCreationStruct
{
    std::string title;
    std::string guiClass;
	GUIType type;
	int x=CW_USEDEFAULT;
	int y=CW_USEDEFAULT;
	int width=CW_USEDEFAULT;
	int height=CW_USEDEFAULT;
	DWORD style=0;
	HBRUSH background=nullptr;
	DWORD styleEx=0;
	Window* parentWindow=nullptr;
	UserInterface* parentUI=nullptr;
	std::promise<void*> prom;
};
#endif // CLASSPROTOTYPE_H_INCLUDED
