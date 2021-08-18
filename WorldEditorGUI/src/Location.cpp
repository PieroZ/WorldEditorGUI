#include "Stringify.h"
#include "Location.h"
#include "Defines.h"
#include "TextureBank.h"
#include "PlayerWalker.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include "Config.h"

Location Location::starting_loc;
//Location Location::starting_loc_elements;

const char LUA_PROPERTIES[] = "properties";

static const auto RENDER_TILES_COLLISION_BOXES = ParamsRegister::R_bool("B_RenderTilesCollisionBoxes", false);


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
	m_bottom_limit = 0;
	m_right_limit = 0;
	m_tiles_per_col = 0;
}

Location::Location(const std::string location_name) //: m_location_name(location_name)
{
	m_render_collision_boxes = RENDER_TILES_COLLISION_BOXES();

	m_location_name = location_name;
	m_bottom_limit = 0;
	m_right_limit = 0;
	m_tiles_per_col = 0;
}

bool Location::OnLoad(char* filename)
{
	m_render_collision_boxes = RENDER_TILES_COLLISION_BOXES();
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
			//FIX ME
			MapIdToCoords(57, Stringify::ToInt(i));
			//MapIdToCoords(4, Stringify::ToInt(i));
			//printf("%s\n", i.c_str());
		}
	}
	return true;
}

