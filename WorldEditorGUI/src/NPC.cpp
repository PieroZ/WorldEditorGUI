#include "NPC.h"
#include "TextureBank.h"
#include <filesystem>
#include "Stringify.h"

#include <iostream>

NPC::NPC(int x, int y, int id, int sprite_x, int sprite_y) : EntityWalker(x, y, id, sprite_x, sprite_y)
{
	std::cout << "NPC created" << std::endl;
	m_interactable_id = id + 10000;

	//std::string lua_script_path("res/scripts/dialogues/" "kedar.lua");
	lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	std::string lua_script_path("res/scripts/dialogues/DIA_NPC_" + Stringify::Int(m_interactable_id) + ".lua");
	if (std::filesystem::exists(lua_script_path))
	{
		dialogue_exists = true;
		m_is_dialogue = dialogue_exists;
		auto ob = lua.script_file(lua_script_path);
	}
	//Col_X = 7;
	//Col_Y = 26;
	//Col_Width = 72;
	//Col_Height = 139;
}

void NPC::OnRender(SDL_Renderer* renderer)
{
	TextureBank::Get("char_ss")->Render(m_x, m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, m_sprite_x * HERO_SPRITE_WIDTH, m_sprite_y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
}

void NPC::DrawBoundingRect(SDL_Renderer* renderer)
{
	const SDL_Rect rect{ m_x, m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
	SDL_RenderDrawRect(renderer, &rect);
}

std::string NPC::Talk(bool& last_line)
{
	std::string e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	// CHECK IF DONE
	auto is_done_lua = lua["CurrentDialog"]["done"];

	if (is_done_lua.valid())
	{
		last_line = true;
	}
	auto continue_lua = lua["CurrentDialog"]["continue"]();

	/*e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	(lua["CurrentDialog"]["continue"]());

	e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;

	(lua["CurrentDialog"]["continue"]());

	e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;*/
	return e;
}

void NPC::Interact()
{
	printf("So I guess, I'll just dance !\n");
}


