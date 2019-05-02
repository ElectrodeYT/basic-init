#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

#ifdef DEBUG
#define DODEBUG(A) A
#else
#define DODEBUG(A)
#endif

void forkexecwait(std::string file);
void forkexecwait(std::string file, std::vector<std::string> args);
pid_t forkexec(std::string file);
pid_t forkexec(std::string file, std::vector<std::string> args);
std::vector<std::string> split_string(std::string str, std::string delimiter);
