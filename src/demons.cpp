#include <iostream>
#include <filesystem>
#include <algorithm>
#include <sys/wait.h>
#include <demons.hpp>
#include <helper.hpp>
#include <config.hpp>

std::vector<Demon> demons;

int DemonManager::addDemon(std::string path, std::string name) {
	return addDemon(path, name, DEMONNORMAL);
}

int DemonManager::addDemon(std::string path, std::string name, DemonSettings settings) {
	std::vector<std::string> args;
	return addDemon(path, name, args, settings);
}

int DemonManager::addDemon(std::string path, std::string name, std::vector<std::string> args, DemonSettings settings) {
        //  Make sure that there is no other demon with this name
        bool found = false;
        for(int i = 0; i < demons.size(); i++) {
                if(demons[i].name == name) { found = true; break; }
        }
        if(found == true) {
                ERRORPRINT("attempted to add another demon but found demon with same name!");
                return -1;
        }
        // Create Demon
        Demon demon;
        demon.path = path;
        demon.name = name;
        demon.settings = settings;
				demon.args = args;
        // Add demon to the list of demons
        demons.push_back(demon);
        return demons.size() - 1; // Return ID of Demon (ie last demon)
}

std::string DemonManager::getDemonNameByID(int id) {
	if(id > demons.size() || id < 0) {
		// TODO: Throw some sort of error (maybe start singleuser demongroup?)
		return "";
	}
	return demons[id].name; // Simple, really
}

int DemonManager::getDemonIDbyName(std::string name) {
	if(name == "") { // Probably from getDemonNameByID
		// TODO: Handle this
		return -1;
	}
	// Look at every demon and compare the name
	for(int i = 0; i < demons.size(); i++) {
		if(demons[i].name == name) {
			return i;
		}
	}
	return -1; // Not found
}

int DemonManager::deleteDemon(std::string name) {
	for(int i = 0; i < demons.size(); i++) {
		if(demons[i].name == name) {
			demons[i].stop();
			demons.erase(demons.begin() + i);
			return 0;
		}
	}
	ERRORPRINT("attempted to delete demon that doesnt exist!");
	return -1;
}

int DemonManager::operateOnDemon(std::string name, DemonOperation op) {
	for(int i = 0; i < demons.size(); i++) {
		if(demons[i].name == name) {
			if(op == DEMONSTART) {
				if(demons[i].start() < 0) {
					DEBUGPRINT("failed to start demon " << name);
					return -1;
				}
				DEBUGPRINT("started demon " << name);
				return 0;
			} else if(op == DEMONSTOP) {
				demons[i].stop();
				DEBUGPRINT("stopped demon " << name);
				return 0;
			} else if(op == DEMONRESTART) {
				demons[i].restart();
				DEBUGPRINT("restarted demon " << name);
				return 0;
			} else if(op == DEMONRUNNING) {
				return demons[i].running;
			} else {
				ERRORPRINT("invalid op!");
				return -2;
			}
		}
	}
	ERRORPRINT("invalid demon");
	return -1;
}

int Demon::start() {
	if(running == true) {
		return 0;
	}
	pid_t p = forkexec(path, args);
	if(p < 0) { return p; }
	pid = p;
	running = true;
	crash_count = 0;
	return p;
}

/// Stop the Demon.
/// Doneby sending SIGTERM to the demon, waiting 500000 us, and then sending SIGKILL if the pid exists.
/// SUPER BIG TODO: Another process may take this pid in this time. Fix this.
int Demon::stop() {
	if(running == false) {
		return 0;
	}
	// Check PID Exists
	if(!kill(pid, 0)) {
		running = false;
		return 0;
	}
	// Kill it
	kill(pid, SIGTERM);
	usleep(500000);
	if(kill(pid, 0)) {
		kill(pid, SIGKILL);
	}
	running = false;
	return 0;
}

/// Restart the demon.
int Demon::restart() {
	if(Demon::stop() != 0) {
		return -1;
	}
	return Demon::start();
}

/// Update the demon.
int Demon::update() {
	DEBUGPRINT("Updating Demon " << name);
	DODEBUG(if(running == true) {DEBUGPRINT("with (pottentialy former) pid " << pid);};)
	std::cout << "\n";
	if(running != true) {
		return 0;
	}
	int status;
	waitpid(pid, &status, WNOHANG);
	return update(status);
}

// Update the demon.
int Demon::update(int status) {
	if(status < 0) {
		DEBUGPRINT("waitpid returned " << status << ", stopping demon");
		stop();
		return status;
	} else if(status >= 0) {
		if(settings != DEMONNORESTART) {
			if(crash_count > MAX_CRASH_COUNT) {
				DEBUGPRINT("demon " << name << " crashed too many times");
				stop();
				return 0;
			}
			DEBUGPRINT("demon crashed, restarting " << name);
			running = false;
			int c = crash_count + 1;
			restart(); // resets zeros crash count, so we have to store it
			crash_count = c;
		} else {
			DEBUGPRINT("demon " << name << " exited");
			pid = 0;
			running = false;
		}
		return 0;
	}
	return 0;
}

int DemonManager::addDemonByConfig(std::string dname) {
	std::string cpath = DEMON_LOCATIONS + dname + DEMON_LOCATIONS_EXTENSION;
	ConfigFile config = Config::readConfigFile(cpath);
	std::string name; // Name of the demon to be loaded.
	std::string path; // Executable path.
	std::vector<std::string> args; // Arguments.
	DemonSettings settings; // DemonSettings.
	std::vector<std::string> wants;
	std::vector<std::string> requires;
	for(int i = 0; i < config.config_names.size(); i++) {
		if(config.config_names[i] == "name") {
			name = config.config_values[i];
		}
		if(config.config_names[i] == "path") {
			path = config.config_values[i];
		}
		if(config.config_names[i] == "arguments") {
			args = split_string(config.config_values[i], " ");
		}
		if(config.config_names[i] == "wants") {
			wants.push_back(config.config_values[i]);
		}
		if(config.config_names[i] == "requires") {
			wants.push_back(config.config_values[i]);
		}
		if(config.config_names[i] == "settings") {
			if(config.config_values[i] == "normal") {
				settings = DEMONNORMAL;
			} else if(config.config_values[i] == "norestart") {
				settings = DEMONNORESTART;
			} else {
				settings = DEMONNORMAL;
			}
		}
	}
	if(name == "" || path == "") {
		// Invalid demon
		return -1;
	}
	return DemonManager::addDemon(path, name, args, settings);
}

int DemonManager::addAllDemonsByConfig() {
	std::vector<std::string> files;
	std::filesystem::directory_iterator iter(DEMON_LOCATIONS);
	std::filesystem::directory_iterator end;
	while(iter != end) {
		std::error_code err_code;
		if(!std::filesystem::is_directory(iter->path())) {
			if(iter->path().extension() == DEMON_LOCATIONS_EXTENSION) {
				DemonManager::addDemonByConfig(iter->path().stem());
			}
		}
		iter.increment(err_code);
		if(err_code) {
			ERRORPRINT("failed to iterate directory with error: " << err_code.message());
		}
	}
	return 0;
}

Demon::Demon() {
	running = false;
	crash_count = 0;
}
