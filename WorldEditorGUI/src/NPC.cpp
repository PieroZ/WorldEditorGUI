#include "NPC.h"
#include "TextureBank.h"
#include <filesystem>
#include "Stringify.h"
#include "ActiveFactStates.h"
#include "Config.h"

#include "App.h"


#include <iostream>

static const auto RENDER_NPC_NAMES = ParamsRegister::R_bool("B_RenderNPCNames", false);


NPC::NPC(/*App& world,*/ std::shared_ptr<Location> occupied_location, int x, int y, int id, int sprite_x, int sprite_y) : EntityWalker(/*world,*/ occupied_location, x, y, id, sprite_x, sprite_y)
{
	std::cout << "NPC created" << std::endl;
	m_interactable_id = id + 10000;

	//std::string lua_script_path("res/scripts/dialogues/" "kedar.lua");
	m_dialog_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	std::string lua_script_path("res/scripts/dialogues/DIA_NPC_" + Stringify::Int(m_interactable_id) + ".lua");
	if (std::filesystem::exists(lua_script_path))
	{
		dialogue_exists = true;
		m_is_dialogue = dialogue_exists;
		auto ob = m_dialog_lua.script_file(lua_script_path);
	}
	m_render_npc_name = RENDER_NPC_NAMES();
	//Col_X = 7;
	//Col_Y = 26;
	//Col_Width = 72;
	//Col_Height = 139;
}

NPC::NPC(const std::string& npc_name)
{
	Col_X = 7;
	Col_Y = 76;
	Col_Width = 72;
	Col_Height = 80;
	m_npc_name = npc_name;
	m_render_npc_name = RENDER_NPC_NAMES();
}

//void NPC::OnRender(SDL_Renderer* renderer)
//{
//	TextureBank::Get(m_sprite_sheet_name)->Render(m_x, m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, m_sprite_id_x * HERO_SPRITE_WIDTH, m_sprite_id_y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
//}

