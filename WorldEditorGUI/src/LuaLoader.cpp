#include <filesystem>
#include "LuaLoader.h"
#include "LocationStruct.h"
#include "Stringify.h"
#include "Defines.h"
#include "Log.h"
#include "SavePoint.h"
#include "IdToSpritesheet.h"



void LuaLoader::ExtractColNoLua(sol::state& lua, int layer_id, int& col_no)
{
	sol::optional<int> col_num_sol = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_COLUMNS];

	if (col_num_sol)
	{
		col_no = col_num_sol.value();
		//std::cout << "col_number = " << col_no << std::endl;
	}
	else
	{
		std::cout << "Unable to find col_number value in lua map" << std::endl;
	}
}

void LuaLoader::ExtractColsPerTileset(sol::state& lua, std::unordered_map<int, int>& tile_id_to_cols)
{
	sol::table tilesets_table = lua[LUA_MAP_HEADER][LUA_TILESETS];
	int tilesets_table_size = tilesets_table.size();

	for (int tileset_index = 1; tileset_index <= tilesets_table_size; tileset_index++)
	{
		sol::optional<int> sprites_count_in_location = tilesets_table[tileset_index]["tilecount"];
		sol::optional<int> tileset_columns_count = tilesets_table[tileset_index]["columns"];
		sol::optional<int> spritesheet_first_gid_optional = tilesets_table[tileset_index]["firstgid"];

		if (sprites_count_in_location && spritesheet_first_gid_optional && tileset_columns_count)
		{
			int sprites_count = sprites_count_in_location.value();
			int spritesheet_first_gid = spritesheet_first_gid_optional.value();
			int columns_count = tileset_columns_count.value();
			
			for (int tile_id = spritesheet_first_gid - 1; tile_id < spritesheet_first_gid + sprites_count - 1; ++tile_id)
			{
				tile_id_to_cols[tile_id] = columns_count;
			}
		}
		else
		{
			INFO("Unable to find col_number value in lua map");
		}
	}

}



void LuaLoader::Preprocessing(const std::string& file_path, std::shared_ptr<Location> loc)
{
	int starting_point = 0;
	if (std::filesystem::exists(file_path))
	{
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(file_path);

		// iterate over all tilesets in lua file

		sol::table tilesets_table = lua[LUA_MAP_HEADER][LUA_TILESETS];

		int tilesets_table_size = tilesets_table.size();

		for (int tileset_index = 1; tileset_index <= tilesets_table_size; tileset_index++)
		{
			sol::table tileset_table = tilesets_table[tileset_index];
			
			static const char LUA_TILESET_IMAGE_PATH[] = "image";
			static const char LUA_TILESET_COUNT[] = "tilecount";

			sol::optional<std::string> spritesheet_path = tileset_table[LUA_TILESET_IMAGE_PATH];
			sol::optional<int> tiles_count_optional = tileset_table[LUA_TILESET_COUNT];

			if (spritesheet_path && tiles_count_optional)
			{
				// .png = 4 chars
				const int DOT_PNG_TEXT_LENGTH = 4;

				const std::string& spritesheet_path_string = spritesheet_path.value();
				const std::string spritesheet_filename = spritesheet_path_string.substr(spritesheet_path_string.find_last_of("/") + 1);
				const std::string spritesheet_filename_wo_ext = spritesheet_filename.substr(0, spritesheet_filename.length() - DOT_PNG_TEXT_LENGTH);

				SetSpritesBaseSize(lua, spritesheet_filename_wo_ext, tileset_index);


				


				int tiles_count = tiles_count_optional.value();
				INFO("{0} {1} = {2}", tiles_count, spritesheet_filename, spritesheet_filename_wo_ext);

				// 26.10.2021 Issue below singleton containing location specific data
				loc->PrepareLocationSpecificData(tiles_count, starting_point, spritesheet_filename_wo_ext);
				//IdToSpritesheet::Get().AppendRangeToMap(tiles_count, starting_point, spritesheet_filename_wo_ext);
				starting_point = tiles_count;
			}



			/*for (const auto& key_value_pair : tileset_table)
			{
				sol::object key = key_value_pair.first;
				sol::object value = key_value_pair.second;

				std::string field_value = value.as<std::string>();

				std::string field_name = key.as<std::string>();
				INFO("{0} {1} = {2}", file_path, field_name, field_value);
			}*/

		}
	}
}

