#pragma once
#include "LuaLoaderBase.h"
#include "NPC.h"
#include <memory>

class NpcLuaLoader : public LuaLoaderBase<std::vector<std::shared_ptr<NPC>>>
{
	// Inherited via LuaLoaderBase
	virtual bool LoadFields(sol::state& lua, sol::optional<sol::table>& lua_layers, std::vector<std::shared_ptr<NPC>>&  loaded_object) override;
	inline static const char LUA_NPC_LAYER_NAME[] = "npcs";

};