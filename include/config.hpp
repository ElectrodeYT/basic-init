#ifndef CONFIG_HPP
#define CONFIG_HPP
// Things for config files
#include <string>
#include <vector>

class ConfigFile {
public:
	// Names of the entries.
	std::vector<std::string> config_names;
	// Values of the entries.
	std::vector<std::string> config_values;
};

class Config {
public:
	static ConfigFile readConfigFile(std::string file);
	// TODO:
	static ConfigFile generateDefaultBoot();
};
#endif