void NPC::OnRender(SDL_Renderer* renderer, Uint8 r/* = 255*/, Uint8 g /*= 255*/, Uint8 b /*= 255*/)
{
	// Draw only if at least one of 4 coordinates:
	// x0
	// x1 = x0 + sprite_width
	// y0
	// y1 = y0 + sprite_height
	// is inside the drawing scope. Drawing scope = <TOP_LEFT_TILE_ID.x, TOP_RIGHT_TILE_ID.x>

	// TODO
	// Save as state/flag ie. is_drawn_on_screen and update every time camera or npc moves 
	if (ShouldBeDrawnOnScreen(m_x, m_y)
		|| ShouldBeDrawnOnScreen(m_x + SCALED_HERO_SPRITE_WIDTH, m_y)
		|| ShouldBeDrawnOnScreen(m_x, m_y + SCALED_HERO_SPRITE_HEIGHT)
		|| ShouldBeDrawnOnScreen(m_x + SCALED_HERO_SPRITE_WIDTH, m_y + SCALED_HERO_SPRITE_HEIGHT))
	{

		int screen_x = m_x - std::max(m_location_camera->GetX() - m_location_camera->half_view_width, 0);
		int screen_y = m_y - std::max(m_location_camera->GetY() - m_location_camera->half_view_height, 0);

		int sprites_rightmost_pixel_pos_on_screen = screen_x + SCALED_HERO_SPRITE_WIDTH;
		int sprites_lowest_pixel_pos_on_screen = screen_y + SCALED_HERO_SPRITE_HEIGHT;

		if (sprites_rightmost_pixel_pos_on_screen >= 0 && sprites_lowest_pixel_pos_on_screen >= 0)
		{
			TextureBank::Get(m_sprite_sheet_name)->Render(screen_x, screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, m_sprite_x * HERO_SPRITE_WIDTH, m_sprite_y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
			if (m_render_npc_name)
			{
				if (!m_npc_name_texture)
				{
					m_npc_name_texture = std::make_shared<Texture>();
					if (!m_debug_font)
					{
						std::cout << "Missing font in npc... \n";
					}
					m_npc_name_texture->loadFromRenderedText(renderer, m_npc_name, { 0,0,0 }, m_debug_font);
				}
				else
				{
					m_npc_name_texture->Render(screen_x, screen_y);
				}
			}

			//const SDL_Rect rect{ screen_x, screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
			//SDL_RenderDrawRect(renderer, &rect);

			////if (m_render_player_collision)
			//{
			//	//const SDL_Rect col_rect{ player.m_x + player.Col_X, player.m_y + player.Col_Y, player.Col_Width, player.Col_Height };
			//	const SDL_Rect col_rect{ screen_x + Col_X, screen_y + Col_Y, Col_Width, Col_Height };
			//	SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
			//	SDL_RenderDrawRect(renderer, &col_rect);
			//	SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
			//	
			//	/*const SDL_Rect bound_rect{ player_screen_x, player_screen_y , SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
			//	SDL_RenderDrawRect(Renderer, &bound_rect);*/
			//}
		}
	}
}

void NPC::DrawBoundingRect(SDL_Renderer* renderer)
{
	const SDL_Rect rect{ m_x, m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
	SDL_RenderDrawRect(renderer, &rect);
}

void NPC::Talk(DialogGUI* gui, DialogControllerState& state, bool& has_said, std::vector<std::string>& choices_lines, int& choice_id, bool& last_line_flag)
{
	// If previous state was choices...
	if (state == DialogControllerState::DIALOG_CHOICES)
	{



		// then switch to dialogue
		state = DialogControllerState::DIALOG_ACTIVE;
		// Istota ponizszej linii jest aby wykonala sie w lua 
		auto lua_sel = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_SELECTION](choices_lines[choice_id]);
		//int lua_fact = m_dialog_lua[LUA_FACTS_MODIFIED];
		//if (lua_fact)
		//{
		//	static const char LUA_FACT_INC_TABLE[] = "FactIncTable";
		//	sol::function first_func = m_dialog_lua["say_hi"];
		//	first_func();

		//	//std::cout << "JAZDA\n" << lua_fact;
		//}

		//m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_SELECTION]("Nie");
		choices_lines.clear();
		gui->SetHighlightedRow(-1);
	}

	//auto fact_inc_table_lua = m_dialog_lua[LUA_FACT_INC_TABLE];
	//if (fact_inc_table_lua.valid())
	//{
	//	std::cout << "SOLID\n";
	//}

	sol::optional<sol::function> pick_start_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_PICK_STARTING_POINT];

	if (pick_start_lua)
	{
		m_dialog_lua[LUA_CURRENT_DIALOG][LUA_PICK_STARTING_POINT]();
	}


	std::string returned_text;
	// Check if got anything to say
	returned_text = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_SAY];
	//std::cout << returned_text << std::endl;

	auto is_done_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_DONE];
	has_said = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_SAY].valid();
	bool is_done = is_done_lua.valid();

	sol::optional<sol::function> continue_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_CONTINUE];
	sol::optional<sol::function> update_facts_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_UPDATE_FACTS];
	sol::optional<sol::function> end_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_END];


	if (update_facts_lua)
	{
		m_dialog_lua[LUA_CURRENT_DIALOG][LUA_UPDATE_FACTS]();
	}
	if (end_lua)
	{
		auto end_dialogue = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_END]();
		last_line_flag = true;
	}
	else if (continue_lua)
	{
		auto next_line = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_CONTINUE]();
	}


	if (!has_said)
	{
		sol::optional<sol::function> on_selection_lua = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_SELECTION];
		if (on_selection_lua)
		{
			//auto next_line = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_CONTINUE]();
			sol::table choices = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_CHOICES];
			 choice_id = 0;
			if (choices.valid())
			{
				state = DialogControllerState::DIALOG_CHOICES;

				//choice_id = 0;
				gui->SetHighlightedRow(choice_id);

				/*			choices.for_each([&](sol::object const& key, sol::object const& value)
								{
									m_choices.emplace_back(value.as < std::string>());
									returned_text += value.as < std::string>();
									returned_text += "\n";
								});*/


				int load_choice_id = 0;

				std::string current_choice = "choice" + Stringify::Int(load_choice_id);


				while (m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_CHOICES][current_choice].valid())
				{
					std::string current_line = m_dialog_lua[LUA_CURRENT_DIALOG][LUA_ON_CHOICES][current_choice];
					choices_lines.emplace_back(current_line);
					returned_text += current_line;
					returned_text += "\n";
					load_choice_id++;
					current_choice = "choice" + Stringify::Int(load_choice_id);
				}


			}
			// choices objects?
		}
	}
	else
	{
		state = DialogControllerState::DIALOG_ACTIVE;
	}


	if ((state == DialogControllerState::DIALOG_ACTIVE) && ((!continue_lua /*&& !on_selection_lua*/) || is_done))
	{
		last_line_flag = true;
	}

	gui->SetDisplayedText(returned_text, m_npc_name);
	//UpdateDialogGui();
	//return "HEY HEY";
}

