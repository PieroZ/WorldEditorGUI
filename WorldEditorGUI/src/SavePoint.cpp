#include "SavePoint.h"
#include "Log.h"
#include "App.h"
#include "SaveGameController.h"
#include <memory>

SavePoint::SavePoint()
{
}

SavePoint::SavePoint(int tile_spritesheet_x, int tile_spritesheet_y, int tile_id, int type_id, const SDL_Rect& collider) : CTile(tile_spritesheet_x, tile_spritesheet_y, tile_id, type_id, collider)
{
}

//SavePoint::SavePoint(std::shared_ptr<CTile> tile)
//{
//	SavePoint(tile->m_tile_spritesheet_x, tile->m_tile_spritesheet_y, tile->m_tile_id, tile->TypeID, tile->m_collider);
//}

void SavePoint::Interact(App* app)
{
	INFO("Interacted with save point !");


	int w = SCREEN_WIDTH / 5;
	int h = SCREEN_HEIGHT / 5;

	int x = SCREEN_WIDTH / 2 - w / 2;
	int y = SCREEN_HEIGHT / 2 - h / 2;

	app->StopPlayerMovement();
	app->m_window_guis.emplace_back(std::make_shared<WindowGUI>(App::GetInstance()->GetRenderer(), app->m_font, SDL_Rect{ x, y, w, h }));
	app->m_window_guis.back()->LoadCenteredText("Do you want to save the game?");
	//app->m_window_guis.back()->LoadBottomLeftText("No", X_OFFSET_TO_FRAME_BORDER, HEIGHT_OF_SINGLE_LINE);
	app->m_window_guis.back()->AppendCenteredBottomText("Yes");
	app->m_window_guis.back()->AppendCenteredBottomText("No");
	


	std::shared_ptr<SaveGameController> save_game_controller = std::make_shared<SaveGameController>(app->m_window_guis.back());
	
	app->m_window_controller = save_game_controller;

	app->m_state = App::STATE_ENUM::SAVE_CHOICE;
	app->m_window_guis.back()->SetHighlightedText(2);
}

int SavePoint::GetCorrespondingTileIds() const
{
	return m_tile_id;
}