void LuaLoader::SetBorderLimitsLua(sol::state& lua, std::shared_ptr<Location> loc)
{
	sol::optional<int> map_width_sol = lua[LUA_MAP_HEADER][LUA_WIDTH];
	if (map_width_sol)
	{
		int map_width = map_width_sol.value();
		loc->m_tiles_per_col = map_width;
		//loc.SetTilesPerCol(map_width);


		//std::cout << "map_width = " << map_width << std::endl;
		loc->m_right_limit = map_width * SCALED_SPRITE_WIDTH;
		//loc.SetRightLimit(map_width * SCALED_SPRITE_WIDTH);
		//std::cout << "location m_right_limit = " << map_width * SCALED_SPRITE_WIDTH << std::endl;
	}
	else
	{
		std::cout << "Unable to find map_width value in lua map" << std::endl;
	}

	sol::optional<int> map_height_sol = lua[LUA_MAP_HEADER][LUA_HEIGHT];
	if (map_height_sol)
	{
		int map_height = map_height_sol.value();
		//std::cout << "map_height = " << map_height << std::endl;
		loc->m_bottom_limit = map_height * SCALED_SPRITE_HEIGHT;
		//loc.SetBottomLimit(map_height * SCALED_SPRITE_HEIGHT);
		//std::cout << "location m_bottom_limit= " << map_height * SCALED_SPRITE_HEIGHT << std::endl;
	}
	else
	{
		std::cout << "Unable to find map_height value in lua map" << std::endl;
	}
}

void LuaLoader::SetSpritesBaseSize(sol::state& lua, const std::string& spritesheet_name, int tileset_id)
{
	static const char LUA_TILESET_GRID[] = "grid";
	static const char LUA_TILESET_WIDTH[] = "width";
	static const char LUA_TILESET_HEIGHT[] = "height";

	sol::optional<int> sprites_width = lua[LUA_MAP_HEADER][LUA_TILESETS][tileset_id][LUA_TILESET_GRID][LUA_TILESET_WIDTH];
	sol::optional<int> sprites_height = lua[LUA_MAP_HEADER][LUA_TILESETS][tileset_id][LUA_TILESET_GRID][LUA_TILESET_HEIGHT];

	if (sprites_width && sprites_height)
	{
		int w = sprites_width.value();
		int h = sprites_height.value();

		IdToSpritesheet::Get().SpecifySizeInSpritesheet(spritesheet_name, w, h);
	}

}

void LuaLoader::MapIdToCoords(const int tile_id, const int cols, const int id, std::vector<int>& block_ids, std::shared_ptr<Location> loc,
	const std::unordered_map<int, SDL_Rect>& collision_map, int layer_id, std::set<int> save_point_ids, std::vector<std::shared_ptr<SavePoint>>& location_save_points,
	int id_offset)
{
	std::shared_ptr<CTile> tempTile;
	
	//if (cols == 0)
	/*{
		INFO("cols = {0}", cols);
	}*/

	if (save_point_ids.find(id - id_offset - 1) != save_point_ids.end())
	{
		tempTile = std::make_shared<SavePoint>();
	}
	else
	{
		tempTile = std::make_shared<CTile>();
	}
	if (id - id_offset <= 0)
	{
		tempTile->TypeID = TILE_TYPE_NONE;
	}
	else
	{
		tempTile->m_tile_id = tile_id;
		tempTile->m_sprite_id = id;

		tempTile->m_tile_spritesheet_x = (id - id_offset - 1) / cols;

		if (tempTile->m_tile_spritesheet_x > 250)
		{
			INFO("BREAK");
		}
		tempTile->m_tile_spritesheet_y = (id - id_offset - 1) % cols;
		//map_id get map width and % to get map_id_x, map_id_y
		tempTile->TypeID = TILE_TYPE_NORMAL;

		for (int block_id : block_ids)
		{
			if ((id - id_offset - 1) == block_id)
			{
				tempTile->TypeID = TILE_TYPE_BLOCK;
				break;
			}
		}
	}

	// If key is present
	if (collision_map.find(id - id_offset) != collision_map.end())
	{
		tempTile->TypeID = TILE_TYPE_BLOCK;
		tempTile->m_collider = collision_map.at(id - id_offset);
	}

	loc->m_tiles_layers[layer_id].emplace_back(tempTile);

	if (save_point_ids.find(id - id_offset - 1) != save_point_ids.end())
	{
		std::shared_ptr<SavePoint> saveTile = std::make_shared<SavePoint>(tempTile->m_tile_spritesheet_x, tempTile->m_tile_spritesheet_y, tempTile->m_tile_id, tempTile->TypeID, tempTile->m_collider);
		location_save_points.emplace_back(saveTile);
	}
	//loc.m_tiles_arr.emplace_back(tempTile);
	
	//CTile tempTile;
	//if (id == 0)
	//{
	//	tempTile.TypeID = TILE_TYPE_NONE;
	//}
	//else
	//{
	//	tempTile.m_tile_spritesheet_x = (id - 1) / cols;
	//	tempTile.m_tile_spritesheet_y = (id - 1) % cols;
	//	//map_id get map width and % to get map_id_x, map_id_y
	//	tempTile.TypeID = TILE_TYPE_NORMAL;

	//	for (int block_id : block_ids)
	//	{
	//		if ((id - 1) == block_id)
	//		{
	//			tempTile.TypeID = TILE_TYPE_BLOCK;
	//			break;
	//		}
	//	}
	//}

	//// If key is present
	//if (collision_map.find(id) != collision_map.end())
	//{
	//	tempTile.TypeID = TILE_TYPE_BLOCK;
	//	tempTile.m_collider = collision_map.at(id);
	//}

	//loc.m_tiles_arr.emplace_back(tempTile);
}

