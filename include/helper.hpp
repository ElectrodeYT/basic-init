#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

void forkexecwait(std::string file);
void forkexecwait(std::string file, std::vector<std::string> args);
pid_t forkexec(std::string file);
pid_t forkexec(std::string file, std::vector<std::string> args);
