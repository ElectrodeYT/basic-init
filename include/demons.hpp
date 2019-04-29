#ifndef DEMONS_HPP
#define DEMONS_HPP
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>

#define MAX_CRASH_COUNT 5

enum DemonSettings {
	DEMONNORMAL = 0,
	DEMONNORESTART,
};

enum DemonOperation {
	DEMONSTART = 0,
	DEMONSTOP,
	DEMONRESTART,
};

class Demon {
public:
	Demon();
	// Is it running?
	bool running;
	// Current status of the Demon
	int status;
	// Amount of times that the demon crashed consequtivley
	int crash_count;
	// Pid of the proccess
	pid_t pid;
	// Executable
	std::string path;
	// Name of the Demon
	std::string name;
	// Arguments for this demon to be started
	std::vector<std::string> args;
	// Demons that should be started for this demon
	std::vector<int> wants_demons;
	// Settings
	DemonSettings settings;
	// Stop the Demon
	int stop();
	// Start the Demon
	int start();
	// Restart the Demon
	int restart();
	// Wait on the proccess
	int update();
	int update(int status);
};

extern std::vector<Demon> demons;

class DemonManager {
public:
	// Add a demon
	static int addDemon(std::string path, std::string name);
	static int addDemon(std::string path, std::string name, DemonSettings setting);
	static int addDemon(std::string path, std::string name, std::vector<std::string> args, DemonSettings setting);
	// Delete a Demon
	static int deleteDemon(std::string name);
	// Perform a operation on a demon
	static int operateOnDemon(std::string name, DemonOperation op);
	// Demon ID related
	static std::string getDemonNameByID(int id);
	static int getDemonIDbyName(std::string name);
};

#endif
