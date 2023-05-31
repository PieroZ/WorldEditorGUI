#pragma once
#include <unordered_map>
#include <string>

class IdToSpritesheet
{
public:
	IdToSpritesheet(IdToSpritesheet const&) = delete;
	void operator=(IdToSpritesheet const&) = delete;
	static IdToSpritesheet& Get();

	const std::string& GetCorrespondingSpritesheet(int id) const;
	//const int GetSpritesheetOffset(const std::string spritesheet_name) const;
	void AppendToMap(int id_key, const std::string& spritesheet_value);
	void AppendRangeToMap(int id_range, int starting_point, const std::string& spritesheet_value);
	void SpecifySizeInSpritesheet(const std::string& spritesheet_name, int width, int height);
	int GetSpriteBaseWidth(const std::string spritesheet_name);
	int GetSpriteBaseHeight(const std::string spritesheet_name);
	
	int GetFirstGidBasedOnID(int tile_id) const;

private:
	IdToSpritesheet();

private:
	std::unordered_map<int, std::string> id_to_ss_map;
	std::unordered_map<int, int> id_to_offset;
	

	// Same as above but with spritesheet ID reference
	//std::vector < std::pair<int, std::unordered_map<int, int>> ss_name_to_id_offset;
	
	// Below is not longer needed as it is replaced by id_to_offset
	//std::unordered_map<std::string, int> id_offset_per_spritesheet;
	std::unordered_map<std::string, int> spritesheet_to_width;
	std::unordered_map<std::string, int> spritesheet_to_height;

	//std::vector<std::string> spritesheets_in_map;
};
