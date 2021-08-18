#pragma once

#include <vector>
#include "sol.hpp"
#include "DialogGUI.h"
#include "NPC.h"
#include <map>
#include "DialogControllerState.h"

class DialogController
{
public:
	DialogController();

	void StartDialog();
	void ContinueDialog();
	std::vector < std::string>& GetDialogChoices();

	bool IsLastLine() const;
	bool IsChoiceState() const;
	bool LoadDialog(const std::string& lua_dialog_path, std::shared_ptr<NPC>& npc_dialoguer);
	void SetDialogGui(DialogGUI* dialog_gui);
	std::string GetCurrentText();
	void UpdateDialogGui();
	void SelectNextChoice();
	void SelectPreviousChoice();
private:
	std::string m_current_text;
	std::vector<std::string> m_choices;
	int m_choice_id;
	bool m_last_line_flag;
	sol::state m_dialog_lua;
	DialogGUI* m_dialog_gui;
	DialogControllerState m_state;

	std::map<std::string, bool> sol_loaded;
	std::map<std::string, sol::state*> loaded_states;

	std::shared_ptr<NPC> m_dialogue_owner;


	inline static const char LUA_CURRENT_DIALOG[] = "CurrentDialog";
	inline static const char LUA_SAY[] = "say";
	inline static const char LUA_DONE[] = "done";
	inline static const char LUA_ON_SELECTION[] = "onSelection";
	inline static const char LUA_ON_CHOICES[] = "choices";
	inline static const char LUA_CONTINUE[] = "continue";
};