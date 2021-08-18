//#include "LuaLoaderBase.h"

//bool LuaLoaderBase::OnLoadLua(const std::string& file_path, LO loaded_object)
//{
//	bool success = true;
//
//	if (std::filesystem::exists(file_path))
//	{
//		sol::state lua;
//		lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
//		auto ab = lua.script_file(file_path);
//		
//		success = LoadFields();
//	}
//	else
//	{
//		success = false;
//	}
//
//	return success;
//}

//bool LuaLoaderBase::GetLuaStringValue(sol::optional<sol::table>& sol_optional_table, const std::string& lua_string_key, std::string& extracted_lua_value)
//{
//	bool lua_table_found = true;
//	if (sol_optional_table)
//	{
//		for (const auto& key_value_pair : sol_optional_table.value())
//		{
//			sol::object key = key_value_pair.first;
//			sol::object value = key_value_pair.second;
//			std::string k = key.as<std::string>();
//			sol::type t = value.get_type();
//
//			if (k == lua_string_key)
//			{
//				//block_type_ids.push_back(Stringify::ToInt(value.as<std::string>()));
//				extracted_lua_value = value.as<std::string>();
//			}
//			
//
//			/*switch (t)
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
//			}*/
//		}
//	}
//	else
//	{
//		lua_table_found = false;
//	}
//
//	return lua_table_found;
//}