// This method is based on assumption that only 1 layer is floor tiles and all other tiles are elements layers
int LuaLoader::GetElementLayersCount(const std::string& file_path)
{
	int count = 1;
	if (std::filesystem::exists(file_path))
	{
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(file_path);
		sol::optional<sol::table> conf = lua[LUA_MAP_HEADER][LUA_LAYERS][count][LUA_DATA];
		while (conf)
		{
			++count;
			sol::optional<sol::table> tmp = lua[LUA_MAP_HEADER][LUA_LAYERS][count][LUA_DATA];
			conf = tmp;
			if (count > 1000)
			{
				std::cout<<"Something is very wrong, layer count is over 1000\n";
				break;
			}
		}
	}
	return count;
}



std::set<int> LuaLoader::GetSavePointsSpriteIds(sol::state& lua)
{
	std::set<int> save_points_sprite_ids;
	int lua_tiles_table_index = 1;
	sol::optional<sol::table> tiles_type = lua[LUA_MAP_HEADER][LUA_TILESETS][1][LUA_TILES][lua_tiles_table_index];

	while (tiles_type)
	{
		sol::optional<sol::table> tiles_type_properties = lua[LUA_MAP_HEADER][LUA_TILESETS][1][LUA_TILES][lua_tiles_table_index]["properties"];
		if (tiles_type_properties)
		{
			for (const auto& key_value_pair : tiles_type_properties.value())
			{
				sol::object key = key_value_pair.first;
				sol::object value = key_value_pair.second;
				std::string k = key.as<std::string>();
				sol::type t = value.get_type();

				// FIX ME TYPES CAN BE DIFFERENT THAN BLOCK:
				//if (k == "id")	block_type_ids.push_back(Stringify::ToInt(value.as<std::string>()));

				if (k == "save_point")
				{
					sol::optional<int> sprite_id = lua[LUA_MAP_HEADER][LUA_TILESETS][1][LUA_TILES][lua_tiles_table_index]["id"];
					save_points_sprite_ids.insert(sprite_id.value());
				}
				switch (t)
				{
				case sol::type::string:
				{
					std::cout << k << ": " << value.as<std::string>() << std::endl;
					break;
				}
				case sol::type::number:
				{
					auto v = value.as<double>();
					std::cout << k << ": " << v << std::endl;
					break;
				}
				default:
				{
					std::cout << "hit the default case!" << std::endl;
				}
				}
			}
		}
		++lua_tiles_table_index;
		sol::optional<sol::table> tmp = lua[LUA_MAP_HEADER][LUA_TILESETS][1][LUA_TILES][lua_tiles_table_index];
		tiles_type = tmp;

	}

	return save_points_sprite_ids;
}

