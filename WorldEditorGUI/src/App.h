#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "sol.hpp"

#include "CEvent.h"
#include "PlayerWalker.h"
#include "WorldObjects.h"
#include "NPC.h"
#include "Location.h"
#include "Camera.h"
#include "TextureBank.h"
#include "DialogGUI.h"
#include "DialogController.h"

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

	DialogGUI m_dial_gui;
	DialogController m_dial_controller;

	SDL_Window* Window = NULL;
	SDL_Renderer* Renderer = NULL;
	TTF_Font* m_font = nullptr;
	// DEBUG VARS 
	TTF_Font* m_debug_font = nullptr;
	bool m_render_player_collision = false;
	// DEBUG VARS END

	SDL_Surface* PrimarySurface = NULL;
	bool m_dialogue_last_line = false;
	std::shared_ptr<NPC> m_npc_currently_interacted_with;

	std::string currently_displayed_line;

	int BackgroundWidth;
	int BackgroundHeight;
	PlayerWalker player;
	WorldObjects w_obj;
	Camera area_cam;
	std::shared_ptr<Location> m_current_location;

	sol::state lua;

	EdgeEnum player_edge_enum;
	//NPC npc;

	// Controller?
	bool player_moving_right = 0;
	bool player_moving_left = 0;
	bool player_moving_up = 0;
	bool player_moving_down = 0;
	

	bool leaving_location = false;
	bool entering_location = false;
	Uint8 fade_out_value = 255;
	Uint8 fade_out_inc = 5;
	std::vector<SDL_Point> tiles_id;

	// TEMP needed to apply below after moving to location fade out effect ends
	std::shared_ptr<Location> entered_loc;
	int loc_x = 0;
	int loc_y = 0;

private:
	App();

	// Capture SDL Events
	void OnEvent(SDL_Event* Event);

	// Initialize our SDL game / app
	bool Init();
	bool PrimarySDLInit();

	void MoveToAnotherLocationSequence();

	bool LoadLocationsResources();

	bool LoadAllEntrancePoints();
	//bool LoadWaypointsLua(char* File);
	bool LoadWaypointsLua(const std::string& File);

	// Logic loop
	void Loop();
	void StopPlayerMovement();

	// Render loop (draw)
	void Render();

	// Free up resources
	void Cleanup();

	void FontInit();

	void UpdateCurrentlyShownText(const std::string& text, SDL_Color text_color = { 255, 255, 255 });
	//void ReadMap();

	//void MapIdToCoords(const int cols, const int id);

public:

	void OnMove(int x_speed, int y_speed);
	void OnKeyDown(SDL_Event* Event);

	void OnKeyUp(SDL_Event* Event);

	int Execute(int argc, char* argv[]);

	void SetLocation(std::shared_ptr<Location> current_location);

public:
	SDL_Renderer* GetRenderer();
	//TTF_Font* GetAppFont();

public:
	static App* GetInstance();

	//static int GetWindowWidth();
	//static int GetWindowHeight();
};