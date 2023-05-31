#ifndef _CTILE_H_
#define _CTILE_H_
#include "SDL.h"
//#include "CAnimation.h"


enum {
	TILE_TYPE_NONE = 0,

	TILE_TYPE_NORMAL,
	TILE_TYPE_BLOCK
};

class CTile {
public:
	//int     TileID;
	int     m_tile_spritesheet_x;
	int     m_tile_spritesheet_y;
	
	int		m_tile_id;
	
	// This value corresponds to lua tileset id value
	// this max value is sum of all counts of loaded tilesets elements
	int		m_sprite_id;
	int     TypeID;

	SDL_Rect m_collider;
	//CAnimation m_tile_animation;

public:
	CTile();
	CTile(int tile_spritesheet_x, int tile_spritesheet_y, int tile_id, int type_id, const SDL_Rect& collider);
};

#endif