void Location::OnRender(SDL_Renderer* renderer, int cam_x, int cam_y, Uint8 r/* = 255*/, Uint8 g /*= 255*/, Uint8 b /*= 255*/)
{
	//int player_tile = GetTileDirId(cam_x, cam_y);
	// Always centered camera approach

	int left = cam_x - (SCREEN_WIDTH / 2);
	int right = cam_x + (SCREEN_WIDTH / 2 - 1);
	int top = cam_y - (SCREEN_HEIGHT / 2);
	int bottom = cam_y + (SCREEN_HEIGHT / 2 - 1);

	//SetBorderLimits(left, right, top, bottom);

	SDL_Point top_left = { left, top };
	SDL_Point top_right = { right, top};
	SDL_Point bottom_right = { right, bottom };
	SDL_Point bottom_left = { left, bottom };
	   
	int top_left_id = floor(GetTileId(top_left.x, top_left.y));
	float top_right_id = ceil(GetTileId(top_right.x, top_right.y));
	float bottom_right_id = GetTileId(bottom_right.x, bottom_right.y);
	float bottom_left_id = GetTileId(bottom_left.x, bottom_left.y);

	/*int tile_offset_x = (cam_x % SCALED_SPRITE_WIDTH);
	int tile_offset_y = (cam_y % SCALED_SPRITE_HEIGHT);*/

	int tile_offset_x = top_left.x % SCALED_HERO_SPRITE_WIDTH;
	int tile_offset_y = top_left.y % SCALED_HERO_SPRITE_HEIGHT;
		
	//int first_tile_id = top_left.x / SCALED_SPRITE_WIDTH;
	//first_tile_id = first_tile_id + floor(float(top_left.y) / float(SCALED_HERO_SPRITE_HEIGHT)) * T_TILES_COLS;

	/*int LocationWidth = T_TILES_COLS * SPRITE_WIDTH;
	int LocationHeight = T_TILES_COLS * SPRITE_WIDTH;*/

	//SDL_Point l_go = IdToXY(T_TILES_COLS, top_left_id);

	//float kloc_debug = GetTileId(cam_x, cam_y);
	//std::cout << "tile_offset_y = " << tile_offset_y << std::endl;

	//int cols_count = top_right_id - top_left_id;
	//int rows_count = (bottom_left_id / TILES_COLS) - (top_left_id / TILES_COLS);
	//int rows_count = top_right_id - top_left_id;

	int cols_count = top_right_id - top_left_id;
	int rows_count = ceil((bottom_right_id - top_left_id) / m_tiles_per_col);

	int draw_count = 0;

	for (int row_id = 0; row_id < rows_count; ++row_id)
	{
		for (int col_id = 0; col_id < cols_count; ++col_id)
		{
			int id = top_left_id + col_id;
			id = id + row_id * m_tiles_per_col;

			SDL_Point tile_cords = IdToXY(m_tiles_per_col, id);

			//TextureBank::Get("ss_nomargin")->Render(tile_cords.x - cam_x, tile_cords.y - cam_y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, tile_cords.x, tile_cords.y, SPRITE_WIDTH, SPRITE_HEIGHT);
			if (id >= 0 && id < m_tiles_arr.size())
			{
				if (m_tiles_arr[id].TypeID != TILE_TYPE_NONE)
				{
					if (r != 255 || g != 255 || b != 255)
					{
						TextureBank::Get("ss_nomargin")->setColor(r, g, b);
					}
					TextureBank::Get("ss_nomargin")->Render(tile_cords.x - top_left.x, tile_cords.y - top_left.y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, m_tiles_arr[id].m_tile_spritesheet_y * SPRITE_WIDTH, m_tiles_arr[id].m_tile_spritesheet_x * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
				}

				if (m_render_collision_boxes)
				{
					if (m_tiles_arr[id].TypeID == TILE_TYPE_BLOCK)
						{
							SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);						
						}
						SDL_RenderDrawRect(renderer, &m_tiles_arr[id].m_collider);
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
				}
				//++draw_count;

						//const SDL_Rect col_rect{ tile_cords.x - top_left.x, tile_cords.y - top_left.y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT };
						//if (m_tiles_arr[id].TypeID == 2)
						//{
						//	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);						
						//}
						//SDL_RenderDrawRect(renderer, &col_rect);
						//SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
			}
		}
	}

	for (auto&& loc_element_layer : m_location_elements)
	{
		for (int row_id = 0; row_id < rows_count; ++row_id)
		{
			for (int col_id = 0; col_id < cols_count; ++col_id)
			{
				int id = top_left_id + col_id;
				id = id + row_id * m_tiles_per_col;

				SDL_Point tile_cords = IdToXY(m_tiles_per_col, id);

				//TextureBank::Get("ss_nomargin")->Render(tile_cords.x - cam_x, tile_cords.y - cam_y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, tile_cords.x, tile_cords.y, SPRITE_WIDTH, SPRITE_HEIGHT);
				if (id >= 0 && id < loc_element_layer.m_tiles_arr.size())
				{
					if (loc_element_layer.m_tiles_arr[id].TypeID != TILE_TYPE_NONE)
					{
						TextureBank::Get("ss_nomargin")->Render(tile_cords.x - top_left.x, tile_cords.y - top_left.y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, loc_element_layer.m_tiles_arr[id].m_tile_spritesheet_y * SPRITE_WIDTH, loc_element_layer.m_tiles_arr[id].m_tile_spritesheet_x * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
					}


					if (m_render_collision_boxes)
					{
						if (loc_element_layer.m_tiles_arr[id].TypeID == TILE_TYPE_BLOCK)
						{
							SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
						}
						int x_tile = loc_element_layer.m_tiles_arr[id].m_collider.x + tile_cords.x;
						int w_tile = loc_element_layer.m_tiles_arr[id].m_collider.w;
						int y_tile = loc_element_layer.m_tiles_arr[id].m_collider.y + tile_cords.y;
						int h_tile = loc_element_layer.m_tiles_arr[id].m_collider.h;

						SDL_Rect collision_box{ x_tile - top_left.x, y_tile - top_left.y, w_tile, h_tile };

						if (w_tile > 0 && h_tile > 0)
						{
							SDL_RenderDrawRect(renderer, &collision_box);
						}
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
					}
					//++draw_count;

							//const SDL_Rect col_rect{ tile_cords.x - top_left.x, tile_cords.y - top_left.y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT };
							//if (m_tiles_arr[id].TypeID == 2)
							//{
							//	SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);						
							//}
							//SDL_RenderDrawRect(renderer, &col_rect);
							//SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_TRANSPARENT);
				}
			}
		}
	}

	//std::cout << "Draw count = " << draw_count << std::endl;

	//for (int id_y = top_left_id / T_TILES_COLS; id_y < rows_count; ++id_y)
	//for (int id_y = 0; id_y < rows_count; ++id_y)
	//{
	//	//for (int id = top_left_id; id < top_right_id; ++id)
	//	for (int id = 0; id < cols_count; ++id)
	//	{
	//		//if (top_left_id + id < 0)
	//		//{
	//		//	continue;
	//		//}
	//		if ((top_left_id + id + id_y * T_TILES_COLS < m_tiles_arr.size()) && top_left_id + id + id_y * T_TILES_COLS >= 0)
	//		{
	//			//id = 0;
	//			//idx = 0;
	//			//idy = 0;
	//	//		for (auto&& p : Location::starting_loc_elements.m_tiles_arr)
	//		//	{
	//			if (m_tiles_arr[id].TypeID != TILE_TYPE_NONE)
	//			{
	//				//TextureBank::Get("ss_nomargin")->Render(id * SCALED_SPRITE_WIDTH - tile_offset_x, id_y * SCALED_SPRITE_HEIGHT - tile_offset_y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, m_tiles_arr[id + id_y * T_TILES_COLS].m_tile_y * SPRITE_WIDTH, m_tiles_arr[id + id_y * T_TILES_COLS].m_tile_x * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
	//				int sprite_x = top_left_id + id + id_y * T_TILES_COLS;
	//				

	//				TextureBank::Get("ss_nomargin")->Render(id * SCALED_SPRITE_WIDTH - tile_offset_x, id_y * SCALED_SPRITE_HEIGHT - tile_offset_y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT, m_tiles_arr[top_left_id + id + id_y * T_TILES_COLS].m_tile_y * SPRITE_WIDTH, m_tiles_arr[sprite_x].m_tile_x * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT);
	//				++draw_count;
	//			}
	//			//id++;
	//			//idx = id % TILES_COLS;
	//			//idy = id / TILES_COLS;
	//	//	}
	//		}
	//		else
	//		{
	//			TextureBank::Get("ss_nomargin")->Render(id * SCALED_SPRITE_WIDTH - tile_offset_x, id_y * SCALED_SPRITE_HEIGHT - tile_offset_y, SCALED_SPRITE_WIDTH, SCALED_SPRITE_HEIGHT,0, 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	//		}
	//	}
	//}

	//std::cout << "Draw Count = " << draw_count<<std::endl;

	//TILES_COLS max liczba PELNYCH kafelkow w poziomie wyswietlana na ekranie (efektywnie moze byc 1 wiecej)

	//m_tiles_arr
}



bool Location::OnLoadLua(const std::string& File, int tiles_layer_id)
{
	bool success = true;
	if (std::filesystem::exists(File))
	{
		m_location_name = std::filesystem::path(File).stem().string();
		//std::cout << std::filesystem::path(File).stem()<<std::endl;
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(File);
		int col_number = 0;

		std::vector<int> block_type_ids;

		ExtractColNoLua(lua, col_number);
		SetBorderLimitsLua(lua);


		for (int i = 1; i < 6; ++i)
		{
			sol::optional<sol::table> tiles_type = lua["abc"]["tilesets"][1]["tiles"][i];
			if (tiles_type) {
				for (const auto& key_value_pair : tiles_type.value())
				{
					sol::object key = key_value_pair.first;
					sol::object value = key_value_pair.second;
					std::string k = key.as<std::string>();
					sol::type t = value.get_type();

					// FIX ME TYPES CAN BE DIFFERENT THAN BLOCK:
					if (k == "id")	block_type_ids.push_back(Stringify::ToInt(value.as<std::string>()));
					switch (t) 
					{
						case sol::type::string: 
						{
							//std::cout << k << ": " << value.as<std::string>() << std::endl;
							break;
						}
						case sol::type::number: 
						{
							auto v = value.as<double>();
							//std::cout << k << ": " << v << std::endl;
							break;
						}
						default: 
						{
							//std::cout << "hit the default case!" << std::endl;
						}
					}
				}
			}
		}


		sol::optional<sol::table> conf = lua["abc"]["layers"][tiles_layer_id]["data"];
		if (conf) {
			for (const auto& key_value_pair : conf.value())
			{
				sol::object key = key_value_pair.first;
				sol::object value = key_value_pair.second;
				std::string k = key.as<std::string>();
				sol::type t = value.get_type();

				// TODO: Instead of hardcoded value extract col_no from spritesheet. 
				// We don't know which spritesheet values should be extracted though.
				// Value is actually stored in lua
				MapIdToCoords(col_number, Stringify::ToInt(value.as<std::string>()), block_type_ids);

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

bool Location::LoadWaypointsLua(const std::string& File)
{
	bool success = true;
	if (std::filesystem::exists(File))
	{
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
		auto ab = lua.script_file(File);

		// Step 1 find id coressponding to object layer
		// In order to do so iterate over layers checking if optional is true

		int id = 1;

		sol::optional<sol::table> conf = lua["abc"]["layers"][id];

		while (conf)
		{
			std::string type = lua["abc"]["layers"][id]["type"];
			if (type == "objectgroup")
			{
				//std::cout << "objectgroup @ " << id << std::endl;

				int object_id = 1;
				sol::optional<sol::table> object_group_objects = lua["abc"]["layers"][id]["objects"][object_id][LUA_PROPERTIES];
				while (object_group_objects)
				{
					for (const auto& key_value_pair : object_group_objects.value())
					{
						sol::object key = key_value_pair.first;
						sol::object value = key_value_pair.second;
						std::string k = key.as<std::string>();
						sol::type t = value.get_type();

						if (k == "GoTo")
						{
							//std::cout << "GoTo:" << value.as < std::string>() << std::endl;
							sol::optional<float> wp_y = lua["abc"]["layers"][id]["objects"][object_id]["y"];
							sol::optional<float> wp_x = lua["abc"]["layers"][id]["objects"][object_id]["x"];
							sol::optional<float> wp_width = lua["abc"]["layers"][id]["objects"][object_id]["width"];
							sol::optional<float> wp_height = lua["abc"]["layers"][id]["objects"][object_id]["height"];

							//if (wp_y)
							//{
							//	std::cout << " 6 * float wp_y = " << 6 * wp_y.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_y.value() " << std::endl;
							//}

							//if (wp_x)
							//{
							//	std::cout << "6 *  float wp_x = " << 6 * wp_x.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_x.value() " << std::endl;
							//}

							//if (wp_width)
							//{
							//	std::cout << " 6 * float wp_width = " << 6 * wp_width.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_width.value() " << std::endl;
							//}

							//if (wp_height)
							//{
							//	std::cout << "6 *  float wp_height = " << 6 * wp_height.value() << std::endl;
							//}
							//else
							//{
							//	// FATAL
							//	std::cout << " error obtaining  wp_height.value() " << std::endl;
							//}
							//ExitWaypoint(int x, int y, int width, int height) : m_x0(x), m_y0(y), m_width(width), m_height(height)
							ExitWaypoint exit_wp(static_cast<int>(wp_x.value()), static_cast<int>(wp_y.value()), static_cast<int>(wp_width.value()), static_cast<int>(wp_height.value()));
							//m_exit_waypoints

						}
					}
					object_id++;
					sol::optional<sol::table> tmp = lua["abc"]["layers"][id]["objects"][object_id][LUA_PROPERTIES];
					object_group_objects = tmp;
				}
					//else
					//{
					//	std::cout << " Missing object_group waypoint properties !\n";
					//}

				//break;
			}
			id++;
			sol::optional<sol::table> conf2 = lua["abc"]["layers"][id];
			conf = conf2;
		}
	}
	else
	{
		std::cout << " File " << File << " does not exist !\n";
		success = false;
	}

	return success;
}

//bool Location::HasPlayerEnteredWaypoint(int p_x, int p_y)
bool Location::HasPlayerEnteredWaypoint(PlayerWalker& player, std::shared_ptr<Location>& entered_loc, int& loc_x, int& loc_y)
{
	int p_x = player.m_x;
	int p_y = player.m_y;
	bool is_wp_triggered = false;

	for (auto&& wp : m_exit_waypoints)
	{
		int x0 = wp.GetX0();
		int y0 = wp.GetY0();
		int w = wp.GetWidth();
		int h = wp.GetHeight();

		if (p_x >= x0 && p_x <= x0 + w && p_y >= y0 && p_y <= y0 + h)
		{
			std::cout << " Waypoint triggered !" << std::endl;
			wp.OnExit(player, entered_loc, loc_x, loc_y);

			is_wp_triggered = true;
		}
	}

	return is_wp_triggered;
}

void Location::AddExitWaypoint(const ExitWaypoint& exit_waypoint)
{
	m_exit_waypoints.push_back(exit_waypoint);
}

std::string Location::GetLocationName() const
{
	return m_location_name;
}

void Location::ExtractColNoLua(sol::state& lua, int& col_no)
{
	sol::optional<int> col_num_sol = lua["abc"]["tilesets"][1]["columns"];

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

void Location::SetBorderLimitsLua(sol::state& lua)
{
	sol::optional<int> map_width_sol = lua["abc"]["width"];
	if (map_width_sol)
	{
		int map_width = map_width_sol.value();
		m_tiles_per_col = map_width;
		//std::cout << "map_width = " << map_width << std::endl;
		m_right_limit = map_width * SCALED_SPRITE_WIDTH;
		//std::cout << "location m_right_limit = " << m_right_limit << std::endl;
	}
	else
	{
		std::cout << "Unable to find map_width value in lua map" << std::endl;
	}

	sol::optional<int> map_height_sol = lua["abc"]["height"];
	if (map_height_sol)
	{
		int map_height = map_height_sol.value();
		//std::cout << "map_height = " << map_height << std::endl;
		m_bottom_limit = map_height * SCALED_SPRITE_HEIGHT;
		//std::cout << "location m_bottom_limit= " << m_bottom_limit << std::endl;
	}
	else
	{
		std::cout << "Unable to find map_height value in lua map" << std::endl;
	}

}

void Location::SetLocationLayersPtrs()
{
	//m_tiles_arr
	//m_tiles_layers
	std::vector<std::shared_ptr<CTile>> tiles_per_layer;
	tiles_per_layer.reserve(m_tiles_arr.size());
	for(auto&& tile : m_tiles_arr)
	{
		std::shared_ptr<CTile> tile_ptr = std::make_shared<CTile>(tile);
		tiles_per_layer.emplace_back(tile_ptr);
	}
	m_tiles_layers.emplace_back(tiles_per_layer);

	for (int elements_layer_index = 0; elements_layer_index < m_location_elements.size(); ++elements_layer_index)
	{
		std::vector<std::shared_ptr<CTile>> tiles_per_elements_layer;
		tiles_per_elements_layer.reserve(m_location_elements[elements_layer_index].m_tiles_arr.size());

		for (auto&& tile : m_location_elements[elements_layer_index].m_tiles_arr)
		{
			std::shared_ptr<CTile> tile_ptr = std::make_shared<CTile>(tile);
			tiles_per_elements_layer.emplace_back(tile_ptr);
		}
		m_tiles_layers.emplace_back(tiles_per_elements_layer);
	}
}

int Location::GetLayersCount() const
{
	return m_tiles_layers.size();
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

// FIX ME: TILES_COL_NO shouldn't be a thing - instead they should be read from lua?
CTile* Location::GetTile(int x, int y, int TILES_COL_NO)
{
	int ID = 0;

	ID = x / SCALED_SPRITE_WIDTH;
	ID = ID + (TILES_COL_NO * (y / SCALED_SPRITE_HEIGHT));

	if (ID < 0 || ID >= m_tiles_arr.size()) {
		return NULL;
	}

	return &m_tiles_arr[ID];
}

std::shared_ptr<CTile> Location::GetTileByLayer(int x, int y, int TILES_COL_NO, int layer_id)
{
	int ID = 0;

	ID = x / SCALED_SPRITE_WIDTH;
	ID = ID + (TILES_COL_NO * (y / SCALED_SPRITE_HEIGHT));

	if (ID < 0 || ID >= m_tiles_arr.size()) {
		return NULL;
	}

	return m_tiles_layers[layer_id][ID];
}

int Location::GetLocationWidth() const
{
	return m_right_limit;
}

int Location::GetLocationHeight() const
{
	return m_bottom_limit;
}

void Location::InsertLocationElements(LocationElements& loc_elements)
{
	m_location_elements.emplace_back(loc_elements);
}

LocationElements& Location::GetLocationElements(const int index)
{
	return m_location_elements[index];
}

void Location::SetLocationElementsVectorSize(const int size)
{
	m_location_elements.reserve(size);
}

void Location::SetLocationName(const std::string& loc_name)
{
	m_location_name = loc_name;
}

void Location::SetTilesPerCol(const int tiles_per_col)
{
	m_tiles_per_col = tiles_per_col;
}

void Location::SetRightLimit(const int right_limit)
{
	m_right_limit = right_limit;
}

void Location::SetBottomLimit(const int bottom_limit)
{
	m_bottom_limit = bottom_limit;
}

void Location::IdToXY(int id, int& x, int& y)
{
	x = (id % m_tiles_per_col) * SCALED_SPRITE_WIDTH;
	y = (id / m_tiles_per_col) * SCALED_SPRITE_HEIGHT;

	//x = (id % TILES_COLS) * m_right_limit;
	//y = (id / TILES_COLS) * m_bottom_limit;
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
		ID_offset_y = -m_tiles_per_col;
	}
	else if (dir == 1)
	{
		ID_offset_y = m_tiles_per_col;
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
	ID = ID + (m_tiles_per_col * floor(y / SCALED_SPRITE_HEIGHT));
	ID = ID + ID_offset_x + ID_offset_y;

	printf("Klikam na kafelek o ID = %d\n", ID);

	if (ID < 0 || ID >= m_tiles_arr.size()) {
		return -1;
	}

	return ID;
}

float Location::GetTileId(float x, float y)
{
	float ID = 0;

	ID = x / SCALED_SPRITE_WIDTH;
	//ID = ID + (TILES_COLS * (y / SCALED_SPRITE_HEIGHT));
	ID = ID + ((m_tiles_per_col * floor(y / SCALED_SPRITE_HEIGHT)));

	//printf("Klikam na kafelek o ID = %d\n", ID);

	if (ID < 0 || ID >= m_tiles_arr.size()) {
		//return -1;
	}

	return ID;
}

std::vector<int> Location::GetTileDirIds(int x, int y, int width, int height, int dir)
{
	return std::vector<int>();
}

unsigned int Location::IsPlayerAtEdgePosition(int p_x, int p_y)
{
	unsigned int edge = EdgeEnum::NONE;
	//HALF_WIDTH_VIEWPORT
	if (p_x - HALF_WIDTH_VIEWPORT < 0)
	{
		// LEFT EDGE
		edge = EdgeEnum::LEFT;
	}
	else if (p_x + HALF_WIDTH_VIEWPORT > SCREEN_WIDTH - 1)
	{
		// RIGHT EDGE
		edge = EdgeEnum::RIGHT;
	}

	if (p_y - HALF_HEIGHT_VIEWPORT < 0)
	{
		// TOP EDGE
		edge |= EdgeEnum::TOP;
	}
	else if (p_y + HALF_HEIGHT_VIEWPORT > SCREEN_HEIGHT - 1)
	{
		// BOTTOM EDGE
		edge |= EdgeEnum::BOTTOM;
	}


	return edge;
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
		tempTile.m_tile_spritesheet_x = (id - 1) / cols;
		tempTile.m_tile_spritesheet_y = (id - 1) % cols;
		tempTile.TypeID = TILE_TYPE_NORMAL;

		if (tempTile.m_tile_spritesheet_y == 1 && tempTile.m_tile_spritesheet_x == 0)
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
		tempTile.m_tile_spritesheet_x = (id - 1) / cols;
		tempTile.m_tile_spritesheet_y = (id - 1) % cols;
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

SDL_Point Location::IdToXY(const int cols, const int id)
{
	return { (id  % cols) * SCALED_SPRITE_WIDTH, (id  / cols) * SCALED_SPRITE_WIDTH };
}

void Location::SetBorderLimits(int& left, int& right, int& top, int& bottom)
{
	if (left < 0)
	{
		left = 0;
		right = SCREEN_WIDTH - 1;
	}
	//else if( right >=)

	if (top < 0)
	{
		top = 0;
		bottom = SCREEN_HEIGHT - 1;
	}
}
