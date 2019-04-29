#ifndef CONFIG_HPP
#define CONFIG_HPP
// Things for config files
#include <string>
#include <vector>

class ConfigFile {
public:
	std::vector<std::string> config_names;
	std::vector<std::string> config_values;
};

class Config {
public:
	static ConfigFile readConfigFile(std::string file);
	static ConfigFile generateDefaultBoot();
};
#endif
