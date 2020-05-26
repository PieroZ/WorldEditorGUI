#ifndef _CTILE_H_
#define _CTILE_H_



enum {
	TILE_TYPE_NONE = 0,

	TILE_TYPE_NORMAL,
	TILE_TYPE_BLOCK
};

class CTile {
public:
	//int     TileID;
	int     m_tile_x;
	int     m_tile_y;
	int     TypeID;

public:
	CTile();
};

#endif