#pragma once
#include "WindowGUI.h"
#include "memory"


class WindowGUIController
{
public:
	WindowGUIController(std::shared_ptr<WindowGUI>& window_gui);

	virtual void OnConfirm(bool& close_window);
	virtual void OnUpKey();
	virtual void OnDownKey();
	std::shared_ptr< WindowGUI > GetControlledGUI();

protected:
	std::shared_ptr<WindowGUI> m_window_gui;
};
