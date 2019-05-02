#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <config.hpp>

// Stuff to do with Config files
ConfigFile Config::readConfigFile(std::string file) {
	// Setup Vars and open file
	std::ifstream ifile(file);
	// If file could not be opened, bail with a default config
	if(!ifile.is_open()) {
		std::cerr << "config::readConfigFile failed to read config file " << file << ", bailing as failed:true\n";
		ConfigFile out;
		out.config_names.push_back("failed");
		out.config_values.push_back("true");
		return out;
	}
	// Create some more vars
	ConfigFile out;
	std::string line;
	// Loop over every line
	while(std::getline(ifile, line)) {
		// Convert line into a stream for easier handling
		std::istringstream is_line(line);
		std::string name;
		// Get setting name
		if(std::getline(is_line, name, '=')) {
			std::string value;
			// Get setting value
			if(std::getline(is_line, value)) {
				out.config_names.push_back(name);
				out.config_values.push_back(value);
				#ifdef DEBUG
				std::cout << "config::readConfigFile " << out.config_names[out.config_names.size()-1] << ":" << out.config_values[out.config_values.size()-1] << "\n";
				#endif
			}
		} else {
			std::cerr << "config::readConfigFile found invalid config line \"" << line <<"\"; attempting to ignore it. this might cause problems\n";
		}
	}
	return out;
}
