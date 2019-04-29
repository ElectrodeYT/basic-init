#include <iostream>
#include <demons.hpp>
#include <helper.hpp>
#include <sys/wait.h>

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
                std::cerr << "DemonManager::addDemon attempted to add another demon but found demon with same name!\n";
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
		// TODO: Hanle this
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
	std::cerr << "DemonManager::deleteDemon attempted to delete demon that doesnt exist!\n";
	return -1;
}

int DemonManager::operateOnDemon(std::string name, DemonOperation op) {
	for(int i = 0; i < demons.size(); i++) {
		if(demons[i].name == name) {
			if(op == DEMONSTART) {
				if(demons[i].start() < 0) {
					std::cout << "DemonManager::operateOnDemon failed to start demon " << name << "\n";
					return -1;
				}
				std::cout << "DemonManager::operateOnDemon started demon " << name << "\n";
				return 0;
			} else if(op == DEMONSTOP) {
				demons[i].stop();
				std::cout << "DemonManager::operateOnDemon stopped demon " << name << "\n";
				return 0;
			} else if(op == DEMONRESTART) {
				demons[i].restart();
				std::cout << "DemonManager::operateOnDemon restarted demon " << name << "\n";
				return 0;
			} else {
				std::cout << "DemonManager::operateOnDemon invalid op\n";
				return -2;
			}
		}
	}
	std::cout << "DemonManager::operateOnDemon invalid demon\n";
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

int Demon::restart() {
	if(Demon::stop() != 0) {
		return -1;
	}
	return Demon::start();
}

int Demon::update() {
	std::cout << "Demon::update Updating demon " << name;
	if(running == true) {std::cout << "with (pottentialy former) pid " << pid;};
	std::cout << "\n";
	if(running != true) {
		return 0;
	}
	int status;
	waitpid(pid, &status, WNOHANG);
	return update(status);

}
int Demon::update(int status) {
	if(status < 0) {
		std::cout << "Demon::update waitpid returned " << status << ", stopping demon\n";
		stop();
		return status;
	} else if(status >= 0) {
		if(settings != DEMONNORESTART) {
			if(crash_count > MAX_CRASH_COUNT) {
				std::cerr << "Demon::update demon " << name << "royally fucked, stopping it!\n";
				stop();
				return 0;
			}
			std::cout << "Demon::update demon crashed, restarting " << name << "\n";
			running = false;
			int c = crash_count + 1;
			restart(); // restart zeros crash count, so we have to store it
			crash_count = c;
		} else {
			std::cout << "Demon::update demon " << name << " exited\n";
			pid = 0;
			running = false;
		}
		return 0;
	}
}

Demon::Demon() {
	running = false;
	crash_count = 0;
}
