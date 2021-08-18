#include "Config.h"
#include "Log.h"
#include <sstream>
#include <fstream>

#include <string>
#include <vector>

namespace {
	bool STRING_TO_BOOL(std::string s)
	{
		return s != std::to_string(false);
	}

	uint64_t STRING_TO_UINT64(std::string s)
	{
		try
		{
			return std::stoull(s); //TODO: A
		}
		catch (...)
		{
			ERROR("Cannot convert \"" + s + "\" to integer");
			return 0ull;
		}
	}

	uint64_t STRING_TO_INT(std::string s)
	{
		try
		{
			int X;
			std::stringstream strStream(s);
			strStream >> X;

			return X;
		}
		catch (...)
		{
			ERROR("Cannot convert \"" + s + "\" to integer");
			return 0ull;
		}
	}
}
Config::Config()
{
	m_ConfigFilename = "ingame_vars_config.txt";
}

const Config& Config::Get()
{
	return GetInstance();
}

Config& Config::GetInstance()
{
	static Config config;
	return config;
}

void Config::log_config() const
{
	INFO("Config:");

	INFO("\tTuning config:");
	tuning_config.Logg();
}

void ParamsRegister::Logg() const
{
	if (tuning_config_map.empty())
		INFO("\t\tempty");
	else
		for (auto entry : tuning_config_map)
			INFO("\t\t" + entry.first + ": " + entry.second);
}

template <ParamsRegister::TuningDataType T>
void ParamsRegister::register_typed_key(std::string key)
{
	// for convenience
	auto& this_ = Config::GetInstance().tuning_config;

	// check if the key is being re-registered as a different data type
	const auto found = this_.known_keys.find(key);
	if (found != this_.known_keys.end())
	{
		if (found->second != T)
			ERROR("Attempting to re-register tuning parameter key \"" + found->first + "\" with different datatype (" + std::to_string(T) + " != " + std::to_string(found->second) + ")");
	}
	else
		this_.known_keys.insert(std::make_pair(key, T));
}

std::function<bool()> ParamsRegister::R_bool(std::string key, bool def, bool silent)
{
	register_typed_key<TUNING_bool>(key);

	return [=]()
	{
		return STRING_TO_BOOL(Config::Get().tuning_config.get(key, std::to_string(def), silent));
	};
}

std::function<uint64_t()> ParamsRegister::R_uint64_t(std::string key, uint64_t def, bool silent)
{
	register_typed_key<TUNING_uint64_t>(key);

	return [=]()
	{
		return STRING_TO_UINT64(Config::Get().tuning_config.get(key, std::to_string(def), silent));
	};
}

std::function<int()> ParamsRegister::R_int(std::string key, int def, bool silent)
{
	register_typed_key<TUNING_int>(key);

	return [=]()
	{
		return STRING_TO_INT(Config::Get().tuning_config.get(key, std::to_string(def), silent));
	};
}

std::function<std::string()> ParamsRegister::R_string(std::string key, std::string def, bool silent)
{
	register_typed_key<TUNING_string>(key);

	return [=]()
	{
		return Config::Get().tuning_config.get(key, def, silent);
	};
}

bool ParamsRegister::ParseTuningConfig(std::string filename)
{
	if (filename.empty()) return true;

	std::ifstream infile(filename);
	if (!infile.is_open())
		ERROR("Could not read tuning config file \"" + filename + "\"");

	// Set up line counter, line buffer and input file
	uint64_t line_number = 0;
	std::string line;

	//Read the file, line by line ...
	while (std::getline(infile, line))
	{
		line_number++;

		// if it's a b lank line, we discard
		if (line.length() == 0)
			continue;

		// If it's a comment line, we discard
		if (line[0] == '#')
			continue;

		const auto equals_pos = line.find("=");
		if (equals_pos == std::string::npos)
		{
			ERROR("Could not parse tuning config file, line " + filename + ":" + std::to_string(line_number));
			ERROR("\t\"" + line + "\"");
			return false;
		}

		const auto key = line.substr(0, equals_pos);
		const auto value = line.substr(equals_pos + 1, line.length() - equals_pos - 1);

		const auto key_found = tuning_config_map.find(key);
		if (key_found != tuning_config_map.end())
		{
			ERROR("Duplicate tuning configuration parameter \"" + key + "\"");
			ERROR("\tExisting: \"" + key_found->second + "\", new value: \"" + value + "\"");
			return false;
		}

		const auto is_key_allowed = known_keys.find(key);
		if (is_key_allowed == known_keys.end())
		{
			ERROR("Key \"" + key + "\" not recognised");
			INFO("Permitted tuning parameter keys are:");
			for (auto k : known_keys)
				INFO("\t" + k.first);
			return false;
		}
		tuning_config_map[key] = value;
	}
	return true;
}

std::string ParamsRegister::get(std::string key, std::string def, bool silent) const
{
	const auto found = tuning_config_map.find(key);
	if (found == tuning_config_map.end())
	{
		if (!silent) INFO("Tuning: " + key + " has default value " + def);
		return def;
	}
	if (!silent) INFO("Tuning: " + key + " has configured value " + found->second);
	return found->second;

}