//==============================================================================
#include "App.h"
#include <SDL_image.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "Stringify.h"
#include "CFPS.h"
#include "Log.h"
#include "Config.h"
#include "DebugRenderer.h"
#include "LocationHelper.h"
#include "LookupTables.h"

App App::Instance;

std::set<int> ids_set;


static int TOTAL_FRAMES = 0;

static const int MOVEMENT_SPEED = 8;

// Location::starting_loc. - >w_obj.GetLocation("start_1")->
// Location::starting_loc ->

#define TILE_COLLISION_DEBUG_RENDER 0
#define FPS_DEBUG_RENDER 1
#define DEBUG_MODE 1


std::string STARTING_LOCATION_FILENAME_NO_EXT = "test_map";
std::string MAP_RESOURCE_LOCATION = "res/map";
std::string STARTING_LOCATION_PATH = "res/map/" + STARTING_LOCATION_FILENAME_NO_EXT + ".lua";



static const auto DEBUG_RENDER_PLAYER_COLLISION = ParamsRegister::R_bool("B_RenderPlayerCollision", false);
static const auto SET_PLAYER_X0 = ParamsRegister::R_int("P_StartX", 330);
static const auto SET_PLAYER_Y0 = ParamsRegister::R_int("P_StartY", 400);
static const auto SET_STARTING_LOCATION = ParamsRegister::R_string("StartingLocation", "test_map");

//const std::string ADDITIONAL_LOCATION_PATH = "res/map/" + "minio_city" + ".lua";





//==============================================================================
//App::App() : player(1920/2, 480)//, npc(400, 400, 0, 0)
// Values taken from map tiles width & height
//App::App() : player(Location::starting_loc, 1920/2, 480), area_cam(player.m_x, player.m_y, 30 * SCALED_SPRITE_WIDTH, 17 * SCALED_SPRITE_HEIGHT)//, npc(400, 400, 0, 0)
//App::App() : player(*this, 1260, 1290), area_cam(player.m_x, player.m_y, 0, 0)//, npc(400, 400, 0, 0)
//App::App() : player(*this, 330, 400), area_cam(player.m_x, player.m_y, 0, 0)//, npc(400, 400, 0, 0)
App::App() : player(330, 400), area_cam(*this, player.m_x, player.m_y, 0, 0)//, npc(400, 400, 0, 0)
//App::App() : player(Location::starting_loc, 1920/2, 480), area_cam(player.m_x, player.m_y, 40 * SCALED_SPRITE_WIDTH, 22 * SCALED_SPRITE_HEIGHT)//, npc(400, 400, 0, 0)
{



	BackgroundWidth = 0;
	BackgroundHeight = 0;

	//start_loc.OnLoad("res/map/map.txt");

	//w_obj.GetLocation("start_1")->OnLoad("res/map/map.txt");
}

void App::OnMove(int x_speed, int y_speed)
{
	player.Move(x_speed, y_speed, w_obj);

	// Move camera only if player is @ middle

	int player_screen_x = SCREEN_WIDTH / 2;

	if (x_speed != 0 && ((player.m_x >= player_screen_x) && (player.m_x <= (m_current_location->GetLocationWidth() - SCREEN_WIDTH / 2))))
	{
		//area_cam.Move(x_speed, 0);
		area_cam.SetX(player.m_x);
	}

	int player_screen_y = SCREEN_HEIGHT / 2;

	if (y_speed != 0 && ((player.m_y >= player_screen_y) && (player.m_y <= (m_current_location->GetLocationHeight() - SCREEN_HEIGHT / 2))))
	{
		//area_cam.Move(0, y_speed);
		area_cam.SetY(player.m_y);
	}


	//if (m_current_location->HasPlayerEnteredWaypoint(player.m_x, player.m_y))
	std::shared_ptr<Location> t_loc;
	int t_loc_x = 0;
	int t_loc_y = 0;

	if (m_current_location->HasPlayerEnteredWaypoint(player, t_loc, t_loc_x, t_loc_y))
	{
		entered_loc = t_loc;
		loc_x = t_loc_x;
		loc_y = t_loc_y;
		//After fade out sequence
		leaving_location = true;
		
		/*area_cam.SetLimits(m_current_location->GetLocationWidth(), m_current_location->GetLocationHeight());
		area_cam.SetCameraBasedOnPlayerPos(player.m_x, player.m_y);*/


		/*area_cam.m_x = player.m_x;
		area_cam.m_y = player.m_y;*/
		//std::cout << "BP\n";
	}

	//std::cout << "\n\n\n";
	//std::cout << "Player x pos: " << player.m_x << std::endl;
	//std::cout << "Player y pos: " << player.m_y << std::endl;
	//std::cout << "Camera x pos: " << area_cam.m_x << std::endl;
	//std::cout << "Camera y pos: " << area_cam.m_y << std::endl;
}

