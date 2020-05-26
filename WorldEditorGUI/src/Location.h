#pragma once

#include <SDL.h>
#include <vector>

#include "CTile.h"

class Location 
{
public:
	static Location starting_loc;
	static Location starting_loc_elements;

public:
	std::vector<CTile> m_tiles_arr;

public:
	Location();

public:
	bool OnLoad(char* File);
	bool OnLoadLua(char* File);
	CTile* GetTile(int x, int y);
	static void IdToXY(int id, int& x, int& y);
	int GetTileDirId(int x, int y, int dir);
	std::vector<int> GetTileDirIds(int x, int y, int width, int height, int dir);

private:
	void MapIdToCoords(const int cols, const int id);
	void MapIdToCoords(const int cols, const int id, std::vector<int>& block_ids);
};