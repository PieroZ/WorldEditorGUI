#include "ActiveFactStates.h"

ActiveFactStates& ActiveFactStates::get()
{
	static ActiveFactStates afs;
	return afs;
}

void ActiveFactStates::InitFactValues(const std::unordered_map<std::string, int>& init_map)
{
	m_world_facts = init_map;
}

void ActiveFactStates::ModifyFactValue(const std::string& fact_name, int modifier)
{
	m_world_facts[fact_name] += modifier;
}

int ActiveFactStates::GetFactValue(const std::string& fact_name)
{
	return m_world_facts[fact_name];
}
