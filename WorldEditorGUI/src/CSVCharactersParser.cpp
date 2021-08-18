#include "CSVCharactersParser.h"
#include "Stringify.h"
#include <fstream>
#include <sstream> // std::stringstream
#include <iostream>

bool CSVCharacterParser::Parse(const std::string& file_path, std::vector<DBCharacterEntry>& parsed_output)
{
	bool success = true;

	// Create an input filestream
	std::ifstream csv_file(file_path);

	// Make sure the file is open
	if (!csv_file.is_open())
	{
		//FATAL
		std::cout << "Unable to load file " << file_path << std::endl;
		success = false;
		return success;
	}

	// Helper vars
	std::string line, field;

	// Read data, line by line
	while (std::getline(csv_file, line))
	{
		// Create a stringstream of the current line
		std::stringstream ss(line);


		// Iterate over each field
		int field_id = 0;
		DBCharacterEntry character_entry;
		while (std::getline(ss, field, ',')) {

			// Initialize and add <colname, int vector> pairs to result
			//std::cout << field << std::endl;
			//result.push_back({ field, std::vector<int> {} });

			if (field_id == 0)
			{
				character_entry.m_name = field;
			}
			else if (field_id == 1)
			{
				character_entry.m_id = Stringify::ToInt(field);
			}
			else if (field_id == 2)
			{
				character_entry.m_spritesheet_name = field;
			}
			else if (field_id == 3)
			{
				character_entry.m_sprite_x = Stringify::ToInt(field);
			}
			else if (field_id == 4)
			{
				character_entry.m_sprite_y = Stringify::ToInt(field);
			}
			field_id++;
		}
		parsed_output.emplace_back(character_entry);
	}

	return success;
}
