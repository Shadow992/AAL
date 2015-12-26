#ifndef WINDOW_H
#define WINDOW_H

#include <vector>
#include <map>
#include <Windows.h>
#include "Control.h"
#include "UserInterface.h"

class Window
{

    std::map<int, Control*> controls;
	UserInterface* parentUI;
	int lastUsedId;
	std::mutex* synchronizer;
	std::mutex* msgSynchronizer;

  public:
    Window();
    Window(HWND hwnd,UserInterface* parent,std::mutex* syn, std::mutex* msgSyn);
    virtual ~Window();

    Control* createControl(GuiControlCreationStruct* controlInfo);
    Control* createControl(GUIType type, const std::string& title, int x, int y, int width=47, int height=25, DWORD style=(WS_CHILD|WS_VISIBLE), DWORD styleEx=0,const std::string& controlClass="");
	Control* createButton(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);
	Control* createInputBox(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);
	Control* createLabel(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);
    Control* createCheckbox(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);

    Control* createCheckboxAsync(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);
	Control* createControlAsync(GUIType type, const std::string& title, int x, int y, int width=47, int height=25, DWORD style=(WS_CHILD|WS_VISIBLE), DWORD styleEx=0,const std::string& controlClass="");
    Control* createButtonAsync(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);
	Control* createInputBoxAsync(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);
	Control* createLabelAsync(const std::string& title, int x, int y, int width=47, int height=25, DWORD style=WS_CHILD|WS_VISIBLE, DWORD styleEx=0);

    Control* getControlById(int);
	bool removeControl(Control*);
	bool showControl(Control*, bool);
	bool popLatestMessage(Message&);

	HWND currentHandle;
};

#endif // WINDOW_H
