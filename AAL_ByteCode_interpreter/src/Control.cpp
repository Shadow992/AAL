#include "Control.h"

Control::Control()
{

}

Control::Control(HWND handle,Window* parent,std::mutex* syn, std::mutex* msgSyn, GUIType t)
{
    parentWindow=parent;
    currentHandle=handle;
    synchronizer=syn;
    msgSynchronizer=msgSyn;
    type=t;
}

Control::~Control()
{
    //dtor
}

void Control::setText(const std::string& newText)
{
    SendMessage(currentHandle, WM_SETTEXT, 0, (LPARAM) newText.c_str());
}

bool Control::isChecked()
{
    return SendMessage(currentHandle, BM_GETCHECK,(WPARAM) nullptr,(LPARAM) nullptr) == BST_CHECKED;
}

void Control::addOnEventAction(GUIEvent eventType, void* action)
{
    msgSynchronizer->lock();
    eventActions[eventType]=action;
    msgSynchronizer->unlock();
}

void* Control::getOnEventAction(GUIEvent eventType)
{
    auto action=eventActions.find(eventType);
    if(action != eventActions.end())
    {
        return action->second;
    }
    return nullptr;
}

std::string Control::getText()
{
    char text[65000];
    GetWindowText(currentHandle,text,65000);
    return text;
}

bool Control::remove()
{
    // not implemented yet
    return false;
}

bool Control::show(bool)
{
    // not implemented yet
    return false;
}
