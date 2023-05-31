#include "EntityWalker.h"
#include "TextureBank.h"
#include "App.h"
#include "LocationHelper.h"

std::vector<EntityWalker*>     EntityWalker::m_entity_arr;

//EntityWalker::EntityWalker(App& world, std::shared_ptr<Location> occupied_location, int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y) : m_world(world), m_occupied_location(occupied_location), m_id(id), m_sprite_x(sprite_x), m_sprite_y(sprite_y)
EntityWalker::EntityWalker(std::shared_ptr<Location> occupied_location, int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y) : IInteractive(occupied_location), m_id(id)//, m_sprite_x(sprite_x), m_sprite_y(sprite_y)
{
	m_x = x;
	m_y = y;
}
//EntityWalker::EntityWalker(App& world, int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y) : m_world(world), m_id(id), m_sprite_x(sprite_x), m_sprite_y(sprite_y)
EntityWalker::EntityWalker(int x, int y, int id, uint8_t sprite_x, uint8_t sprite_y) : m_id(id)//, m_sprite_x(sprite_x), m_sprite_y(sprite_y)
{
	m_x = x;
	m_y = y;
}

EntityWalker::EntityWalker()
{
}


SDL_Point EntityWalker::GetMapCoordinates() const
{
	return { m_x, m_y };
}

void EntityWalker::SetPosition(int x, int y)
{
	m_x = x;
	m_y = y;
}

bool EntityWalker::Collides(int oX, int oY, int oW, int oH)
{
	int left1, left2;
	int right1, right2;
	int top1, top2;
	int bottom1, bottom2;


	int tX = m_x + Col_X;// + Col_X;
	int tY = m_y + Col_Y;// + Col_Y;

	left1 = tX;
	left2 = oX;

	right1 = left1 + Col_Width - 1;// - Col_Width;
	right2 = oX + oW - 1;

	top1 = tY;
	top2 = oY;

	bottom1 = top1 + Col_Height - 1;// - Col_Height;
	bottom2 = oY + oH - 1;


	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;

	if (right1 < left2) return false;
	if (left1 > right2) return false;

	return true;
}


// Most likely unused
bool EntityWalker::IsPosValid(int NewX, int NewY)
{
	bool result = true;

	int StartX = (NewX) / SCALED_SPRITE_WIDTH;
	int StartY = (NewY) / SCALED_SPRITE_HEIGHT;

	int EndX = ((NewX) + HERO_SPRITE_WIDTH - 1) / SCALED_SPRITE_WIDTH;
	int EndY = ((NewY) + HERO_SPRITE_HEIGHT - 1) / SCALED_SPRITE_HEIGHT;

	for (int iY = StartY; iY <= EndY; iY++) {
		for (int iX = StartX; iX <= EndX; iX++) {
			//CTile* Tile = Location::starting_loc.GetTile(iX * SCALED_SPRITE_WIDTH, iY * SCALED_SPRITE_HEIGHT);
			//CTile* Tile = m_occupied_location->GetTile(iX * SCALED_SPRITE_WIDTH, iY * SCALED_SPRITE_HEIGHT);

			CTile* Tile = nullptr;

			if (PosValidTile(Tile) == false) 
			{
				result = false;
			}
		}
	}


	for (int i = 0; i < m_entity_arr.size(); i++)
	{
		if (PosValidEntity(m_entity_arr[i], NewX, NewY) == false) {
			result = false;
		}
	}


	return result;
}

bool EntityWalker::PosValidTile(CTile* Tile) 
{
	if (Tile == NULL) {
		return true;
	}

	if (Tile->TypeID == TILE_TYPE_BLOCK) {
		return false;
	}

	return true;
}

bool EntityWalker::PosValidEntity(EntityWalker* Entity, int NewX, int NewY)
{
	if (this != Entity && Entity != NULL  &&
		Entity->Collides(NewX, NewY , SCALED_HERO_SPRITE_WIDTH - 1, SCALED_HERO_SPRITE_HEIGHT - 1) == true)
	{

		//CEntityCol EntityCol;

		//EntityCol.EntityA = this;
		//EntityCol.EntityB = Entity;

		//CEntityCol::EntityColList.push_back(EntityCol);

		return false;
	}

	return true;
}

EntityWalker::Direction EntityWalker::GetDir() const
{
	return m_dir;
}
//
//// TODO:
//// Move to Interactable probably
//void EntityWalker::SetLocation(std::shared_ptr<Location> occupied_loc)
//{
//	// ? :D
//	//m_world.SetLocation(occupied_loc);
//	m_occupied_location = occupied_loc;
//}
//
//std::shared_ptr<Location> EntityWalker::GetOccupiedLocation()
//{
//	return m_occupied_location;
//}

void EntityWalker::OnRender(SDL_Renderer* renderer)
{
}

void EntityWalker::DrawBoundingRect(SDL_Renderer* renderer)
{
}

void EntityWalker::Interact(App* app)
{
}

int EntityWalker::GetCorrespondingTileIds() const
{
	int x = 0;
	int y = 0;
	if (Col_Width == 0 || Col_Height == 0)
	{
		x = m_x + SCALED_HERO_SPRITE_WIDTH / 2;
		y = m_y + SCALED_HERO_SPRITE_HEIGHT / 2;
	}
	else
	{
		x = m_x + Col_X + (Col_Width - 1)/2;
		y = m_y + Col_Y + (Col_Height - 1)/2;
	}

	return LocationHelper::get().GetIdBasedOnXY(x, y, m_occupied_location->m_tiles_per_col);
}

int EntityWalker::GetBottomRightSpriteTileId() const
{
	int x = m_x + SCALED_HERO_SPRITE_WIDTH;
	int y = m_y + SCALED_HERO_SPRITE_HEIGHT;

	return LocationHelper::get().GetIdBasedOnXY(x, y, m_occupied_location->m_tiles_per_col);
}


int EntityWalker::GetTopLeftSpriteTileId() const
{
	int x = m_x;
	int y = m_y;

	return LocationHelper::get().GetIdBasedOnXY(x, y, m_occupied_location->m_tiles_per_col);
}
