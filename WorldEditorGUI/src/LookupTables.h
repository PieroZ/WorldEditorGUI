#pragma once
#include <map>
#include <utility>
#include <vector>
#include <set>

class LookupTables
{
	LookupTables();
public:
	LookupTables(LookupTables const&) = delete;
	void operator=(LookupTables const&) = delete;

	static LookupTables& Get();
	
	void UpdateMapIdToOverlapingIds(std::pair<int, int> map_id_layer_id_pair, std::vector<int>& overlaping_ids);
	std::vector<int> GetOverlapingIds(std::pair<int, int> map_id_layer_id_pair);
	// Definitely optimalisations required...
	std::set<int> GetAllOverlapingIdsByLayerAndRange(int layer_id, std::set<int>& visible_ids);

private:
	// key: [map_corresponding_ID, layer_id] : [IDs_overlaping_to_this_id]
	
	// 29.09.2021
	// Zmienic kolejnoscia key-value + std::map<std::pair<int, int>, std::vector<int>> => std::map<std::pair<int, int>, int>

	// warstwa_id_kafelka, 
	std::map<std::pair<int, int>, int> m_map_visible_id_to_actual_sprite_id;
	std::map<std::pair<int, int>, std::vector<int>> m_map_id_to_overlaping_ids;
};