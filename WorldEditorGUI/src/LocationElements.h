#pragma once
#include "LocationStruct.h"


// Class representing elements on location
// Initiated first with just filename_map.lua as it's location name
// Then LuaLoader::OnLocationLoadLua updates all it's members
class LocationElements : public LocationStruct
{
public:
	LocationElements(const std::string location_name);	
};