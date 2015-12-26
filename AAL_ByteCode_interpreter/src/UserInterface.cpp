#include "UserInterface.h"

UserInterface::UserInterface()
{
    currInstance=GetModuleHandle(NULL);
    synchronizer=new std::mutex;
    msgSynchronizer=new std::mutex;
    msgAvailable=0;
}

UserInterface::~UserInterface()
{
    running=false;
    if(messageThread!=nullptr)
    {
        messageThread->join();
    }


    delete synchronizer;
    delete messageThread;
    delete msgSynchronizer;

    // delete created windows/controls
}

Window* UserInterface::createWindow(GuiControlCreationStruct* windowInfo)
{
    if(windowInfo->style==0)
    {
         windowInfo->style=WS_OVERLAPPEDWINDOW;
    }

    if(windowInfo->styleEx==0)
    {
         windowInfo->styleEx=WS_EX_STATICEDGE;
    }

    if(windowInfo->background==nullptr)
    {
         windowInfo->background=GetSysColorBrush(COLOR_3DFACE);
    }

    return createWindow(windowInfo->title, windowInfo->x, windowInfo->y, windowInfo->width, windowInfo->height, windowInfo->style, windowInfo->background, windowInfo->styleEx);
}


Window* UserInterface::createWindowAsync(const std::string& title, int x, int y, int width, int height, DWORD style, HBRUSH background , DWORD styleEx)
{
    GuiControlCreationStruct* windowStruct=new GuiControlCreationStruct;
    windowStruct->type=GUI_WINDOW;
    windowStruct->title=title;
    windowStruct->x=x;
    windowStruct->y=y;
    windowStruct->width=width;
    windowStruct->height=height;
    windowStruct->style=style;
    windowStruct->styleEx=styleEx;
    windowStruct->parentUI=this;

    synchronizer->lock();
    requestedControls.push_back(windowStruct);
    synchronizer->unlock();

    std::future<void*> fut = windowStruct->prom.get_future();
    Window* tmp=(Window*)fut.get();
    delete windowStruct;
    return tmp;
}

Window* UserInterface::createWindow(const std::string& title, int x, int y, int width, int height, DWORD style, HBRUSH background , DWORD styleEx)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = UserInterface::wndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = currInstance;
    wcex.hIcon          = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = background;
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = title.c_str();
    wcex.hIconSm        = LoadIcon(currInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        return nullptr;
    }

    // The parameters to CreateWindow explained:
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindow(
                    title.c_str(),
                    title.c_str(),
                    style,
                    x, y,
                    width, height,
                    NULL,
                    NULL,
                    currInstance,
                    NULL
                );

    if (!hWnd)
    {
        return nullptr;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,SW_SHOWNORMAL);
    UpdateWindow(hWnd);

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (long)this);


    Window* createdWin=new Window(hWnd,this,synchronizer,msgSynchronizer);

    msgSynchronizer->lock();
    messageQueue[createdWin]=new std::list<Message>;
    hwndMapping[hWnd]=createdWin;
    currentWindow=hwndMapping.begin();
    windowCreated=true;
    msgSynchronizer->unlock();

    return createdWin;
}

bool UserInterface::removeWindow(Window*)
{

}

bool UserInterface::showWindow(Window*, bool)
{

}

bool UserInterface::popMessageForWindow(Window* win,Message& msg)
{
    if(windowCreated==false)
        return false;

    msgSynchronizer->lock();
    auto it=messageQueue.find(win);
    if(it==messageQueue.end())
    {
        msgSynchronizer->unlock();
        return false;
    }

    std::list<Message>* msgQueue=it->second;
    if(msgQueue->empty())
    {
        msgSynchronizer->unlock();
        return false;
    }

    msg=msgQueue->front();
    msgQueue->pop_front();
    msgAvailable--;
    msgSynchronizer->unlock();
    return true;
}

Window* UserInterface::getNextWindow()
{
    if(windowCreated==false)
        return nullptr;

    msgSynchronizer->lock();
    if(currentWindow!=hwndMapping.end())
    {
        Window* curr=currentWindow->second;
        ++currentWindow;
        msgSynchronizer->unlock();
        return curr;
    }
    currentWindow=hwndMapping.begin();
    msgSynchronizer->unlock();
    return nullptr;
}

void UserInterface::startAsyncMessageUpdate()
{
    running=true;
    if(messageThread==nullptr)
    {
        messageThread=new std::thread(&UserInterface::_startAsyncMessageUpdate,this);
    }
}

void UserInterface::_startAsyncMessageUpdate()
{
    while(running)
    {
        updateMessages();
        createRequestedControls();
        Sleep(10);
    }
}

void UserInterface::createRequestedControls()
{
    synchronizer->lock();
    for(unsigned int i=0;i<requestedControls.size();i++)
    {
        if(requestedControls[i]->type==GUI_WINDOW)
        {
            requestedControls[i]->prom.set_value(requestedControls[i]->parentUI->createWindow(requestedControls[i]));
        }
        else
        {
            requestedControls[i]->prom.set_value(requestedControls[i]->parentWindow->createControl(requestedControls[i]));
        }
    }
    requestedControls.clear();
    synchronizer->unlock();
}

void UserInterface::updateMessages()
{
    MSG msg;
    for(int i=0; i<100 && PeekMessage( &msg, nullptr, 0, 0 ,1)!=0; i++)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK UserInterface::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    UserInterface* me = (UserInterface*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (me)
    {
        return me->realWndProc(hwnd, msg, wParam, lParam);
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK UserInterface::realWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_COMMAND:
    {
        auto it=hwndMapping.find(hWnd);
        if(it!=hwndMapping.end())
        {
            Window* currentWindow=it->second;
            if((HWND)lParam)
            {
                int controlId=LOWORD(wParam);
                Control* control=currentWindow->getControlById(controlId);
                if(control==nullptr)
                {
                    break;
                }

                msgSynchronizer->lock();
                auto it=messageQueue.find(currentWindow);
                if(it==messageQueue.end())
                {
                    break;
                }
                msgAvailable++;
                std::list<Message>* msgQueue=it->second;
                if(HIWORD(wParam) == BN_CLICKED)
                {
                    msgQueue->push_back({control,ON_CLICK,control->getOnEventAction(ON_CLICK)});
                }
                msgSynchronizer->unlock();
                if(control->type==GUI_CHECKBOX)
                {
                    if(control->isChecked())
                    {
                        SendMessage(control->currentHandle, BM_SETCHECK,(WPARAM) 0,(LPARAM) nullptr);
                    }
                    else
                    {
                        SendMessage(control->currentHandle, BM_SETCHECK,(WPARAM) 1,(LPARAM) nullptr);
                    }
                }
            }
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

