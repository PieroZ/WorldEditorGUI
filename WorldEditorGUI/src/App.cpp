//==============================================================================
#include "App.h"
#include <SDL_image.h>
#include <stdio.h>
#include "Stringify.h"
#include <fstream>
#include <sstream>

App App::Instance;


#define TILE_COLLISION_DEBUG_RENDER 0

//==============================================================================
App::App() : player(200, 400)//, npc(400, 400, 0, 0)
{
	BackgroundWidth = 0;
	BackgroundHeight = 0;
	//start_loc.OnLoad("res/map/map.txt");

	//Location::starting_loc.OnLoad("res/map/map.txt");
}

//------------------------------------------------------------------------------
void App::OnKeyDown(SDL_Event* Event) 
{
	static const int speed = 15;
	if (m_state == STATE_ENUM::NORMAL)
	{
		if (Event->type == SDL_KEYDOWN)
		{
			if (Event->key.keysym.sym == SDLK_DOWN || Event->key.keysym.sym == SDLK_s)
			{
				player.Move(0, speed, w_obj);
			}
			else if (Event->key.keysym.sym == SDLK_UP || Event->key.keysym.sym == SDLK_w)
			{
				player.Move(0, -speed, w_obj);
			}
			else if (Event->key.keysym.sym == SDLK_RIGHT || Event->key.keysym.sym == SDLK_d)
			{
				player.Move(speed, 0, w_obj);
			}
			else if (Event->key.keysym.sym == SDLK_LEFT || Event->key.keysym.sym == SDLK_a)
			{
				player.Move(-speed, 0, w_obj);
			}
			else if (Event->key.keysym.sym == SDLK_RETURN)
			{
				int tile_id_to_be_interacted_with = Location::starting_loc.GetTileDirId(player.m_x, player.m_y, static_cast<int>(player.m_dir));
				int x = 0;
				int y = 0;
				Location::IdToXY(tile_id_to_be_interacted_with, x, y);
				bool has_dialogue = false;
				//uint32_t interactable_id = 0;
				if (w_obj.IsInteractableAt(x, y, has_dialogue, m_dialogueable))
				{
					if (has_dialogue)
					{
						m_state = STATE_ENUM::DIALOGUE;
						UpdateCurrentlyShownText(w_obj.PlayDialogue(m_dialogueable, m_dialogue_last_line));
					}
				}
			}
		}
	}
	else if (m_state == STATE_ENUM::DIALOGUE)
	{
		if (Event->type == SDL_KEYDOWN)
		{
			if (Event->key.keysym.sym == SDLK_RETURN)
			{
				if (!m_dialogue_last_line)
				{
					UpdateCurrentlyShownText(w_obj.PlayDialogue(m_dialogueable, m_dialogue_last_line));
				}
				else
				{
					m_dialogue_last_line = false;
					m_state = STATE_ENUM::NORMAL;
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void App::OnKeyUp(SDL_Event* Event) 
{
	if (m_state == STATE_ENUM::NORMAL)
	{
		if (Event->type == SDL_KEYUP)
		{
			if (Event->key.keysym.sym == SDLK_DOWN || Event->key.keysym.sym == SDLK_s)
			{
				player.m_moving = false;
			}
			else if (Event->key.keysym.sym == SDLK_UP || Event->key.keysym.sym == SDLK_w)
			{
				player.m_moving = false;
			}
			else if (Event->key.keysym.sym == SDLK_RIGHT || Event->key.keysym.sym == SDLK_d)
			{
				player.m_moving = false;
			}
			else if (Event->key.keysym.sym == SDLK_LEFT || Event->key.keysym.sym == SDLK_a)
			{
				player.m_moving = false;
			}
		}
	}
}


void App::UpdateCurrentlyShownText(const std::string& text, SDL_Color text_color)
{
	currently_displayed_line = text;

	int w = 0;
	int h = 0;
	if (TTF_SizeText(m_font, text.c_str(), &w, &h) < 0) 
	{
		std::cerr << "Error: Unable to retrieve size of text" << std::endl;
		return;
	}
	   
	/*if (w > 1000)
	{
		std::istringstream iss(text);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>());

		int idx = 0;
		std::string line_to_be_splitted;
		int line_width = 0;
		while (line_width < 900)
		{
			line_to_be_splitted = line_to_be_splitted + results[idx] + " ";
			++idx;

			if (TTF_SizeText(m_font, line_to_be_splitted.c_str(), &line_width, &h) < 0)
			{
				std::cerr << "Error: Unable to retrieve size of text" << std::endl;
				return;
			}

			if (line_width >= 900)
			{
				line_to_be_splitted += "\n";
			}
		}

		currently_displayed_line = "UGAUGAGAUGA\nGSRGSRGJR";
	}*/


	if (!text_texture.loadFromRenderedText(Renderer, currently_displayed_line, text_color, m_font))
	{
		printf("Failed to render text texture!\n");
		//success = false;
	}
}


//------------------------------------------------------------------------------
void App::Loop() 
{
	//CAppStateManager::OnLoop();
}

//------------------------------------------------------------------------------
void App::Render() 
{
	SDL_RenderClear(Renderer);
	/*for (int r = 0; r < TILES_ROWS; ++r)
	{
		for (int c = 0; c < TILES_COLS; ++c)
		{
			TextureBank::Get("ss_mod")->Render(c * SCALED_SPRITE_WIDTH, r * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, SPRITE_WIDTH * TEST_TILE_OFF_X, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
		}
	}*/
	
	int id = 0;
	int idx = 0;
	int idy = 0;
	for (auto&& p : Location::starting_loc.m_tiles_arr)
	{
		TextureBank::Get("ss_nomargin")->Render(idx * SCALED_SPRITE_WIDTH, idy * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, p.m_tile_y * SPRITE_WIDTH, p.m_tile_x * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
#if TILE_COLLISION_DEBUG_RENDER
		const SDL_Rect col_rect{ idx * SCALED_SPRITE_WIDTH, idy * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT };
		if (p.TypeID == 2)
		{
			SDL_SetRenderDrawColor(Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		}

		SDL_RenderDrawRect(Renderer, &col_rect);
		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
#endif // TILE_COLLISION_DEBUG_RENDER
		id++;
		idx = id % TILES_COLS;
		idy = id / TILES_COLS;


//#if TILE_COLLISION_DEBUG_RENDER

//#endif // TILE_COLLISION_DEBUG_RENDER

	}

	id = 0;
	idx = 0;
	idy = 0;
	for (auto&& p : Location::starting_loc_elements.m_tiles_arr)
	{
		if (p.TypeID != TILE_TYPE_NONE)
		{
			TextureBank::Get("ss_nomargin")->Render(idx * SCALED_SPRITE_WIDTH, idy * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, p.m_tile_y * SPRITE_WIDTH, p.m_tile_x * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
		}
		id++;
		idx = id % TILES_COLS;
		idy = id / TILES_COLS;
	}

	//TILE_TYPE_NONE

	//const SDL_Rect hmm{ idx * SCALED_SPRITE_WIDTH, idy-1 * SCALED_SPRITE_HEIGHT, 150, 150 };
	////SDL_SetRenderDrawColor(Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(Renderer, &hmm);
	
	SDL_Point sprite_coords = player.GetSpriteCoordinates();
	if (player.m_moving)
	{
		TextureBank::Get("char_ss")->Render(player.m_x, player.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH * player.player_anim.GetCurrentFrame(), sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	}
	else
	{
		TextureBank::Get("char_ss")->Render(player.m_x, player.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	}

	/*const SDL_Rect rect{ player.m_x, player.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
	SDL_RenderDrawRect(Renderer, &rect);*/

#if TILE_COLLISION_DEBUG_RENDER
	const SDL_Rect col_rect{ player.m_x + player.Col_X, player.m_y + player.Col_Y, player.Col_Width, player.Col_Height };
	SDL_SetRenderDrawColor(Renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(Renderer, &col_rect);
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
#endif // TILE_COLLISION_DEBUG_RENDER

	//for (auto&& npc : w_obj.GetNpcArr())
	for (auto&& npc : w_obj.GetNpcArr())
	{
		npc->OnRender(Renderer);
		npc->DrawBoundingRect(Renderer);
	}
	
	if (m_state == STATE_ENUM::DIALOGUE)
	{
		TextureBank::Get("dialogue_frame")->Render(380, 850);
		text_texture.Render(400, 900);
	}
	   
	//for (int i = 0; i < w_obj.GetNpcArr().size(); i++) {
	//	if (!CEntity::EntityList[i]) continue;

	//	CEntity::EntityList[i]->OnRender(Surf_Display);
	//}


	//TextureBank::Get("char_ss")->Render(npc.m_x, npc.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, 4 * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);

	//CAppStateManager::OnRender();

	SDL_RenderPresent(Renderer);            // glowna funkcja resetujaca obraz
}

//------------------------------------------------------------------------------
void App::Cleanup()
{
	//TextureBank::Cleanup();

	if (Renderer != nullptr) {
		SDL_DestroyRenderer(Renderer);
		Renderer = NULL;
	}

	if (Window) {
		SDL_DestroyWindow(Window);
		Window = NULL;
	}

	//Free  font
	TTF_CloseFont(m_font);
	m_font = nullptr;

	//if (m_font) {
	//	TTF_CloseFont(m_font);
	//	m_font = nullptr;
	//}

	//for (uint32_t i = 0; i < CEntity::EntityList.size(); i++) {
	//	if (!CEntity::EntityList[i]) continue;

	//	CEntity::EntityList[i]->OnCleanup();
	//}

	//CEntity::EntityList.clear();
	//CArea::AreaControl.OnCleanup();



	//	CAppStateManager::SetActiveAppState(APPSTATE_INTRO, NULL);

	//	CSoundBank::SoundControl.OnCleanup();

	//    Mix_CloseAudio();


	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void App::FontInit()
{
	// TheWildBreathOfZelda-15Lv.ttf
	// AGoblinAppears-o2aV.ttf
	// res/fonts/AncientModernTales-a7Po.ttf
	std::string font1 = "res/fonts/AncientModernTales-a7Po.ttf";
	std::string font2 = "res/fonts/AGoblinAppears-o2aV.ttf";
	std::string font3 = "res/fonts/TheWildBreathOfZelda-15Lv.ttf";
	std::string font4 = "res/fonts/JmharkhamRegular-olD0.ttf";

	std::string fontfile(font4);
	//std::fstream file("res/fonts/AncientModernTales-a7Po.txt");

	//if (file.is_open()) {
	//	file >> fontfile;
	//	file.close();
	//}
	//else {
	//	std::cerr << "Error: Unable to open \"fontname.txt\"" << std::endl;
	//}


	if (!(m_font = TTF_OpenFont(fontfile.c_str(), 26))) 
	{
		std::cerr << "Error: Unable to open font" << std::endl;
		return;
	}

	//int w = 0;
	//int h = 0;
	//std::string text = "SDL_ttf is awesome!";
	//SDL_Color red = { 255, 0, 0 };

	//if (TTF_SizeText(m_font, text.c_str(), &w, &h) < 0) {
	//	std::cerr << "Error: Unable to retrieve size of text" << std::endl;
	//	return;
	//}

	//Render text
	SDL_Color textColor = { 0, 0, 0 };
	currently_displayed_line = "";
	if (!text_texture.loadFromRenderedText(Renderer, currently_displayed_line, textColor, m_font))
	{
		printf("Failed to render text texture!\n");
		//success = false;
	}
	//text_solid = TTF_RenderText_Solid(font, text.c_str(), red);
}

//------------------------------------------------------------------------------
int App::Execute(int argc, char* argv[]) 
{
	/*auto& config = Config::GetInstance();
	config.tuning_config.ParseTuningConfig(config.m_ConfigFilename);*/

	if (!Init()) return 0;

	SDL_Event Event;

	while (Running) {
		while (SDL_PollEvent(&Event) != 0) {
			OnEvent(&Event);

		}
		Loop();		// Update 60 times per second ?
		Render();

		SDL_Delay(1); // Breath
	}

	Cleanup();

	return 1;
}



//==============================================================================
SDL_Renderer* App::GetRenderer() { return Renderer; }

//TTF_Font* App::GetAppFont() { return m_font; }

//==============================================================================
App* App::GetInstance() { return &App::Instance; }

//int App::GetWindowWidth()  { return WINDOWWIDTH; }
//int App::GetWindowHeight() { return WINDOWHEIGHT; }

//==============================================================================


//------------------------------------------------------------------------------
bool App::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		//Log("Unable to Init SDL: %s", SDL_GetError());
		return false;
	}

	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0")) {
		//Log("Unable to Init hinting: %s", SDL_GetError());
	}

	//Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	}

	//lua_State* L = luaL_newstate();
	// Init LUA
	
	/*lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	std::string lua_script_path("res/scripts/dialogues/kedar.lua");
	lua.script_file(lua_script_path);*/

	//lua.script("CurrentDialog = gossip2");
	//lua["CurrentDialog"] = lua.g

	//sol::object number_obj = lua.get<sol::object>("number");
	//e = (lua["gossip"]);
	//std::cout << e << std::endl;

	//e = (lua["gossip2"]);
	//std::cout << e << std::endl;
	/*e = (lua["CurrentDialog"]["say"]);
	std::cout << e << std::endl;*/

	int i;

	// Declare display mode structure to be filled in.
	SDL_DisplayMode current;


	// Get current display mode of all displays.
	for (i = 0; i < SDL_GetNumVideoDisplays(); ++i)
	{
		int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

		if (should_be_zero != 0)
			// In case of error...
			SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

		//else
			// On success, print the current display mode.
		   // SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate);
			//logger.DEBUGG("Display #" + Stringify::Int(i) + ": current display mode is " + Stringify::Int(current.w) + "x" + Stringify::Int(current.h) + "px @ " + Stringify::Int(current.refresh_rate) + "hz");
	}


	//const int SCREEN_WIDTH = 640;
	//const int SCREEN_HEIGHT = 480;

	//title = "My SDL Game";
	if ((Window = SDL_CreateWindow(
		"My SDL Game",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		current.w, current.h, SDL_WINDOW_SHOWN) // || SDL_WINDOW_FULLSCREEN
		) == NULL)
	{
		//Log("Unable to create SDL Window: %s", SDL_GetError());
		return false;
	}

	PrimarySurface = SDL_GetWindowSurface(Window);

	if ((Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED)) == NULL)	// SDL_RENDERER_PRESENTVSYNC
	{
		//Log("Unable to create renderer");
		return false;
	}

	SDL_SetRenderDrawColor(Renderer, 0x00, 0x00, 0x00, 0xFF);

	// Initialize image loading for PNGs
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		printf("Unable to init SDL_image: %s", IMG_GetError());
		//Log("Unable to init SDL_image: %s", IMG_GetError());
		return false;
	}
	// Load all of our Textures (see TextureBank class for expected folder)
	if (TextureBank::Init() == false) {
		//Log("Unable to init TextureBank");
		return false;
	}

	//if (TTF_Init() == -1) {
	//	printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
	//	return false;
	//}

	//m_font = TTF_OpenFont("res/Fonts/lazy2.ttf", 28);
	//if (m_font == nullptr) {
	//	printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
	//	return false;
	//}

	//if (TextureBank::InitTextureBankForArea("starting_area.txt") == false)
	//{
	//	Log("Unable to init TextureBank");
	//	return false;
	//}

	//BackgroundWidth = TextureBank::Get("BackGroundTest20")->GetWidth(); // 1920
 //   BackgroundHeight = TextureBank::Get("BackGroundTest20")->GetWidth() ; // 1024

	BackgroundWidth = 1920; // 1920
	BackgroundHeight = 1024; // 1024

	//Location::starting_loc.OnLoad("res/map/map3.txt");
	Location::starting_loc_elements.OnLoad("res/map/map3_addons.txt");
	Location::starting_loc.OnLoadLua("res/map/start_2.lua");

	//NPC* los = new NPC{ 400, 400, 0, 1, 4 };
	//EntityWalker::m_entity_arr.emplace_back(los);
	TextureBank::Get("dialogue_frame")->setAlpha(157);

	FontInit();
	//EntityWalker::m_entity_arr.emplace_back(NPC{1000, 400, 0, 4, 0});

		/*npc_arr.push_back(std::make_unique<NPC>(400, 400, 0, 1, 4));
	npc_arr.push_back(std::make_unique<NPC>(1000, 400, 0, 4, 0));*/

	//CAppStateManager::SetActiveAppState(APPSTATE_DIALOGUE_TEST, Renderer);

	//ReadMap();
	return true;
}

//------------------------------------------------------------------------------

void App::OnEvent(SDL_Event* Event) {
	if (Event->type == SDL_QUIT) Running = false;

	// Temporary
	if (Event->type == SDL_KEYDOWN) {
		if (Event->key.keysym.sym == SDLK_ESCAPE) {
			Running = false;
		}
	}

	CEvent::OnEvent(Event);

	//CAppStateManager::OnEvent(Event);
}

