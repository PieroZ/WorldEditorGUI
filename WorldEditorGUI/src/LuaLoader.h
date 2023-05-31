#pragma once

#include <sol.hpp>
#include <unordered_map>
#include "SavePoint.h"
#include <vector>
#include <set>
#include <memory>
#include "SDL.h"

class LocationStruct;

class LuaLoader
{
public:
	static bool OnLocationLoadLua(const std::string& File, int tiles_layer_id, std::shared_ptr<Location> loc, std::vector<std::shared_ptr<SavePoint>>& location_save_points);
	static void ExtractColNoLua(sol::state& lua, int layer_id, int& col_no);
	static void ExtractColsPerTileset(sol::state& lua, std::unordered_map<int, int>& tile_id_to_cols);

	static void Preprocessing(const std::string& file_path, std::shared_ptr<Location> loc);
	static void SetBorderLimitsLua(sol::state& lua, std::shared_ptr<Location> loc);
	static void SetSpritesBaseSize(sol::state& lua, const std::string& spritesheet_name, int layer_id);

	static void MapIdToCoords(const int tile_id, const int cols, const int id, std::vector<int>& block_ids, std::shared_ptr<Location> loc, const std::unordered_map<int,
		SDL_Rect>& collision_map, int layer_id, std::set<int> save_point_ids, std::vector<std::shared_ptr<SavePoint>>& location_save_points, int id_offset);
	static int GetElementLayersCount(const std::string& file_path);
	static std::set<int> GetSavePointsSpriteIds(sol::state& lua);
	static std::string PrepareSpritesheetNameToIdMap(sol::state& lua, int layer_id);

private:
	// Create a collison map for a single spritesheet in currently loaded location
	static std::unordered_map<int, SDL_Rect> CreateCollisionMap(sol::state& lua, int layer_id);
private:


	inline static const char LUA_MAP_HEADER[] = "abc";
	inline static const char LUA_LAYERS[] = "layers";
	inline static const char LUA_DATA[] = "data";
	inline static const char LUA_TILESETS[] = "tilesets";
	inline static const char LUA_TILES[] = "tiles";
	inline static const char LUA_COLUMNS[] = "columns";
	inline static const char LUA_PROPERTIES[] = "properties";
	inline static const char LUA_OBJECTS[] = "objects";
	inline static const char LUA_WIDTH[] = "width";
	inline static const char LUA_HEIGHT[] = "height";

};