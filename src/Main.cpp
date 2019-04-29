#include <iostream>
#include <sstream>
#include <csignal>
#include <string>
#include <config.hpp>
#include <helper.hpp>
#include <signals.hpp>
#include <demons.hpp>
#include <unistd.h>

// Number of TTY demons to spawn.
#define TTY_COUNT 4

int main(int argc, char** argv) {
	std::cout << "BasicInit v0.1\n";
	// Setup signal handlers
	signal(SIGCHLD, sigchld);
	ConfigFile config_main = Config::readConfigFile("/etc/basicinit/init.cfg");
	// Parse the config file
	bool mount_root_rw = true;
	bool start_shell = true;
	bool start_shell_on_console = false;
	std::string shell = "/bin/login";

	for(int i = 0; i < config_main.config_names.size(); i++) {
		// I really hate that switches dont work on string but oh well
		// mountrw: valid values {yes, no}
		if(config_main.config_names[i] == "mountrw") {
			if(config_main.config_values[i] == "yes") { mount_root_rw = true; } else { mount_root_rw = false; }
		// shell: shell executable to start. valid values {directorys}
		} else if(config_main.config_names[i] == "shell") {
			start_shell = true;
			shell = config_main.config_values[i];
		} else if(config_main.config_names[i] == "shell_on_console") {
			if(config_main.config_values[i] == "yes") { start_shell_on_console = true; } else { start_shell_on_console = false; }
		} else if(config_main.config_names[i] == "start_shell") {
			if(config_main.config_values[i] == "yes") { start_shell = true; } else { start_shell = false;}
		}
	}
	std::vector<std::string> args{"-a"};
	forkexecwait("mount", args);
	if(mount_root_rw == true) {
		args = {"-o", "remount,rw", "/"};
		forkexecwait("mount", args);
	}
	if(start_shell == true) {
		std::cout << "None::main Starting shells\n";
		for(int i = 1; i <= TTY_COUNT; i++) {
			// Start Restart Demons for each TTY until TTY_COUNT
			std::stringstream ss_name;
			ss_name << "getty" << i;
			std::stringstream ss_getty;
			ss_getty << "tty" << i;
			std::vector<std::string> args = {"0", ss_getty.str()};
			std::cout << "None::main Adding Demon with name " << ss_name.str() << " and path of " << "getty" << "\n";
			DemonManager::addDemon("getty", ss_name.str(), args, DEMONNORMAL);
			DemonManager::operateOnDemon(ss_name.str(), DEMONSTART);
		}
		if(start_shell_on_console == true) {
			std::cout << "None::main Creating Console Shell Demon\n";
			std::vector<std::string> args = {"0", "console"};
			DemonManager::addDemon("getty", "gettyconsole", args, DEMONNORMAL);
			DemonManager::operateOnDemon("gettyconsole", DEMONSTART);
		}
	}
	while(true) {sleep(1000);}
}
