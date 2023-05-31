#include "LookupTables.h"

LookupTables::LookupTables()
{

}

LookupTables& LookupTables::Get()
{
	static LookupTables lookup_tables;
	return lookup_tables;
}

void LookupTables::UpdateMapIdToOverlapingIds(std::pair<int, int> map_id_layer_id_pair, std::vector<int>& overlaping_ids)
{
	m_map_id_to_overlaping_ids[map_id_layer_id_pair] = overlaping_ids;
}

std::vector<int> LookupTables::GetOverlapingIds(std::pair<int, int> map_id_layer_id_pair)
{
	return m_map_id_to_overlaping_ids.at(map_id_layer_id_pair);
}

std::set<int> LookupTables::GetAllOverlapingIdsByLayerAndRange(int layer_id, std::set<int>& visible_ids)
{
	std::set<int> result_ids;
	for (auto&& v_id : visible_ids)
	{
		std::pair<int, int> layer_id_tile_id(layer_id, v_id);
		auto it = m_map_id_to_overlaping_ids.find(layer_id_tile_id);
		if (it != m_map_id_to_overlaping_ids.end())
		{
			auto&& ids = GetOverlapingIds(layer_id_tile_id);
			for (auto&& id : ids)
			{
				result_ids.insert(id);
			}
		}
	}

	return result_ids;
}
