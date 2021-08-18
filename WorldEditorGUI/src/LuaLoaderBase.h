#pragma once
#include <sol.hpp>
#include <string>
#include <filesystem>

// Template Method 
template <typename LO> // Loaded object
class LuaLoaderBase
{
public:
	bool OnLoadLua(const std::string& file_path, LO& loaded_object)
	{
		bool success = true;

		if (std::filesystem::exists(file_path))
		{
			sol::state lua;
			lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
			auto ab = lua.script_file(file_path);
			
			sol::optional<sol::table> lua_layers = lua[LUA_MAP_HEADER][LUA_LAYERS][LUA_LAYERS_ID];
			if (lua_layers)
			{
				success = LoadFields(lua, lua_layers, loaded_object);
			}
			else
			{
				success = false;
			}
		}
		else
		{
			success = false;
		}

		return success;
	}
protected:
	virtual bool LoadFields(sol::state& lua, sol::optional<sol::table>& lua_layers, LO& loaded_object) = 0;
	bool GetLuaStringValue(sol::optional<sol::table>& sol_optional_table, const std::string& lua_string_key, std::string& extracted_lua_value)
	{
		bool lua_table_found = true;
		if (sol_optional_table)
		{
			for (const auto& key_value_pair : sol_optional_table.value())
			{
				sol::object key = key_value_pair.first;
				sol::object value = key_value_pair.second;
				std::string k = key.as<std::string>();
				sol::type t = value.get_type();

				if (k == lua_string_key)
				{
					//block_type_ids.push_back(Stringify::ToInt(value.as<std::string>()));
					extracted_lua_value = value.as<std::string>();
				}


				/*switch (t)
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
				}*/
			}
		}
		else
		{
			lua_table_found = false;
		}

		return lua_table_found;
	}

	sol::optional<sol::table> GetLuaLayerTable(sol::state& lua, const std::string& layer_name)
	{
		int tiles_layer_id = 1;
		sol::optional<std::string> col_num_sol = lua[LUA_MAP_HEADER][LUA_LAYERS][tiles_layer_id][LUA_NAME];
		sol::optional<sol::table> lua_table;

		while (col_num_sol)
		{
			//if (col_num_sol.as<std::string>() == layer_name)
			std::string l_name = col_num_sol.value();
			if (l_name == layer_name)
			{
				sol::optional<sol::table> lua_table = lua[LUA_MAP_HEADER][LUA_LAYERS][tiles_layer_id];
				return lua_table;
			}
			tiles_layer_id++;
			
			sol::optional<std::string> tmp = lua[LUA_MAP_HEADER][LUA_LAYERS][tiles_layer_id][LUA_NAME];
			col_num_sol = tmp;
		}

		return lua_table;
	}

protected:
	std::string m_filename;

	inline static const char LUA_MAP_HEADER[] = "abc";
	inline static const char LUA_LAYERS[] = "layers";
	inline static const char LUA_DATA[] = "data";
	inline static const char LUA_TILESETS[] = "tilesets";
	inline static const char LUA_TILES[] = "tiles";
	inline static const char LUA_COLUMNS[] = "columns";
	inline static const char LUA_NAME[] = "name";
	inline static const char LUA_X[] = "x";
	inline static const char LUA_Y[] = "y";
	inline static const char LUA_PROPERTIES[] = "properties";
	inline static const char LUA_OBJECTS[] = "objects";
	inline static const char LUA_WIDTH[] = "width";
	inline static const char LUA_HEIGHT[] = "height";
	
	inline static const int LUA_LAYERS_ID = 1;
};