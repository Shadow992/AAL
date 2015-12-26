#ifndef CONTROL_H
#define CONTROL_H

#include <vector>
#include <map>
#include <Windows.h>
#include "Window.h"
#include "UserInterface.h"

class Control
{
    std::map<GUIEvent, void*> eventActions;
	Window* parentWindow;
	std::mutex* synchronizer;
	std::mutex* msgSynchronizer;

  public:
    Control();
    Control(HWND handle,Window* parent,std::mutex* syn, std::mutex* msgSyn, GUIType t);
    virtual ~Control();

    void setText(const std::string& newText);
    void addOnEventAction(GUIEvent eventType, void* action);
	void* getOnEventAction(GUIEvent eventType);
	std::string getText();
	bool isChecked();

	bool remove();
	bool show(bool);

	GUIType type;
	HWND currentHandle;
};

#endif // CONTROL_H
