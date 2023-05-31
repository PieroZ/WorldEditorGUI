#pragma once
#include "WindowGUIController.h"

class SaveGameController : public WindowGUIController
{
public:
	SaveGameController(std::shared_ptr<WindowGUI>& window_gui);
	~SaveGameController();

	void OnConfirm(bool& close_window) override;
	void OnUpKey() override;
	void OnDownKey() override;

private:
	int GetRowId(int to_be_selected_id) const;


private:
	enum CHOICES_IDS { Yes = 1, No = 2};
};