//------------------------------------------------------------------------------
void App::OnKeyDown(SDL_Event* Event) 
{
	if (!leaving_location && !entering_location)
	{
		if (m_state == STATE_ENUM::NORMAL)
		{
			if (Event->type == SDL_KEYDOWN)
			{
				if (Event->key.keysym.sym == SDLK_DOWN || Event->key.keysym.sym == SDLK_s)
				{
					player_moving_down = true;
					player_moving_up = false;
					//OnMove(0, speed);
					//player_edge_enum = static_cast<EdgeEnum>(player.IsPlayerAtEdgePosition());
				}
				else if (Event->key.keysym.sym == SDLK_UP || Event->key.keysym.sym == SDLK_w)
				{
					player_moving_up = true;
					player_moving_down = false;
					//OnMove(0, -speed);
					//player.IsPlayerAtEdgePosition();
				}
				else if (Event->key.keysym.sym == SDLK_RIGHT || Event->key.keysym.sym == SDLK_d)
				{
					player_moving_right = true;
					player_moving_left = false;
					//OnMove(speed, 0);
					//player.IsPlayerAtEdgePosition();
				}
				else if (Event->key.keysym.sym == SDLK_LEFT || Event->key.keysym.sym == SDLK_a)
				{
					player_moving_left = true;
					player_moving_right = false;
					//OnMove(-speed, 0);
					//player.IsPlayerAtEdgePosition();
				}
				if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER)
				{

					// TODO: FIX THIS SHOULD CHECK ALL THE LAYERS
					int tile_id_to_be_interacted_with = m_current_location->GetTileDirId(player.m_x + player.Col_X - 1, player.m_y + player.Col_Y - 1,
						static_cast<int>(player.m_dir));

					std::array<int, 4> ids_to_be_checked = m_current_location->GetTilesIdBasedOnPlayerDir(player.m_x + player.Col_X + player.Col_Width / 2, player.m_y + player.Col_Y + player.Col_Height / 2, static_cast<int>(player.m_dir));
					int x = 0;
					int y = 0;
					//Location::IdToXY(tile_id_to_be_interacted_with, x, y);

					//m_current_location->IdToXY(tile_id_to_be_interacted_with, x, y);

					bool has_dialogue = false;
					//uint32_t interactable_id = 0;
					// Firstly check if NPC stays on that 


					//EntityWalker::Direction player_dir = player.GetDir();
					//int scan_offset_x = 0;
					//int scan_offset_y = 0;

					//if (player_dir == EntityWalker::Direction::UP)
					//{
					//	scan_offset_y = SCALED_HERO_SPRITE_HEIGHT / 2;
					//}
					//else if (player_dir == EntityWalker::Direction::LEFT)
					//{
					//	scan_offset_x = SCALED_HERO_SPRITE_WIDTH / 2;
					//}
					//int x_map = 0, y_map = 0;
					//player.GetXY(x_map, y_map);

					//int clicked_tile_id = LocationHelper::get().GetIdBasedOnXY(x_map + scan_offset_x, y_map + scan_offset_y, m_current_location->m_tiles_per_col);

					//if (w_obj.IsInteractiveAt(tile_id_to_be_interacted_with, m_current_location))

					std::shared_ptr<IInteractive> player_interaction_target = NULL;

					std::string ids_string;

					for (const auto& id : ids_to_be_checked)
					{
						player_interaction_target = w_obj.GetInteractiveAt(id, m_current_location);

						if (player_interaction_target)
						{
							break;
						}
						ids_string += Stringify::Int(id);
						ids_string += ", ";
					}

					DebugRenderer::get().AddDebugLine("IDClicked", "Recently clicked ID:" + ids_string, App::GetInstance()->GetRenderer());


					if (player_interaction_target)
					{	
						player_interaction_target->Interact(this);
					}

					//if (w_obj.IsInteractableAt(player, has_dialogue, m_npc_currently_interacted_with, m_current_location))
					//{
					//	m_npc_currently_interacted_with->Interact(this);
					//	if (has_dialogue)
					//	{
					//		if (m_npc_currently_interacted_with->IsRotatable())
					//		{
					//			m_npc_currently_interacted_with->SetDirBasedOnPlayer(player.GetDir());
					//		}
					//		m_state = STATE_ENUM::DIALOGUE;
					//		//UpdateCurrentlyShownText(w_obj.PlayDialogue(m_npc_currently_interacted_with, m_dialogue_last_line));
					//		//m_dial_gui.SetDisplayedText(w_obj.PlayDialogue(m_npc_currently_interacted_with, m_dialogue_last_line));
					//		m_dial_controller.LoadDialog("res/scripts/dialogues/" + m_npc_currently_interacted_with->GetNpcName() + ".lua", m_npc_currently_interacted_with);
					//		m_dial_controller.StartDialog();
					//		m_dial_controller.ContinueDialog();
					//	}
					//}
				}
			}
		}
		else if (m_state == STATE_ENUM::DIALOGUE)
		{
			if (Event->type == SDL_KEYDOWN)
			{
				if (m_dial_controller.IsChoiceState())
				{
					if (Event->key.keysym.sym == SDLK_DOWN || Event->key.keysym.sym == SDLK_s)
					{
						m_dial_controller.SelectNextChoice();
					}
					else if (Event->key.keysym.sym == SDLK_UP || Event->key.keysym.sym == SDLK_w)
					{
						m_dial_controller.SelectPreviousChoice();
					}
					else if (Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER)
					{
						m_dial_controller.ContinueDialog();
					}
				}
				else
				{

					if (IsConfirmButtonPressed(Event))
					{
						if (!m_dial_controller.IsLastLine())
						{
							m_dial_controller.ContinueDialog();
							//UpdateCurrentlyShownText(w_obj.PlayDialogue(m_npc_currently_interacted_with, m_dialogue_last_line));

							//m_dial_gui.SetDisplayedText(w_obj.PlayDialogue(m_npc_currently_interacted_with, m_dialogue_last_line));
							//m_dial_controller.StartDialog();
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
		else if (m_state == STATE_ENUM::SAVE_CHOICE)
		{
			if (Event->type == SDL_KEYDOWN)
			{
				if (IsDownButtonPressed(Event))
				{
					m_window_controller->OnDownKey();
				}
				else if (IsUpButtonPressed(Event))
				{
					m_window_controller->OnUpKey();
				}
				else if (IsConfirmButtonPressed(Event))
				{
					bool close_window = false;
					m_window_controller->OnConfirm(close_window);

					if (close_window)
					{
						auto controlled_window = m_window_controller->GetControlledGUI();

						const auto found = find(m_window_guis.begin(), m_window_guis.end(), controlled_window);

						if (found != m_window_guis.end())
						{
							m_window_controller.reset();
							//m_window_guis.erase(std::remove(m_window_guis.begin(), m_window_guis.end(), found), m_window_guis.end());
							m_window_guis.erase(found);
							m_state = STATE_ENUM::NORMAL;
						}
					}
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
				player_moving_down = false;
				player.m_moving = false;
			}
			else if (Event->key.keysym.sym == SDLK_UP || Event->key.keysym.sym == SDLK_w)
			{
				player_moving_up = false;;
				player.m_moving = false;
			}
			else if (Event->key.keysym.sym == SDLK_RIGHT || Event->key.keysym.sym == SDLK_d)
			{
				player_moving_right = false;
				player.m_moving = false;
			}
			else if (Event->key.keysym.sym == SDLK_LEFT || Event->key.keysym.sym == SDLK_a)
			{
				player_moving_left = false;
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

}


//------------------------------------------------------------------------------
void App::Loop() 
{
	if (player_moving_right)
	{
		OnMove(MOVEMENT_SPEED, 0);
	}
	else if (player_moving_left)
	{
		OnMove(-MOVEMENT_SPEED, 0);
	}

	if (player_moving_up)
	{
		OnMove(0, -MOVEMENT_SPEED);
	}
	else if (player_moving_down)
	{
		OnMove(0, MOVEMENT_SPEED);
	}

	if (m_state == STATE_ENUM::DIALOGUE)
	{
		StopPlayerMovement();
	}

	if (leaving_location || entering_location)
	{
		StopPlayerMovement();
		MoveToAnotherLocationSequence();
		UpdateVisibleTexturesAlpha();
	}
	//CAppStateManager::OnLoop();
}

void App::StopPlayerMovement()
{
	player_moving_down = false;
	player_moving_up = false;
	player_moving_left = false;
	player_moving_right = false;
	player.m_moving = false;
}

//------------------------------------------------------------------------------
void App::Render() 
{
	SDL_RenderClear(Renderer);
	/*for (int r = 0; r < TILES_ROWS; ++r)
	{
		for (int c = 0; c < TILES_COLS; ++c)
		{
			TextureBank::Get("ss_mod")->Render(c * SCALED_SPRITE_WIDTH, r * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, BASE_SPRITE_WIDTH * TEST_TILE_OFF_X, 0, BASE_SPRITE_WIDTH, BASE_SPRITE_HEIGHT);
		}
	}*/
	


	//w_obj.GetLocation("start_1")->OnRender(Renderer, player.m_x, player.m_y);
	//Location::starting_loc_elements.OnRender(Renderer, player.m_x, player.m_y);

	//static int g = 0;
	//g++;
	//g = g % 256;

	m_current_location->OnRender(Renderer, area_cam.GetX(), area_cam.GetY(), fade_out_value, fade_out_value, fade_out_value);
	
	//Location::starting_loc_elements.OnRender(Renderer, area_cam.m_x, area_cam.m_y);


//	int id = 0;
//	int idx = 0;
//	int idy = 0;
//	for (auto&& p : w_obj.GetLocation("start_1")->m_tiles_arr)
//	{
//		TextureBank::Get("ss_nomargin")->Render(idx * SCALED_SPRITE_WIDTH, idy * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, p.m_tile_y * BASE_SPRITE_WIDTH, p.m_tile_x * BASE_SPRITE_HEIGHT, BASE_SPRITE_WIDTH, BASE_SPRITE_HEIGHT);
//#if TILE_COLLISION_DEBUG_RENDER
//		const SDL_Rect col_rect{ idx * SCALED_SPRITE_WIDTH, idy * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT };
//		if (p.TypeID == 2)
//		{
//			SDL_SetRenderDrawColor(Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
//		}
//
//		SDL_RenderDrawRect(Renderer, &col_rect);
//		SDL_SetRenderDrawColor(Renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
//#endif // TILE_COLLISION_DEBUG_RENDER
//		id++;
//		idx = id % TILES_COLS;
//		idy = id / TILES_COLS;
//	}
//
//	id = 0;
//	idx = 0;
//	idy = 0;
//	for (auto&& p : Location::starting_loc_elements.m_tiles_arr)
//	{
//		if (p.TypeID != TILE_TYPE_NONE)
//		{
//			TextureBank::Get("ss_nomargin")->Render(idx * SCALED_SPRITE_WIDTH, idy * SCALED_SPRITE_HEIGHT, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, p.m_tile_y * BASE_SPRITE_WIDTH, p.m_tile_x * BASE_SPRITE_HEIGHT, BASE_SPRITE_WIDTH, BASE_SPRITE_HEIGHT);
//		}
//		id++;
//		idx = id % TILES_COLS;
//		idy = id / TILES_COLS;
//	}

	//TILE_TYPE_NONE

	//const SDL_Rect hmm{ idx * SCALED_SPRITE_WIDTH, idy-1 * SCALED_SPRITE_HEIGHT, 150, 150 };
	////SDL_SetRenderDrawColor(Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(Renderer, &hmm);
	
	if (fade_out_value != 255)
	{
		TextureBank::Get("char_ss")->setColor(fade_out_value, fade_out_value, fade_out_value);
	}

	int player_screen_x = SCREEN_WIDTH / 2;

	if (player.m_x < player_screen_x)
	{
		player_screen_x = player.m_x;
	}
	else if (player.m_x > (m_current_location->GetLocationWidth() - SCREEN_WIDTH / 2))
	{
		// SCREEN_WIDTH IS ASSUMED TO BE CAMERA_WIDTH
		player_screen_x = SCREEN_WIDTH - (m_current_location->GetLocationWidth() - player.m_x);
	}
	int player_screen_y = SCREEN_HEIGHT / 2;

	if (player.m_y < player_screen_y)
	{
		player_screen_y = player.m_y;
	}
	else if(player.m_y > (m_current_location->GetLocationHeight() - SCREEN_HEIGHT / 2))
	{
		player_screen_y = SCREEN_HEIGHT - (m_current_location->GetLocationHeight() - player.m_y);
	}
	
	SDL_Point sprite_coords = player.GetSpriteCoordinates();
	/*const SDL_Rect rect{ player.m_x, player.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
	SDL_RenderDrawRect(Renderer, &rect);*/


//#if TILE_COLLISION_DEBUG_RENDER
	

//#endif // TILE_COLLISION_DEBUG_RENDER

	
	//const SDL_Rect mid_rect{ SCREEN_WIDTH / 2 -10, SCREEN_HEIGHT / 2 - 10, 20, 20};
	//SDL_SetRenderDrawColor(Renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
	//SDL_RenderDrawRect(Renderer, &mid_rect);

	//for (auto&& npc : w_obj.GetNpcArr())
	//for (auto&& npc : w_obj.GetNpcArr())
	for (auto&& npc : w_obj.GetNpcLocationVector(m_current_location))
	{
		npc->OnRender(Renderer);
		//npc->DrawBoundingRect(Renderer);
	}

	if (player.m_moving)
	{
		TextureBank::Get("char_ss")->Render(player_screen_x, player_screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH * player.player_anim.GetCurrentFrame(), sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
		//TextureBank::Get("char_ss")->Render(player.m_x, player.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH * player.player_anim.GetCurrentFrame(), sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	}
	else
	{
		TextureBank::Get("char_ss")->Render(player_screen_x, player_screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
		//TextureBank::Get("char_ss")->Render(player.m_x, player.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	}

	if (m_render_player_collision)
	{
		//const SDL_Rect col_rect{ player.m_x + player.Col_X, player.m_y + player.Col_Y, player.Col_Width, player.Col_Height };
		const SDL_Rect col_rect{ player_screen_x + player.Col_X, player_screen_y + player.Col_Y, player.Col_Width, player.Col_Height };
		SDL_SetRenderDrawColor(Renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Renderer, &col_rect);
		SDL_SetRenderDrawColor(Renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		const SDL_Rect bound_rect{ player_screen_x, player_screen_y , SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
		SDL_RenderDrawRect(Renderer, &bound_rect);
	}
	
	if (m_state == STATE_ENUM::DIALOGUE)
	{
		/*TextureBank::Get("dialogue_frame")->Render(380, 850);
		text_texture.Render(400, 900);*/

		m_dial_gui.OnRender();
	}

	for (auto&& window_gui : m_window_guis)
	{
		window_gui->Render();
	}
	   
	//for (int i = 0; i < w_obj.GetNpcArr().size(); i++) {
	//	if (!CEntity::EntityList[i]) continue;

	//	CEntity::EntityList[i]->OnRender(Surf_Display);
	//}


	//TextureBank::Get("char_ss")->Render(npc.m_x, npc.m_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, 4 * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);

	//CAppStateManager::OnRender();

#if FPS_DEBUG_RENDER
	DebugRender();

	//std::shared_ptr<Texture> fps_texture = std::make_shared<Texture>();
	//if (!m_font)
	//{
	//	std::cout << "Missing font in DEBUG fps render ... \n";
	//}
	//fps_texture->loadFromRenderedText(Renderer, "FPS: " + Stringify::Int(fps), { 0,0,0 }, m_debug_font);
	//fps_texture->Render(50, 50);

#endif



	SDL_RenderPresent(Renderer);            // glowna funkcja resetujaca obraz
}

void App::OrderedRender()
{
	SDL_RenderClear(Renderer);
	int player_standing_on_id = player.GetTopLeftSpriteTileId();
	
	// Cleanup ?
	int player_screen_x = SCREEN_WIDTH / 2;

	if (player.m_x < player_screen_x)
	{
		player_screen_x = player.m_x;
	}
	else if (player.m_x > (m_current_location->GetLocationWidth() - SCREEN_WIDTH / 2))
	{
		// SCREEN_WIDTH IS ASSUMED TO BE CAMERA_WIDTH
		player_screen_x = SCREEN_WIDTH - (m_current_location->GetLocationWidth() - player.m_x);
	}
	int player_screen_y = SCREEN_HEIGHT / 2;

	if (player.m_y < player_screen_y)
	{
		player_screen_y = player.m_y;
	}
	else if (player.m_y > (m_current_location->GetLocationHeight() - SCREEN_HEIGHT / 2))
	{
		player_screen_y = SCREEN_HEIGHT - (m_current_location->GetLocationHeight() - player.m_y);
	}

	SDL_Point sprite_coords = player.GetSpriteCoordinates();

	m_current_location->OnRenderBaseLayer(Renderer, area_cam.GetX(), area_cam.GetY(), fade_out_value, fade_out_value, fade_out_value);


	//int id = top_left_id + col_id;
	//id = id + row_id * m_current_location->m_tiles_per_col;


	for (int id : m_currently_drawn_tile_ids)
	{
		m_current_location->RenderTile(Renderer, id, area_cam.GetTopLeftPos());
		if (player_standing_on_id == id)
		{			
			if (m_render_player_collision)
			{
				//const SDL_Rect col_rect{ player.m_x + player.Col_X, player.m_y + player.Col_Y, player.Col_Width, player.Col_Height };
				const SDL_Rect col_rect{ player_screen_x + player.Col_X, player_screen_y + player.Col_Y, player.Col_Width, player.Col_Height };
				SDL_SetRenderDrawColor(Renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderDrawRect(Renderer, &col_rect);
				SDL_SetRenderDrawColor(Renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
				const SDL_Rect bound_rect{ player_screen_x, player_screen_y , SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
				SDL_RenderDrawRect(Renderer, &bound_rect);
			}
		}
	}
	//Refactor
	if (player.m_moving)
	{
		TextureBank::Get("char_ss")->Render(player_screen_x, player_screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH * player.player_anim.GetCurrentFrame(), sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	}
	else
	{
		TextureBank::Get("char_ss")->Render(player_screen_x, player_screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	}

	for (auto&& npc : w_obj.GetNpcLocationVector(m_current_location))
	{
		npc->OnRender(Renderer);
		//npc->DrawBoundingRect(Renderer);
	}

	//for (auto&& loc_element_layer : m_tiles_layers)
	//{
	//	for (int row_id = 0; row_id < rows_count; ++row_id)
	//	{
	//		for (int col_id = 0; col_id < cols_count; ++col_id)
	//		{
	//			int id = top_left_id + col_id;
	//			id = id + row_id * m_current_location->m_tiles_per_col;
	//			m_current_location->RenderTile(Renderer, id, m_top_left);

	//			if (player_standing_on_id == id)
	//			{
	//				if (player.m_moving)
	//				{
	//					TextureBank::Get("char_ss")->Render(player_screen_x, player_screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH * player.player_anim.GetCurrentFrame(), sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	//				}
	//				else
	//				{
	//					TextureBank::Get("char_ss")->Render(player_screen_x, player_screen_y, SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, sprite_coords.y * HERO_SPRITE_HEIGHT, HERO_SPRITE_WIDTH, HERO_SPRITE_HEIGHT);
	//				}

	//				if (m_render_player_collision)
	//				{
	//					//const SDL_Rect col_rect{ player.m_x + player.Col_X, player.m_y + player.Col_Y, player.Col_Width, player.Col_Height };
	//					const SDL_Rect col_rect{ player_screen_x + player.Col_X, player_screen_y + player.Col_Y, player.Col_Width, player.Col_Height };
	//					SDL_SetRenderDrawColor(Renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
	//					SDL_RenderDrawRect(Renderer, &col_rect);
	//					SDL_SetRenderDrawColor(Renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
	//					const SDL_Rect bound_rect{ player_screen_x, player_screen_y , SCALED_HERO_SPRITE_WIDTH, SCALED_HERO_SPRITE_HEIGHT };
	//					SDL_RenderDrawRect(Renderer, &bound_rect);
	//				}
	//			}
	//		}
	//	}
	//}

	if (m_state == STATE_ENUM::DIALOGUE)
	{
		m_dial_gui.OnRender();
	}

	for (auto&& window_gui : m_window_guis)
	{
		window_gui->Render();
	}



#if FPS_DEBUG_RENDER
	DebugRender();
#endif
	SDL_RenderPresent(Renderer);
}


void App::DebugRender()
{
	int fps = CFPS::FPSControl.GetFPS();

	DebugRenderer::get().AddDebugLine("FPS", "FPS: " + Stringify::Int(fps), Renderer);
	DebugRenderer::get().AddDebugLine("PlayerX", "Player X: " + Stringify::Int(player.m_x), Renderer);
	DebugRenderer::get().AddDebugLine("PlayerY", "Player Y: " + Stringify::Int(player.m_y), Renderer);

	DebugRenderer::get().AddDebugLine("area_cam.m_x", "area_cam.m_x: " + Stringify::Int(area_cam.GetX()), Renderer);
	DebugRenderer::get().AddDebugLine("area_cam.m_y", "area_cam.m_y: " + Stringify::Int(area_cam.GetY()), Renderer);
	DebugRenderer::get().AddDebugLine("CollCount", "Coll count: " + Stringify::Int(m_current_location->m_collisions_vector.size()), Renderer);
	DebugRenderer::get().AddDebugLine("InteractivesSize", "Interactive SizE: " + Stringify::Int(w_obj.GetInteractableVector(m_current_location).size()), Renderer);
	DebugRenderer::get().AddDebugLine("m_currently_drawn_tile_ids", "m_currently_drawn_tile_ids SizE: " + Stringify::Int(m_currently_drawn_tile_ids.size()), Renderer);


	for (auto&& npc : w_obj.GetNpcLocationVector(m_current_location))
	{
		DebugRenderer::get().AddDebugLine(npc->GetNpcName(), npc->GetNpcName() + ": X: " + Stringify::Int(npc->m_x) + " Y: " + Stringify::Int(npc->m_y) + " Tile ID: " + Stringify::Int(npc->GetCorrespondingTileIds()), Renderer);
	}


	DebugRenderer::get().OnRender(Renderer);
}

void App::UpdateCurrentlyDrawnTilesId()
{
	int top_left_id = area_cam.GetTopLeftID();
	int top_right_id = area_cam.GetTopRightID();
	int bottom_right_id = area_cam.GetBottomRightID();
	int bottom_left_id = area_cam.GetBottomLeftID();

	//int tile_offset_x = left % SCALED_SPRITE_WIDTH;
	//int tile_offset_y = top % SCALED_SPRITE_HEIGHT;

	int cols_count = top_right_id - top_left_id;
	if (m_current_location)
	{
		//int rows_count = ceil((bottom_right_id - top_left_id) / m_current_location->m_tiles_per_col);
		int rows_count = (bottom_right_id - top_left_id + m_current_location->m_tiles_per_col - 1) / m_current_location->m_tiles_per_col;


		std::set<int> tile_ids_to_be_drawn;
		std::set<int> overlaping_ids;


		for (int row_id = 0; row_id < rows_count; ++row_id)
		{
			for (int col_id = 0; col_id < cols_count; ++col_id)
			{
				int id = top_left_id + col_id;
				id = id + row_id * m_current_location->m_tiles_per_col;
				tile_ids_to_be_drawn.insert(id);
			}
		}

		for (int i = 0; i < m_current_location->m_tiles_layers.size(); ++i)
		{
			overlaping_ids = LookupTables::Get().GetAllOverlapingIdsByLayerAndRange(i, tile_ids_to_be_drawn);

			tile_ids_to_be_drawn.insert(overlaping_ids.begin(), overlaping_ids.end());
		}

		m_currently_drawn_tile_ids = tile_ids_to_be_drawn;
	}
}

void App::UpdateVisibleCornersCoordinates()
{
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

	//Free  fonts
	TTF_CloseFont(m_font);
	m_font = nullptr;

#if FPS_DEBUG_RENDER
	TTF_CloseFont(m_debug_font);
	m_debug_font = nullptr;
#endif

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

	m_dial_gui.Cleanup();
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
#if FPS_DEBUG_RENDER
	if (!(m_debug_font = TTF_OpenFont(font3.c_str(), 26)))
	{
		std::cerr << "Error: Unable to open font" << std::endl;
		return;
	}
#endif

	/*TTF_SetFontHinting(m_font, 13);*/

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
	currently_displayed_line;

	//text_solid = TTF_RenderText_Solid(font, text.c_str(), red);


	m_dial_gui.Init(Renderer, fontfile, "kanapka", "dialogue_frame");
	m_dial_gui.SetCoords(400, 900, 0.6 * SCREEN_WIDTH, 0.18 * SCREEN_HEIGHT);
}

//------------------------------------------------------------------------------
int App::Execute(int argc, char* argv[]) 
{
	/*auto& config = Config::GetInstance();
	config.tuning_config.ParseTuningConfig(config.m_ConfigFilename);*/

	if (!Init()) return 0;

	SDL_Event Event;
	
	const int FPS = 60;
	const int frame_delay = 1000 / FPS;

	Uint32 frame_start;
	int frame_time;

	while (Running) {
		frame_start = SDL_GetTicks();
		while (SDL_PollEvent(&Event) != 0) {
			OnEvent(&Event);

		}
		const Uint8* keystates = SDL_GetKeyboardState(NULL);

		//if (keystates[SDL_SCANCODE_DOWN])
		//{
		//	OnMove(0, 5);
		//	//player_edge_enum = static_cast<EdgeEnum>(player.IsPlayerAtEdgePosition());
		//}
		//else if (keystates[SDL_SCANCODE_UP])
		//{
		//	OnMove(0, -5);
		//	//player.IsPlayerAtEdgePosition();
		//}
		//else if (keystates[SDL_SCANCODE_RIGHT])
		//{
		//	OnMove(5, 0);
		//	//player.IsPlayerAtEdgePosition();
		//}
		//else if (keystates[SDL_SCANCODE_LEFT])
		//{
		//	OnMove(-5, 0);
		//	//player.IsPlayerAtEdgePosition();
		//}

		Loop();		// Update 60 times per second ?
		
		//Render();
		OrderedRender();

		//SDL_Delay(1); // Breath
		CFPS::FPSControl.OnLoop();

		frame_time = SDL_GetTicks() - frame_start;

		if (frame_delay > frame_time)
		{
			SDL_Delay(frame_delay - frame_time); // Breath
		}

		//char Buffer[255];
		//sprintf(Buffer, "%d", CFPS::FPSControl.GetFPS());
		//std::cout << CFPS::FPSControl.GetFPS() << std::endl;
		//SDL_WM_SetCaption(Buffer, Buffer);
	}

	Cleanup();

	return 1;
}

bool App::IsConfirmButtonPressed(SDL_Event* Event) const
{
	return Event->key.keysym.sym == SDLK_RETURN || Event->key.keysym.sym == SDLK_KP_ENTER;
}

bool App::IsUpButtonPressed(SDL_Event* Event) const
{
	return Event->key.keysym.sym == SDLK_UP || Event->key.keysym.sym == SDLK_w;
}

bool App::IsDownButtonPressed(SDL_Event* Event) const
{
	return Event->key.keysym.sym == SDLK_DOWN || Event->key.keysym.sym == SDLK_s;
}

void App::SetLocation(std::shared_ptr<Location> current_location)
{
	m_current_location = current_location;
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
	Log::Init();
	CORE_WARN("Initialized log!");
	auto& config = Config::GetInstance();
	config.tuning_config.ParseTuningConfig(config.m_ConfigFilename);
	int a = 14;
	INFO("Hello! Var={0}", a);
	if (!PrimarySDLInit())
	{
		return false;
	}
	// Load all of our Textures (see TextureBank class for expected folder)
	if (TextureBank::Init() == false) {
		//Log("Unable to init TextureBank");
		return false;
	}


	FontInit();
	
	//m_window_guis.back()->LoadText("gowno", { X_OFFSET_TO_FRAME_BORDER, HEIGHT_OF_SINGLE_LINE });

#if DEBUG_MODE
	int x0 = SET_PLAYER_X0();
	int y0 = SET_PLAYER_Y0();

	player.SetPosition(x0, y0);

	STARTING_LOCATION_FILENAME_NO_EXT = SET_STARTING_LOCATION();
	STARTING_LOCATION_PATH = "res/map/" + STARTING_LOCATION_FILENAME_NO_EXT + ".lua";

	m_render_player_collision = DEBUG_RENDER_PLAYER_COLLISION();
	DebugRenderer::get().SetFont(m_debug_font);

#endif

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

	//w_obj.GetLocation("start_1")->OnLoad("res/map/map3.txt");
	//Location::starting_loc_elements.OnLoad("res/map/map3_addons.txt");
	//if (!Location::starting_loc_elements.OnLoadLua("res/map/start_1.lua", 2) || !m_current_location->OnLoadLua("res/map/start_1.lua", 1))


	LoadAllEntrancePoints();
	if (!LoadLocationsResources())
	{
		// FATAL
		ERROR("Failed to load locations resources");
		return false;
	}

	//if (!Location::starting_loc_elements.OnLoadLua(STARTING_LOCATION_PATH, 2) || !m_current_location->OnLoadLua(STARTING_LOCATION_PATH, 1))
	//{
	//	return false;
	//}


	m_current_location->LoadWaypointsLua(STARTING_LOCATION_PATH);

	area_cam.SetLimits(m_current_location->GetLocationWidth(), m_current_location->GetLocationHeight(), m_current_location->m_tiles_per_col);
	area_cam.SetCameraBasedOnPlayerPos(player.m_x, player.m_y);

	//for (auto&& npc : w_obj.GetNpcArr())



	//NPC* los = new NPC{ 400, 400, 0, 1, 4 };
	//EntityWalker::m_entity_arr.emplace_back(los);
	
	TextureBank::Get("dialogue_frame")->setAlpha(191);
	TextureBank::Get("dialogue_frame")->setBlendMode(SDL_BLENDMODE_BLEND);



	for (auto&& npc : w_obj.GetNpcLocationVector(m_current_location))
	{
		npc->SetCamera(&area_cam);
		npc->SetDebugFont(m_font);
	}

	m_dial_controller.SetDialogGui(&m_dial_gui);

	//EntityWalker::m_entity_arr.emplace_back(NPC{1000, 400, 0, 4, 0});

		/*npc_arr.push_back(std::make_unique<NPC>(400, 400, 0, 1, 4));
	npc_arr.push_back(std::make_unique<NPC>(1000, 400, 0, 4, 0));*/

	//CAppStateManager::SetActiveAppState(APPSTATE_DIALOGUE_TEST, Renderer);

	//ReadMap();

	return true;
}

bool App::PrimarySDLInit()
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

	SCREEN_WIDTH = current.w;
	SCREEN_HEIGHT = current.h;

	//title = "My SDL Game";
	if ((Window = SDL_CreateWindow(
		"Place of Tomorrow",
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
	return true;
}

void App::MoveToAnotherLocationSequence()
{
	if (leaving_location)
	{
		fade_out_value -= fade_out_inc;
		if (fade_out_value == 0)
		{
			leaving_location = false;

			player.SetLocation(entered_loc);
			player.SetPosition(loc_x, loc_y);
			SetLocation(entered_loc);

			area_cam.SetLimits(m_current_location->GetLocationWidth(), m_current_location->GetLocationHeight(), m_current_location->m_tiles_per_col);
			area_cam.SetCameraBasedOnPlayerPos(player.m_x, player.m_y);

			entering_location = true;
		}
	}
	else if (entering_location)
	{
		fade_out_value += fade_out_inc;
		if (fade_out_value == 255)
		{
			entering_location = false;
		}
	}

}

bool App::LoadLocationsResources()
{
	return w_obj.LoadResources();
}

void App::UpdateVisibleTexturesAlpha()
{
	TextureBank::Get("char_ss")->setColor(fade_out_value, fade_out_value, fade_out_value);
	TextureBank::Get("ss_nomargin")->setColor(fade_out_value, fade_out_value, fade_out_value);
}

bool App::LoadAllEntrancePoints()
{
	for (const auto& entry : std::filesystem::directory_iterator(MAP_RESOURCE_LOCATION))
	{
//		std::cout << entry.path().extension() << std::endl;
		if (entry.path().extension() == ".lua")
		{
			auto loc = std::make_shared<Location>(entry.path().stem().string());
			w_obj.AddEmptyLocation(loc);
			LoadWaypointsLua(entry.path().string());
			//std::cout << entry.path().extension() << std::endl;


		}
	}
	w_obj.AssociateEntranceToEndpoint();
	w_obj.AssociateEntrypointsToLocations();
	w_obj.AssociateEndpointsToLocations();
	m_current_location = w_obj.GetLocation(STARTING_LOCATION_FILENAME_NO_EXT);
	//m_current_location = w_obj.GetLocation("start_1");
	player.SetLocation(m_current_location);

	return false;
}

// 04082020
//bool App::LoadWaypointsLua(char* filename)
bool App::LoadWaypointsLua(const std::string& filename)
{
	static const char LUA_MAP_HEADER[] = "abc";
	static const char LUA_LAYERS[] = "layers";
	static const char LUA_PROPERTIES[] = "properties";
	static const char LUA_OBJECTS[] = "objects";

	bool success = true;
	if (std::filesystem::exists(filename))
	{
		//std::cout << std::filesystem::path(filename).stem() << std::endl;
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(filename);

		// Step 1 find id coressponding to object layer
		// In order to do so iterate over layers checking if optional is true

		int id = 1;

		sol::optional<sol::table> conf = lua[LUA_MAP_HEADER][LUA_LAYERS][id];

		while (conf)
		{
			std::string type = lua[LUA_MAP_HEADER][LUA_LAYERS][id]["type"];
			if (type == "objectgroup")
			{
				//std::cout << "objectgroup @ " << id << std::endl;

				int object_id = 1;
				sol::optional<sol::table> object_group_objects = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id][LUA_PROPERTIES];
				while (object_group_objects)
				{
					for (const auto& key_value_pair : object_group_objects.value())
					{
						sol::object key = key_value_pair.first;
						sol::object value = key_value_pair.second;
						std::string k = key.as<std::string>();
						sol::type t = value.get_type();

						if (k == "GoTo")
						{
							//std::cout << "GoTo:" << value.as < std::string>() << std::endl;
							sol::optional<float> wp_y = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id]["y"];
							sol::optional<float> wp_x = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id]["x"];
							sol::optional<float> wp_width = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id]["width"];
							sol::optional<float> wp_height = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id]["height"];

							int x = int(SPRITE_SCALE) * wp_x.value();
							int y = int(SPRITE_SCALE) * wp_y.value();
							int w = int(SPRITE_SCALE) * wp_width.value();
							int h = int(SPRITE_SCALE) * wp_height.value();

							//if (wp_y)
							//{
							//	std::cout << " 6 * float wp_y = " << int(SPRITE_SCALE) * wp_y.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_y.value() " << std::endl;
							//}

							//if (wp_x)
							//{
							//	std::cout << "6 *  float wp_x = " << int(SPRITE_SCALE) * wp_x.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_x.value() " << std::endl;
							//}

							//if (wp_width)
							//{
							//	std::cout << " 6 * float wp_width = " << int(SPRITE_SCALE) * wp_width.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_width.value() " << std::endl;
							//}

							//if (wp_height)
							//{
							//	std::cout << "6 *  float wp_height = " << int(SPRITE_SCALE) * wp_height.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_height.value() " << std::endl;
							//}
							//ExitWaypoint(int x, int y, int width, int height) : m_x0(x), m_y0(y), m_width(width), m_height(height)
							ExitWaypoint exit_wp(std::filesystem::path(filename).stem().string(), value.as < std::string>(), x, y, w, h);
							w_obj.AddExitWaypoint(exit_wp);
						}
						else if (k == "Entrance")
						{
							//std::cout << "Entrance:" << value.as < std::string>() << std::endl;
							sol::optional<float> wp_y = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id]["y"];
							sol::optional<float> wp_x = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id]["x"];

							//if (wp_y)
							//{
							//	std::cout << " 6 * float wp_y = " << int(SPRITE_SCALE) * wp_y.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_y.value() " << std::endl;
							//}

							//if (wp_x)
							//{
							//	std::cout << "6 *  float wp_x = " << int(SPRITE_SCALE) * wp_x.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_x.value() " << std::endl;
							//}

							EntranceWaypoint entry_wp(std::filesystem::path(filename).stem().string(), value.as < std::string>(), static_cast<int>(SPRITE_SCALE * wp_x.value()), static_cast<int>(SPRITE_SCALE * wp_y.value()));
							w_obj.AddEntranceWaypoint(entry_wp);
						}
					}
					object_id++;
					sol::optional<sol::table> tmp = lua[LUA_MAP_HEADER][LUA_LAYERS][id][LUA_OBJECTS][object_id][LUA_PROPERTIES];
					object_group_objects = tmp;
				}
				//else
				//{
				//	std::cout << " Missing object_group waypoint properties !\n";
				//}

				//break;
			}
			id++;
			sol::optional<sol::table> conf2 = lua[LUA_MAP_HEADER][LUA_LAYERS][id];
			conf = conf2;
		}
	}
	else
	{
		std::cout << " File " << filename << " does not exist !\n";
		success = false;
	}

	return success;
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

