#pragma once
#include <unordered_map>
#include <string>
//#include <vector>

class ActiveFactStates
{
	ActiveFactStates()
	{

	}

	std::unordered_map<std::string, int> m_world_facts;
public:
	ActiveFactStates(ActiveFactStates const&) = delete;
	void operator=(ActiveFactStates const&) = delete;

	static ActiveFactStates& get();

	void InitFactValues(const std::unordered_map<std::string, int>& init_map);

	void ModifyFactValue(const std::string& fact_name, int modifier);

	int GetFactValue(const std::string& fact_name);
};