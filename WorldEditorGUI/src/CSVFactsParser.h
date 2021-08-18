#pragma once
#include <string>
#include <unordered_map>


class CSVFactParser
{
public:
	static bool Parse(const std::string& file_path, std::unordered_map<std::string, int>& parsed_output);
};

