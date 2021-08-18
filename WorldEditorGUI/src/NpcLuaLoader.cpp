#include "NpcLuaLoader.h"
#include "Defines.h"

bool NpcLuaLoader::LoadFields(sol::state& lua, sol::optional<sol::table>& lua_layers, std::vector<std::shared_ptr<NPC>>& loaded_object)
{
	bool success = true;
	sol::optional<sol::table> lua_npc_table = GetLuaLayerTable(lua, LUA_NPC_LAYER_NAME);

	if (lua_npc_table)
	{
		//std::cout << " Found some npcs !\n";
		
		sol::optional<sol::table> lua_npcs_optional_table = GetLuaLayerTable(lua, LUA_NPC_LAYER_NAME);

		if (lua_npcs_optional_table)
		{
			sol::table lua_npcs_table = lua_npcs_optional_table.value();

			int npc_index = 1;
			sol::optional<sol::table> single_npc_optional_entry = lua_npcs_table[LUA_OBJECTS][npc_index];

			while(single_npc_optional_entry)
			{
				sol::table single_npc_entry = single_npc_optional_entry.value();
				std::string name = single_npc_entry[LUA_NAME];
				float x = single_npc_entry[LUA_X];
				float y = single_npc_entry[LUA_Y];
				int x_scaled = x * SPRITE_SCALE;
				int y_scaled = y * SPRITE_SCALE;

				std::shared_ptr<NPC> npc = std::make_shared<NPC>(name);
				npc->SetPosition(x_scaled, y_scaled);
				npc->SetName(name);
				loaded_object.emplace_back(npc);

				// DEBUG
				//std::cout << "name = " << name<<std::endl;
				//std::cout << "x_scaled = " << x_scaled<<std::endl;
				//std::cout << "y_scaled = " << y_scaled<<std::endl;

				++npc_index;
				sol::optional<sol::table> tmp = lua_npcs_table[LUA_OBJECTS][npc_index];
				single_npc_optional_entry = tmp;
			}

			//int opacity = npc_table["opacity"];
			//std::cout << "opacity = " << opacity << std::endl;
		}

	}
	else
	{
		// DEBUG
		std::cout << " Unable to find npc layer ! \n";
	}

	return success;
}
