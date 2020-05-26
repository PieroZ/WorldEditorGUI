#include "Stringify.h"
#include "Location.h"
#include "Defines.h"
#include <fstream>
#include <sstream>
#include <sol.hpp>
#include <filesystem>

Location Location::starting_loc;
Location Location::starting_loc_elements;

// TODO: Move to utils
std::string trim(const std::string& str)
{
	size_t first = str.find_first_not_of(' ');
	if (std::string::npos == first)
	{
		return str;
	}
	size_t last = str.find_last_not_of(' ');
	return str.substr(first, (last - first + 1));
}

std::vector<std::string> split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(trim(token));
	}
	return tokens;
}


Location::Location()
{
}

bool Location::OnLoad(char* filename)
{
	std::ifstream infile(filename);
	if (!infile.is_open())
	{
		printf("Cannot open infline\n");
		return false;
	}

	std::string line;

	while (std::getline(infile, line))
	{
		std::vector<std::string> ids_row = split(line, ',');

		for (auto&& i : ids_row)
		{
			MapIdToCoords(57, Stringify::ToInt(i));
			//MapIdToCoords(4, Stringify::ToInt(i));
			//printf("%s\n", i.c_str());
		}
	}
	return true;
}

bool Location::OnLoadLua(char* File)
{
	if (std::filesystem::exists(File))
	{
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(File);

		std::vector<int> block_type_ids;

		for (int i = 1; i < 4; ++i)
		{
			sol::optional<sol::table> tiles_type = lua["abc"]["tilesets"][1]["tiles"][i];
			if (tiles_type) {
				int skip = 0;
				for (const auto& key_value_pair : tiles_type.value())
				{
					sol::object key = key_value_pair.first;
					sol::object value = key_value_pair.second;
					std::string k = key.as<std::string>();
					sol::type t = value.get_type();

					if(skip==1)	block_type_ids.push_back(Stringify::ToInt(value.as<std::string>()));
					skip++;
					switch (t) {
					case sol::type::string: {
						std::cout << k << ": " << value.as<std::string>() << std::endl;
					}
										  break;
					case sol::type::number: {
						auto v = value.as<double>();
						std::cout << k << ": " << v << std::endl;
					}
										  break;
					default: {
						std::cout << "hit the default case!" << std::endl;
					}
					}
				}
			}
		}


		sol::optional<sol::table> conf = lua["abc"]["layers"][1]["data"];
		if (conf) {
			for (const auto& key_value_pair : conf.value())
			{
				sol::object key = key_value_pair.first;
				sol::object value = key_value_pair.second;
				std::string k = key.as<std::string>();
				sol::type t = value.get_type();

				MapIdToCoords(57, Stringify::ToInt(value.as<std::string>()), block_type_ids);

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
	return false;
}

CTile* Location::GetTile(int x, int y)
{
	int ID = 0;

	ID = x / SCALED_SPRITE_WIDTH;
	ID = ID + (TILES_COLS * (y / SCALED_SPRITE_HEIGHT));

	if (ID < 0 || ID >= m_tiles_arr.size()) {
		return NULL;
	}

	return &m_tiles_arr[ID];
}

void Location::IdToXY(int id, int& x, int& y)
{
	x = (id % TILES_COLS) * SCALED_SPRITE_WIDTH;
	y = (id / TILES_COLS) * SCALED_SPRITE_HEIGHT;
}



/*
	0	UP,
	1	DOWN,
	2	LEFT,
	3	RIGHT
*/
int Location::GetTileDirId(int x, int y, int dir)
{
	int ID = 0;
	int ID_offset_x = 0;
	int ID_offset_y = 0;
	if (dir == 0)
	{
		ID_offset_y = -TILES_COLS;
	}
	else if (dir == 1)
	{
		ID_offset_y = TILES_COLS;
	}
	else if (dir == 2)
	{
		ID_offset_y = -1;
	}
	else if (dir == 3)
	{
		ID_offset_y = 1;
	}

	ID = x / SCALED_SPRITE_WIDTH;
	ID = ID + (TILES_COLS * (y / SCALED_SPRITE_HEIGHT));
	ID = ID + ID_offset_x + ID_offset_y;

	printf("Klikam na kafelek o ID = %d\n", ID);

	if (ID < 0 || ID >= m_tiles_arr.size()) {
		return -1;
	}

	return ID;
}

std::vector<int> Location::GetTileDirIds(int x, int y, int width, int height, int dir)
{
	return std::vector<int>();
}

void Location::MapIdToCoords(const int cols, const int id)
{
	CTile tempTile;
	if (id == 0)
	{
		tempTile.TypeID = TILE_TYPE_NONE;
	}
	else
	{
		tempTile.m_tile_x = (id - 1) / cols;
		tempTile.m_tile_y = (id - 1) % cols;
		tempTile.TypeID = TILE_TYPE_NORMAL;

		if (tempTile.m_tile_y == 1 && tempTile.m_tile_x == 0)
		{
			tempTile.TypeID = TILE_TYPE_BLOCK;
		}
	}

	m_tiles_arr.emplace_back(tempTile);
}

void Location::MapIdToCoords(const int cols, const int id, std::vector<int>& block_ids)
{
	CTile tempTile;
	if (id == 0)
	{
		tempTile.TypeID = TILE_TYPE_NONE;
	}
	else
	{
		tempTile.m_tile_x = (id - 1) / cols;
		tempTile.m_tile_y = (id - 1) % cols;
		tempTile.TypeID = TILE_TYPE_NORMAL;

		for (int block_id : block_ids)
		{
			if ((id - 1) == block_id)
			{
				tempTile.TypeID = TILE_TYPE_BLOCK;
				break;
			}
		}
	}

	m_tiles_arr.emplace_back(tempTile);
}