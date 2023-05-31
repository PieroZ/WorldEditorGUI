#include "CTile.h"

CTile::CTile() 
{
	m_tile_spritesheet_x = 0;
	m_tile_spritesheet_y = 0;
	TypeID = TILE_TYPE_NONE;
	m_collider = { 0, 0, 0, 0 };
	
	m_tile_id = 0;
}

CTile::CTile(int tile_spritesheet_x, int tile_spritesheet_y, int tile_id, int type_id, const SDL_Rect& collider) : 
	m_tile_spritesheet_x(tile_spritesheet_x), m_tile_spritesheet_y(tile_spritesheet_y), m_tile_id(tile_id), TypeID(type_id), m_collider(collider)
{
}