std::string LuaLoader::PrepareSpritesheetNameToIdMap(sol::state& lua, int layer_id)
{
	int LUA_TILESET_ID = 1;
	int starting_point = 0;
	static const char LUA_TILESET_IMAGE_PATH[] = "image";
	static const char LUA_TILESET_COUNT[] = "tilecount";
	sol::optional<std::string> spritesheet_path = lua[LUA_MAP_HEADER][LUA_TILESETS][LUA_TILESET_ID][LUA_TILESET_IMAGE_PATH];

	std::string result;

	while (spritesheet_path)
	{
		const std::string& spritesheet_path_string = spritesheet_path.value();
		const std::string spritesheet_filename = spritesheet_path_string.substr(spritesheet_path_string.find_last_of("/") + 1);
		const std::string spritesheet_filename_wo_ext = spritesheet_filename.substr(0, spritesheet_filename.length() - 4);

		SetSpritesBaseSize(lua, spritesheet_filename_wo_ext, LUA_TILESET_ID);

		if (layer_id == LUA_TILESET_ID)
		{
			result = spritesheet_filename_wo_ext;
		}
		sol::optional<int> tiles_count = lua[LUA_MAP_HEADER][LUA_TILESETS][LUA_TILESET_ID][LUA_TILESET_COUNT];

		int count = tiles_count.value();
		IdToSpritesheet::Get().AppendRangeToMap(count, starting_point, spritesheet_filename_wo_ext);
		starting_point = count;

		++LUA_TILESET_ID;
		sol::optional<std::string> tmp = lua[LUA_MAP_HEADER][LUA_TILESETS][LUA_TILESET_ID][LUA_TILESET_IMAGE_PATH];
		spritesheet_path = tmp;
	}
	
	if(LUA_TILESET_ID == 1)
	{
		ERROR("Missing embedded tileset in lua map! ");
	}

	return result;
}

std::unordered_map<int, SDL_Rect> LuaLoader::CreateCollisionMap(sol::state& lua, int layer_id)
{
	std::unordered_map<int, SDL_Rect> collision_map;

	int lua_tiles_table_index = 1;
	sol::optional<sol::table> tiles_type = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index];

	// Iterate as long there is another element in the lua tiles table or until we find object group corresponding to the collision properties
	while (tiles_type)
	{
		const std::string lua_object_group_string = "objectGroup";
		const std::string lua_objects_string = "objects";
		int lua_collision_object_index = 1;
		sol::optional<sol::table> colission_lua_table = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index][lua_object_group_string][lua_objects_string][lua_collision_object_index];
		bool has_found_collision_properties = false;

		while (colission_lua_table)
		{
			has_found_collision_properties = true;
			SDL_Rect bounding_box{ 0,0,0,0 };
			int id = -1;

			sol::optional<int> id_lua = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index]["id"];
			sol::optional<float> x_lua = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index][lua_object_group_string][lua_objects_string][lua_collision_object_index]["x"];
			sol::optional<float> y_lua = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index][lua_object_group_string][lua_objects_string][lua_collision_object_index]["y"];
			sol::optional<float> width_lua = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index][lua_object_group_string][lua_objects_string][lua_collision_object_index]["width"];
			sol::optional<float> height_lua = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index][lua_object_group_string][lua_objects_string][lua_collision_object_index]["height"];


			if (id_lua)
			{
				id = id_lua.value() + 1;
			}
			else
			{
				ERROR("Missing id in collision bounding box property");
			}
			if (x_lua)
			{
				float fx = x_lua.value() * SPRITE_SCALE;
				bounding_box.x = static_cast<int>(fx);
			}
			else
			{
				ERROR("Missing x in collision bounding box property");
			}

			if (y_lua)
			{
				float fy = y_lua.value() * SPRITE_SCALE;
				bounding_box.y = static_cast<int>(fy);
			}
			else
			{
				ERROR("Missing y in collision bounding box property");

			}
			if (width_lua)
			{
				float fw = width_lua.value() * SPRITE_SCALE;
				bounding_box.w = static_cast<int>(fw);
			}
			else
			{
				ERROR("Missing width in collision bounding box property");
			}

			if (height_lua)
			{
				float fh = height_lua.value() * SPRITE_SCALE;
				bounding_box.h = static_cast<int>(fh);
			}
			else
			{
				ERROR("Missing height in collision bounding box property");

			}
			
			collision_map[id] = bounding_box;
			TRACE("colission tile id={0}", id);
			TRACE("colission x={0}", bounding_box.x);
			TRACE("colission y={0}", bounding_box.y);
			TRACE("colission width={0}", bounding_box.w);
			TRACE("colission height={0}", bounding_box.h);

		

			++lua_collision_object_index;
			sol::optional<sol::table> colission_tmp_table = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index][lua_object_group_string][lua_objects_string][lua_collision_object_index];
			colission_lua_table = colission_tmp_table;
		}

		//if (has_found_collision_properties)
		//{
		//	break;
		//}

		++lua_tiles_table_index;
		sol::optional<sol::table> tmp = lua[LUA_MAP_HEADER][LUA_TILESETS][layer_id][LUA_TILES][lua_tiles_table_index];
		tiles_type = tmp;
	}

	return collision_map;
}

