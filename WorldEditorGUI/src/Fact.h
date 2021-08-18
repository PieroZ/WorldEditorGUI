#pragma once
#include <string>

class Fact
{
public:
	Fact(std::string fact_name, int init_val);

	std::string m_fact_name;
	int m_fact_value;
};
