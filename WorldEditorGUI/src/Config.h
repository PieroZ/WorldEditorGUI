#pragma once
#include <set>
#include <map>
#include <functional>


class Config;

class ParamsRegister
{
private:
	enum TuningDataType {
		TUNING_bool,
		TUNING_uint64_t,
		TUNING_int,
		TUNING_string
	};
	template <TuningDataType T>
	static void register_typed_key(std::string key);

	std::map<std::string, TuningDataType> known_keys;

public:
	static std::function<bool()> R_bool(std::string key, bool def = false, bool silent = false);
	static std::function<uint64_t()> R_uint64_t(std::string key, uint64_t def = 0, bool silent = false);
	static std::function<int()> R_int(std::string key, int def = 0, bool silent = false);
	static std::function<std::string()> R_string(std::string key, std::string def = "", bool silent = false);

public:
	void Logg() const;
	bool ParseTuningConfig(std::string filename);

private:
	std::map<std::string, std::string> tuning_config_map;
	std::string get(std::string key, std::string def, bool silent = false) const;
};

class Config
{
public:

	Config();

	static const Config& Get();
	static Config& GetInstance();

	void log_config() const;

	std::string m_ConfigFilename;
	ParamsRegister tuning_config;

};