#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "ClassPrototype.h"
#include <vector>
#include <list>
#include <map>
#include <mutex>
#include <Windows.h>
#include "Control.h"
#include "Window.h"
#include <iostream>
#include <thread>
#include <atomic>

class UserInterface
{
	std::map<HWND,Window*> hwndMapping;
	std::map<Window*,std::list<Message>*> messageQueue;
	std::map<HWND,Window*>::iterator currentWindow;
    std::mutex* synchronizer;
    std::mutex* msgSynchronizer;
    bool running=false;
    std::thread* messageThread=nullptr;
    bool windowCreated=false;

    void updateMessages();
    void createRequestedControls();
    void _startAsyncMessageUpdate();

  public:
    UserInterface();
    virtual ~UserInterface();

    std::vector<GuiControlCreationStruct*> requestedControls;
    Window* createWindowAsync(const std::string& title, int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int width=CW_USEDEFAULT, int height=CW_USEDEFAULT, DWORD style=WS_OVERLAPPEDWINDOW, HBRUSH background=GetSysColorBrush(COLOR_3DFACE), DWORD styleEx=WS_EX_STATICEDGE);
    Window* createWindow(GuiControlCreationStruct *controlInfo);
    Window* createWindow(const std::string& title, int x=CW_USEDEFAULT, int y=CW_USEDEFAULT, int width=CW_USEDEFAULT, int height=CW_USEDEFAULT, DWORD style=WS_OVERLAPPEDWINDOW, HBRUSH background=GetSysColorBrush(COLOR_3DFACE), DWORD styleEx=WS_EX_STATICEDGE);
	bool removeWindow(Window*);
	bool showWindow(Window*, bool);
	bool popMessageForWindow(Window*,Message&);
	Window* getNextWindow();
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
    LRESULT CALLBACK realWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
    void startAsyncMessageUpdate();

    HINSTANCE currInstance;
    std::atomic_int msgAvailable;
};

#endif // USERINTERFACE_H
