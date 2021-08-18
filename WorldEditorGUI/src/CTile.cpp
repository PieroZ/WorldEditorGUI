#include "CTile.h"

CTile::CTile() 
{
	m_tile_spritesheet_x = 0;
	m_tile_spritesheet_y = 0;
	TypeID = TILE_TYPE_NONE;
	m_collider = { 0, 0, 0, 0 };
}