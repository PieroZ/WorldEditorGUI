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
	int     TypeID;

	SDL_Rect m_collider;
	//CAnimation m_tile_animation;

public:
	CTile();
};

#endif