/* NPC::OldTalk(bool& last_line)
{
	std::string e = (m_dialog_lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	// CHECK IF DONE
	auto is_done_lua = m_dialog_lua["CurrentDialog"]["done"];

	if (is_done_lua.valid())
	{
		last_line = true;
	}

	sol::optional<sol::function> continue_lua = m_dialog_lua["CurrentDialog"]["continue"];
	if (continue_lua)
	{
		auto next_line = m_dialog_lua["CurrentDialog"]["continue"]();
	}

	//sol::optional<sol::function> on_selection_lua = lua["CurrentDialog"]["onSelection"];
	//if(on_selection_lua)
	//{
	//	auto next_line = lua["CurrentDialog"]["continue"]();
	//}

	//auto continue_lua = lua["CurrentDialog"]["continue"]();

	/*e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	(lua["CurrentDialog"]["continue"]());

	e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	(lua["CurrentDialog"]["continue"]());

	e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;*/


std::string NPC::GetNpcName() const
{
	return m_npc_name;
}

void NPC::SetName(const std::string& name)
{
	m_npc_name = name;
}


// Do something with interactable id?
bool NPC::LoadDialogue()
{
	std::string lua_script_path("res/scripts/dialogues/" + m_npc_name + ".lua");
	if (std::filesystem::exists(lua_script_path))
	{
		dialogue_exists = true;
		m_is_dialogue = dialogue_exists;
		m_dialog_lua.set_function("ReadLuaTableValues", ReadLuaTableValues);
		m_dialog_lua.set_function("CheckLuaFacts", CheckLuaFacts);
		auto ob = m_dialog_lua.script_file(lua_script_path);
		return true;
	}
	return false;
}

// TODO: Check if used 
bool NPC::LoadDialogue(const std::string& dialogue_file_path)
{
	if (std::filesystem::exists(dialogue_file_path))
	{
		dialogue_exists = true;
		m_is_dialogue = dialogue_exists;
		auto ob = m_dialog_lua.script_file(dialogue_file_path);
		return true;
	}
	return false;
}
//
//sol::state NPC::GetDialogLuaState()
//{
//	return m_dialog_lua;
//}

void NPC::SetDialogLuaState(sol::state* dialog_state)
{
	//m_dialog_lua = *dialog_state;
}

bool NPC::IsRotatable() const
{
	return true;
}

void NPC::SetDirBasedOnPlayer(const Direction& players_dir)
{
	if (players_dir == Direction::UP)
	{
		SetDir(Direction::DOWN);
	}
	else if (players_dir == Direction::DOWN)
	{
		SetDir(Direction::UP);
	}
	else if (players_dir == Direction::RIGHT)
	{
		SetDir(Direction::LEFT);
	}
	else if (players_dir == Direction::LEFT)
	{
		SetDir(Direction::RIGHT);
	}
}

void NPC::SetDir(const Direction& dir)
{
	m_dir = dir;
	SetSpriteXYFromDir(m_dir);
}

int NPC::GetSpriteIdFromDir(const Direction& dir) const
{
	int result = -1;

	if (m_dir == Direction::DOWN)
	{
		result = 0;
	}
	else if (m_dir == Direction::LEFT)
	{
		result = 1;
	}
	else if (m_dir == Direction::RIGHT)
	{
		result = 2;
	}
	else if (m_dir == Direction::UP)
	{
		result = 3;
	}
	return result;
}

