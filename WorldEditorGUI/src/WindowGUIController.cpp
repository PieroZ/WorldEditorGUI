#include "WindowGUIController.h"
#include "Log.h"

WindowGUIController::WindowGUIController(std::shared_ptr<WindowGUI>& window_gui) : m_window_gui(window_gui)
{
	TRACE("I znowu...");
}

void WindowGUIController::OnConfirm(bool& close_window)
{
	// Do nothing pure virtual
}

void WindowGUIController::OnUpKey()
{
	// Do nothing pure virtual
}

void WindowGUIController::OnDownKey()
{
	// Do nothing pure virtual
}

std::shared_ptr< WindowGUI > WindowGUIController::GetControlledGUI()
{
	return m_window_gui;
}
