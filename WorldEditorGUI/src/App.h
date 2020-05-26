#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "sol.hpp"

#include "CEvent.h"
#include "PlayerWalker.h"
#include "WorldObjects.h"
#include "NPC.h"
#include "Location.h"
#include "TextureBank.h"

#include <vector>

class App : public CEvent
{
private:
	static App Instance;

	enum class STATE_ENUM
	{
		NORMAL,
		DIALOGUE
	} m_state = STATE_ENUM::NORMAL;

	bool Running = true;

	SDL_Window* Window = NULL;
	SDL_Renderer* Renderer = NULL;
	TTF_Font* m_font = nullptr;
	SDL_Surface* PrimarySurface = NULL;
	Texture text_texture;
	bool m_dialogue_last_line = false;
	std::shared_ptr<NPC> m_dialogueable;

	std::string currently_displayed_line;

	int BackgroundWidth;
	int BackgroundHeight;
	PlayerWalker player;
	WorldObjects w_obj;
	Location start_loc;
	sol::state lua;
	//NPC npc;

	std::vector<SDL_Point> tiles_id;

private:
	App();

	// Capture SDL Events
	void OnEvent(SDL_Event* Event);

	// Initialize our SDL game / app
	bool Init();

	// Logic loop
	void Loop();

	// Render loop (draw)
	void Render();

	// Free up resources
	void Cleanup();

	void FontInit();

	void UpdateCurrentlyShownText(const std::string& text, SDL_Color text_color = { 255, 255, 255 });
	//void ReadMap();

	//void MapIdToCoords(const int cols, const int id);

public:

	void OnKeyDown(SDL_Event* Event);

	void OnKeyUp(SDL_Event* Event);

	int Execute(int argc, char* argv[]);

public:
	SDL_Renderer* GetRenderer();
	//TTF_Font* GetAppFont();

public:
	static App* GetInstance();

	//static int GetWindowWidth();
	//static int GetWindowHeight();
};