#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

// Define DODEBUG Macros

#ifdef DEBUG
#define DODEBUG(A) A
#define DONDEBUG(A)
#else
#define DODEBUG(A)
#define DONDEBUG(A) A
#endif

// Define DEBUGPRINT Macros
#define DEBUGPRINT(A) DODEBUG(std::cout << "[" << __PRETTY_FUNCTION__ << "] " << A << "\n");
#define DEBUGPRINT_NONEWLINE(A) DODEBUG(std::cout << "[" << __PRETTY_FUNCTION__ << "] " << A);
// Define ERRRORPRINT Macros
#define ERRORPRINT(A) std::cerr << "[" << __PRETTY_FUNCTION__ << "] " << A << "\n";

void forkexecwait(std::string file);
void forkexecwait(std::string file, std::vector<std::string> args);
pid_t forkexec(std::string file);
pid_t forkexec(std::string file, std::vector<std::string> args);
std::vector<std::string> split_string(std::string str, std::string delimiter);