void NPC::SetSpriteXYFromDir(const Direction& dir)
{
	if (m_dir == Direction::DOWN)
	{
		m_sprite_y = m_base_sprite_y;
	}
	else if (m_dir == Direction::LEFT)
	{
		m_sprite_y = m_base_sprite_y + 1;
	}
	else if (m_dir == Direction::RIGHT)
	{
		m_sprite_y = m_base_sprite_y + 2;
	}
	else if (m_dir == Direction::UP)
	{
		m_sprite_y = m_base_sprite_y + 3;
	}
}

// UNUSED ?
void NPC::Interact(App* app)
{
	printf("So I guess, I'll just dance !\n");

	if (this->IsRotatable())
	{
		this->SetDirBasedOnPlayer(app->player.GetDir());
	}
	app->m_state = App::STATE_ENUM::DIALOGUE;
	//UpdateCurrentlyShownText(w_obj.PlayDialogue(m_npc_currently_interacted_with, m_dialogue_last_line));
	//m_dial_gui.SetDisplayedText(w_obj.PlayDialogue(m_npc_currently_interacted_with, m_dialogue_last_line));
	app->m_dial_controller.LoadDialog("res/scripts/dialogues/" + GetNpcName() + ".lua", this);
	app->m_dial_controller.StartDialog();
	app->m_dial_controller.ContinueDialog();
}

void NPC::FillWithDBData(const DBCharacterEntry& entry)
{
	SetSpritesheetName(entry.m_spritesheet_name);
	SetSpriteCoords(entry.m_sprite_x, entry.m_sprite_y);
	/*void SetSpritesheetName(const std::string & sprite_sheet_name);
	void SetSpriteCoords(int x, int y);*/
}

bool NPC::HasDialogue() const
{
	return dialogue_exists;
}

void NPC::ReadLuaTableValues(sol::table properties)
{
	//std::cout << "Inside: test_function" << std::endl;

	//std::string text = properties["1"];
	//std::cout << "The text is: " << text << std::endl;
	//std::string text2 = properties["2"];
	//std::cout << "The text2 is: " << text2 << std::endl;

	for (const auto& key_value_pair : properties)
	{
		sol::object key = key_value_pair.first;
		sol::object value = key_value_pair.second;
		std::string k = key.as<std::string>();
		std::string value_str = value.as<std::string>();
		/*sol::type t = value.get_type();*/

		std::cout << "key = " << k << std::endl;
		std::cout << "value_str = " << value_str << std::endl;

		ActiveFactStates::get().ModifyFactValue(k, Stringify::ToInt(value_str));
	}


//	std::cout << "The number is: " << static_cast<int>(properties["number"]) << std::endl;
}

bool NPC::CheckLuaFacts(sol::table table_facts_check)
{
	bool is_condition_met = true;
	for (const auto& key_value_pair : table_facts_check)
	{
		sol::object key = key_value_pair.first;
		sol::object value = key_value_pair.second;
		std::string k = key.as<std::string>();
		std::string value_str = value.as<std::string>();
		/*sol::type t = value.get_type();*/

		std::cout << "key = " << k << std::endl;
		std::cout << "value_str = " << value_str << std::endl;

		int active_fact_value = ActiveFactStates::get().GetFactValue(k);

		if (active_fact_value < Stringify::ToInt(value_str))
		{
			is_condition_met = false;
			break;
		}

	}
	return is_condition_met;
}

void NPC::SetDebugFont(TTF_Font* debug_font)
{
	if (debug_font)
	{
		m_debug_font = debug_font;
	}

}

bool NPC::ShouldBeDrawnOnScreen(int x, int y)
{
	return ((x >= m_location_camera->GetX() - m_location_camera->half_view_width)
		&& (x < m_location_camera->GetX() + m_location_camera->half_view_width)
		&& (y >= m_location_camera->GetY() - m_location_camera->half_view_height)
		&& (y < m_location_camera->GetY() + m_location_camera->half_view_height));
}


