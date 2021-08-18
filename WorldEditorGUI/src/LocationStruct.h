#pragma once
#include <vector>
#include "CTile.h"
#include <string>
#include <memory>

struct LocationStruct
{
	std::vector<CTile> m_tiles_arr;

	//15.08.2021 m_tiles_layers are supposed to replace m_tiles_arr & are supposed to contain all tiles elements from one location
	std::vector<std::vector<std::shared_ptr<CTile>>> m_tiles_layers;
	int m_bottom_limit;
	int m_right_limit;
	int m_tiles_per_col;
	std::string m_location_name;
};