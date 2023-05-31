#include "IdToSpritesheet.h"
#include "Log.h"


IdToSpritesheet::IdToSpritesheet()
{
}

IdToSpritesheet& IdToSpritesheet::Get()
{
	static IdToSpritesheet afs;
	return afs;
}

const std::string& IdToSpritesheet::GetCorrespondingSpritesheet(int id) const
{
	return id_to_ss_map.at(id);
}

//const int IdToSpritesheet::GetSpritesheetOffset(const std::string spritesheet_name) const
//{
//	return id_offset_per_spritesheet.at(spritesheet_name);
//}

void IdToSpritesheet::AppendToMap(int id_key, const std::string& spritesheet_value)
{
	id_to_ss_map[id_key] = spritesheet_value;
}

void IdToSpritesheet::AppendRangeToMap(int id_range, int starting_point, const std::string& spritesheet_value)
{
	// check if the key is being re-registered 
	/*if (std::find(spritesheets_in_map.begin(), spritesheets_in_map.end(), spritesheet_value) == spritesheets_in_map.end())
	{
		spritesheets_in_map.emplace_back(spritesheet_value);
		id_offset_per_spritesheet[spritesheet_value] = starting_point;
	}
	else
	{
		return;
	}*/

	for (int i = starting_point; i < id_range + starting_point; ++i)
	{
		id_to_ss_map[i] = spritesheet_value;
		id_to_offset[i] = starting_point;
	}
}

void IdToSpritesheet::SpecifySizeInSpritesheet(const std::string& spritesheet_name, int width, int height)
{
	spritesheet_to_width[spritesheet_name] = width;
	spritesheet_to_height[spritesheet_name] = height;
}

int IdToSpritesheet::GetSpriteBaseWidth(const std::string spritesheet_name)
{
	return spritesheet_to_width.at(spritesheet_name);
}

int IdToSpritesheet::GetSpriteBaseHeight(const std::string spritesheet_name)
{
	return spritesheet_to_height.at(spritesheet_name);
}

// replaced by locations method
int IdToSpritesheet::GetFirstGidBasedOnID(int tile_id) const
{
	return id_to_offset.at(tile_id);
}