bool LuaLoader::OnLocationLoadLua(const std::string& File, int tiles_layer_id, std::shared_ptr<Location> loc, std::vector<std::shared_ptr<SavePoint>>& location_save_points)
{
	bool success = true;
	int map_id = 0;
	if (std::filesystem::exists(File))
	{
		//loc.SetLocationName(std::filesystem::path(File).stem().string());
		loc->m_location_name = std::filesystem::path(File).stem().string();
		//std::cout << std::filesystem::path(File).stem()<<std::endl;
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(File);
		int col_number = 0;

		std::vector<int> block_type_ids;

		ExtractColNoLua(lua, tiles_layer_id, col_number);
		
		std::unordered_map<int, int> tile_id_to_cols;
		ExtractColsPerTileset(lua, tile_id_to_cols);
		SetBorderLimitsLua(lua, loc);
		
		//std::string spritesheet_name = PrepareSpritesheetNameToIdMap(lua, tiles_layer_id);

		std::unordered_map<int, SDL_Rect> collision_map = CreateCollisionMap(lua, tiles_layer_id);
		std::set<int> save_points_sprite_ids = GetSavePointsSpriteIds(lua);


		//for (int i = 1; i < 4; ++i)
		//{
		//	sol::optional<sol::table> tiles_type = lua[LUA_MAP_HEADER][LUA_TILESETS][1][LUA_TILES][i];
		//	if (tiles_type) 
		//	{
		//		for (const auto& key_value_pair : tiles_type.value())
		//		{
		//			sol::object key = key_value_pair.first;
		//			sol::object value = key_value_pair.second;
		//			std::string k = key.as<std::string>();
		//			sol::type t = value.get_type();

		//			// FIX ME TYPES CAN BE DIFFERENT THAN BLOCK:
		//			if (k == "id")	block_type_ids.push_back(Stringify::ToInt(value.as<std::string>()));
		//			switch (t)
		//			{
		//				case sol::type::string:
		//				{
		//					std::cout << k << ": " << value.as<std::string>() << std::endl;
		//					break;
		//				}
		//				case sol::type::number:
		//				{
		//					auto v = value.as<double>();
		//					std::cout << k << ": " << v << std::endl;
		//					break;
		//				}
		//				default:
		//				{
		//					std::cout << "hit the default case!" << std::endl;
		//				}
		//			}
		//		}
		//	}
		//}


		//int id_offset = IdToSpritesheet::Get().GetSpritesheetOffset(spritesheet_name);

		sol::optional<sol::table> conf = lua[LUA_MAP_HEADER][LUA_LAYERS][tiles_layer_id][LUA_DATA];
		if (conf) {
			int tile_id = 0;
			for (const auto& key_value_pair : conf.value())
			{
				sol::object key = key_value_pair.first;
				sol::object value = key_value_pair.second;
				std::string k = key.as<std::string>();
				sol::type t = value.get_type();

				int sprite_id = value.as<int>();

				// TODO: Instead of hardcoded value extract col_no from spritesheet. 
				// We don't know which spritesheet values should be extracted though.
				// Value is actually stored in lua
				
				//int id_offset = IdToSpritesheet::Get().GetFirstGidBasedOnID(tile_id);
				int id_offset = loc->GetFirstGIDBasedOnSpritesID(sprite_id);

				MapIdToCoords(tile_id, tile_id_to_cols[value.as<int>()], value.as<int>(), block_type_ids, loc, collision_map, tiles_layer_id - 1, save_points_sprite_ids, location_save_points, id_offset);
				tile_id++;

				//switch (t) {
				//case sol::type::string: {
				//	std::cout << k << ": " << value.as<std::string>() << std::endl;
				//}
				//					  break;
				//case sol::type::number: {
				//	auto v = value.as<double>();
				//	std::cout << k << ": " << v << std::endl;
				//}
				//					  break;
				//default: {
				//	std::cout << "hit the default case!" << std::endl;
				//}
				//}
			}
		}


		//for (const auto& key_value_pair : e) 
		//{
		//	sol::object key = key_value_pair.first;
		//	sol::object value = key_value_pair.second;

		//	std::cout << value.as<std::string>() << std::endl;
		//	//std::cout << value << std::endl;
		//}

	}
	else
	{
		success = false;
	}
	return success;
}
