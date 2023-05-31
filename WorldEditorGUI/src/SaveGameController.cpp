#include "SaveGameController.h"
#include "Log.h"

SaveGameController::SaveGameController(std::shared_ptr<WindowGUI>& window_gui) : WindowGUIController(window_gui)
{
}

SaveGameController::~SaveGameController()
{
}

void SaveGameController::OnConfirm(bool& close_window)
{
	int selected_row = m_window_gui->GetHighlightedRow();
	close_window = false;

	if (selected_row == CHOICES_IDS::Yes)
	{
		INFO("Confirmed saving the game...");
	}
	else if (selected_row == CHOICES_IDS::No)
	{
		INFO("No saving required. Exiting saving gui...");
		close_window = true;
		// Pass information to app to exit save dialogue state
	}
}

void SaveGameController::OnUpKey()
{
	int row_to_be_selected = m_window_gui->GetHighlightedRow() - 1;
	int actual_selected_row = GetRowId(row_to_be_selected);

	m_window_gui->SetHighlightedText(actual_selected_row);
}

void SaveGameController::OnDownKey()
{
	int row_to_be_selected = m_window_gui->GetHighlightedRow() + 1;
	int actual_selected_row = GetRowId(row_to_be_selected);

	m_window_gui->SetHighlightedText(actual_selected_row);
}

int SaveGameController::GetRowId(int to_be_selected_id) const
{
	int selected_row;
	if (to_be_selected_id == CHOICES_IDS::Yes || to_be_selected_id > CHOICES_IDS::No)
	{
		selected_row = CHOICES_IDS::Yes;
	}
	else
	{
		selected_row = CHOICES_IDS::No;
	}

	return selected_row;
}
