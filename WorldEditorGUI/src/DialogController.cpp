#include "DialogController.h"
#include "Stringify.h"
#include <filesystem>


DialogController::DialogController() : m_last_line_flag(false), m_dialog_gui(nullptr), m_state(DialogControllerState::INACTIVE), m_choice_id(-1)
{
	m_dialog_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
}

void DialogController::StartDialog()
{
	m_last_line_flag = false;
}

void DialogController::ContinueDialog()
{
	//std::string NPC::Talk(DialogGUI* gui, DialogControllerState& state, bool& has_said, std::vector<std::string>& choices_lines, int& choice_id, bool& last_line_flag)
	bool has_said = false;
	m_dialogue_owner->Talk(m_dialog_gui, m_state, has_said, m_choices, m_choice_id, m_last_line_flag);

	//if (IsChoiceState())
	//{
	//	m_state = DialogControllerState::DIALOG_ACTIVE;
	//	m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_SELECTION](m_choices[m_choice_id]);
	//	//m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_SELECTION]("Nie");
	//	m_choices.clear();
	//	m_dialog_gui->SetHighlightedRow(-1);
	//}

	//// Check if got anything to say
	//m_current_text = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_SAY];
	//std::cout << m_current_text << std::endl;

	//auto is_done_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_DONE];
	//auto has_said = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_SAY].valid();
	//bool is_done = is_done_lua.valid();

	//sol::optional<sol::function> continue_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_CONTINUE];
	//if (continue_lua)
	//{
	//	auto next_line = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_CONTINUE]();
	//}


	//if (!has_said)
	//{
	//	sol::optional<sol::function> on_selection_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_SELECTION];
	//	if (on_selection_lua)
	//	{
	//		//auto next_line = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_CONTINUE]();
	//		sol::table choices = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_CHOICES];
	//		int choice_id = 0;
	//		if (choices.valid())
	//		{
	//			m_state = DialogControllerState::DIALOG_CHOICES;
	//			m_choice_id = 0;
	//			m_dialog_gui->SetHighlightedRow(m_choice_id);

	///*			choices.for_each([&](sol::object const& key, sol::object const& value)
	//				{
	//					m_choices.emplace_back(value.as < std::string>());
	//					m_current_text += value.as < std::string>();
	//					m_current_text += "\n";
	//				});*/


	//			int load_choice_id = 0;

	//			std::string current_choice = "choice" +  Stringify::Int(load_choice_id);


	//			while (m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_CHOICES][current_choice].valid())
	//			{
	//				std::string current_line = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_CHOICES][current_choice];
	//				m_choices.emplace_back(current_line);
	//				m_current_text += current_line;
	//				m_current_text += "\n";
	//				load_choice_id++;
	//				current_choice = "choice" + Stringify::Int(load_choice_id);
	//			}


	//		}
	//		// choices objects?
	//	}
	//}
	//else
	//{
	//	m_state = DialogControllerState::DIALOG_ACTIVE;
	//}


	//if ((m_state == DialogControllerState::DIALOG_ACTIVE) && ((!continue_lua /*&& !on_selection_lua*/) || is_done))
	//{
	//	m_last_line_flag = true;
	//}

	//UpdateDialogGui();

	//auto continue_lua = m_dialog_lua["CurrentDialog"][LUA_CONTINUE]();

	/*e = (m_dialog_lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	(m_dialog_lua["CurrentDialog"][LUA_CONTINUE]());

	e = (m_dialog_lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	(m_dialog_lua["CurrentDialog"][LUA_CONTINUE]());

	e = (m_dialog_lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;*/
}

std::vector < std::string>& DialogController::GetDialogChoices()
{
	// ??
	return m_choices;
}

bool DialogController::IsLastLine() const
{
	return m_last_line_flag;
}

bool DialogController::IsChoiceState() const
{
	return m_state == DialogControllerState::DIALOG_CHOICES;
}

bool DialogController::LoadDialog(const std::string& lua_dialog_path, std::shared_ptr<NPC>& npc_dialoguer)
{
	m_dialogue_owner = npc_dialoguer;
//	std::string lua_script_path("res/scripts/dialogues/DIA_NPC_" + Stringify::Int(m_interactable_id) + ".lua");

	//bool result = npc_dialoguer->LoadDialogue(lua_dialog_path);

	//if (result)
	//{
	//	m_dialog_lua = npc_dialoguer->GetDialogLuaState();
	//}
	//return result;
	//std::string lua_script_path(lua_dialog_path);
	//if (std::filesystem::exists(lua_script_path) )
	//{
	//	//dialogue_exists = true;
	//	//m_is_dialogue = dialogue_exists;
	//	auto ob = m_dialog_lua.script_file(lua_script_path);
	//	return true;
	//}
	//return false;
	return false;
}

void DialogController::SetDialogGui(DialogGUI* dialog_gui)
{
	m_dialog_gui = dialog_gui;
}

std::string DialogController::GetCurrentText()
{
	return m_current_text;
}

void DialogController::UpdateDialogGui()
{
	m_dialog_gui->SetDisplayedText(m_current_text, "DUPA");
}

void DialogController::SelectNextChoice()
{
	if (m_choice_id < m_choices.size() - 1)
	{
		m_choice_id++;
	}
	else
	{
		m_choice_id = 0;
	}
	m_dialog_gui->SetHighlightedRow(m_choice_id);
}

void DialogController::SelectPreviousChoice()
{
	if (m_choice_id > 0)
	{
		m_choice_id--;
	}
	else
	{
		m_choice_id = m_choices.size() - 1;
	}
	m_dialog_gui->SetHighlightedRow(m_choice_id);
}
