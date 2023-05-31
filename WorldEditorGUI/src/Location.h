#pragma once

#include <SDL.h>
#include <vector>
#include <sol.hpp>

#include "CTile.h"
#include "ExitWaypoint.h"
#include "LocationStruct.h"
#include "LocationElements.h"


//public:
//	enum EdgeEnum
//	{
//		NONE = 0x00,
//		LEFT = 0x01,
//		RIGHT = 0x02,
//		TOP = 0x04,
//		BOTTOM = 0x08,
//		TOP_LEFT = 0x05,
//		TOP_RIGHT = 0x03,
//		BOTTOM_LEFT = 0x09,
//		BOTTOM_RIGHT = 0x0A
//	};

class PlayerWalker;

class Location : public LocationStruct
{

public:
	static Location starting_loc;
	static Location starting_loc_elements;

//public:
//	std::vector<CTile> m_tiles_arr;
//
//private:
//	int m_bottom_limit;
//	int m_right_limit;
//	int m_tiles_per_col;
//	std::string m_location_name;
private:
	std::vector<ExitWaypoint> m_exit_waypoints;
	std::vector< LocationElements> m_location_elements;
	
	// Currently only one spritesheet per location is supported
	std::string spritesheet_name;

	// debug only vars
	bool m_render_collision_boxes;
	bool m_render_tiles_grid;

	// map used to determine id sprite offset according to lua extracted tiles values
	std::unordered_map<int, int> id_to_offset;
	// map used to link sprites lua id to spritesheet name
	std::unordered_map<int, std::string> lua_id_to_spritesheet_name;

public:
	Location();
	Location(const std::string location_name);

public:
	bool OnLoad(char* File);
	void OnRender(SDL_Renderer* renderer, int cam_x, int cam_y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);
	void OnRenderBaseLayer(SDL_Renderer* renderer, int cam_x, int cam_y, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255);

	void RenderTile(SDL_Renderer* renderer, int tile_id, const SDL_Point& top_left);


	void PrepareLocationSpecificData(const int spritesheet_tiles_count, const int first_spritesheet_gid, const std::string& spritesheet_name);
	int GetFirstGIDBasedOnSpritesID(const int sprites_id) const;
	const std::string GetSpritesheetNameBasedOnSpritesId(const int sprites_id) const;

	bool OnLoadLua(const std::string& File, int tiles_layer_id);
	bool LoadWaypointsLua(const std::string& File);
	//bool HasPlayerEnteredWaypoint(int x, int y);
	bool HasPlayerEnteredWaypoint(PlayerWalker& player, std::shared_ptr<Location>& entered_loc, int& loc_x, int& loc_y);
	void AddExitWaypoint(const ExitWaypoint& exit_waypoint);
	std::string GetLocationName() const;
	
	int GetLayersCount() const;

	std::shared_ptr<CTile> GetTileByLayer(int x, int y, int TILES_COL_NO, int layer_id);
	int GetLocationWidth() const;
	int GetLocationHeight() const;
	
	void InsertLocationElements(LocationElements& loc_elements);
	LocationElements& GetLocationElements(const int index);
	void SetLocationElementsVectorSize(const int size);

	void SetLocationName(const std::string& loc_name);
	void SetTilesPerCol(const int tiles_per_col);
	void SetRightLimit(const int right_limit);
	void SetBottomLimit(const int bottom_limit);

	void IdToXY(int id, int& x, int& y);
	int GetTileDirId(int x, int y, int dir);

	std::array<int, 4> GetTilesIdBasedOnPlayerDir(int x, int y, int dir);

	float GetTileId(float x, float y);
	std::vector<int> GetTileDirIds(int x, int y, int width, int height, int dir);
	unsigned int IsPlayerAtEdgePosition(int p_x, int p_y);

private:
	void MapIdToCoords(const int cols, const int id);
	void MapIdToCoords(const int cols, const int id, const int layer_id, std::vector<int>& block_ids);
	SDL_Point IdToXY(const int cols, const int id);
	
	void ExtractColNoLua(sol::state& lua, int& col_no);
	void SetBorderLimitsLua(sol::state& lua);

	// Unused remove in future:
	void SetBorderLimits(int& left, int& right, int& top, int& bottom);
};