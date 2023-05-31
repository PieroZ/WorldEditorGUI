#pragma once
#include <vector>
#include "CTile.h"
#include "LocationHelper.h"
#include <string>
#include <memory>
#include <algorithm>    

struct LocationStruct
{
	//std::vector<CTile> m_tiles_arr;

	//15.08.2021 m_tiles_layers are supposed to replace m_tiles_arr & are supposed to contain all tiles elements from one location
	std::vector<std::vector<std::shared_ptr<CTile>>> m_tiles_layers;
	//std::vector<std::shared_ptr<SDL_Rect>> m_collisions_vector;
	std::vector<std::shared_ptr<SDL_Rect>> m_collisions_vector;
	int m_bottom_limit;
	int m_right_limit;
	int m_tiles_per_col;
	std::string m_location_name;

	void SetTilesLayersSize(const int size)
	{
		m_tiles_layers.reserve(size);
		for (int i = 0; i < size; ++i)
		{
			std::vector<std::shared_ptr<CTile>> init_vec;
			m_tiles_layers.emplace_back(init_vec);
		}
	}

	void FillCollisionVector()
	{
		m_collisions_vector.clear();

		for (auto&& layer : m_tiles_layers)
		{
			for (auto&& tile : layer)
			{
				if (tile->m_collider.w > 0)
				{
					// FIX CHECK IS THIS ALLOWED?
					int x = 0;
					int y = 0;
					LocationHelper::get().GetXYBasedOnId(tile->m_tile_id, m_tiles_per_col, x, y);
					
					std::shared_ptr<SDL_Rect> collider_on_map = std::make_shared<SDL_Rect>();
					collider_on_map->x = tile->m_collider.x + x;
					collider_on_map->y = tile->m_collider.y + y;
					collider_on_map->w = tile->m_collider.w;
					collider_on_map->h = tile->m_collider.h;

					m_collisions_vector.emplace_back(collider_on_map);
					//m_collisions_vector.emplace_back(std::make_shared<SDL_Rect>(tile->m_collider));
				}
			}
		}

		std::sort(m_collisions_vector.begin(), m_collisions_vector.end(), [](std::shared_ptr<SDL_Rect> a, std::shared_ptr < SDL_Rect> b) -> bool
			{
				return a->x < b->x;
			});
	}
};