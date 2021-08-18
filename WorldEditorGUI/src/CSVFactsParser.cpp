#include "CSVFactsParser.h"
#include "Stringify.h"
#include <fstream>
#include <sstream> // std::stringstream
#include <iostream>

bool CSVFactParser::Parse(const std::string& file_path, std::unordered_map<std::string, int>& parsed_output)
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
	std::string line, fact_name;

	// Read data, line by line
	while (std::getline(csv_file, line))
	{
		// Create a stringstream of the current line
		std::stringstream ss(line);

		parsed_output[line] = 0;
	}

	return success;
};
