#pragma once
#include <string>
#include <vector>

struct DBCharacterEntry
{
	int m_id{ -1 };
	int m_sprite_x{ -1 };
	int m_sprite_y{ -1 };
	std::string m_name;
	std::string m_spritesheet_name;
};

class CSVCharacterParser
{
public:
	static bool Parse(const std::string& file_path, std::vector<DBCharacterEntry>& parsed_output);
};

