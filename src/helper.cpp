#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <helper.hpp>

void forkexecwait(std::string file) {
	// Create some vars
  pid_t pid;
	// Fork
  pid = fork();
	// If child..
  if(pid == 0) {
	  // Exec
    std::cout << "none::forkexecwait starting " << file << "\n";
    int err = execlp(file.c_str(), file.c_str(), NULL);
    std::cerr << "none::forkexecwait stil here after execlp " << file << "; execlp error " << err << "\n";
    exit(0);
  } else if(pid < 0) {
	  // Error
    std::cerr << "none:forkexecwait failed to fork";
  } else {
		// Wait for completion; this also makes it so that the process is deleted after finishing
    waitpid(pid, NULL, 0);
  }
}

void forkexecwait(std::string file, std::vector<std::string> args) {
  	// Create var
    pid_t pid;
	  // Fork
    pid = fork();
    if(pid == 0) {
		// Convert String vector to const char* vector
		  std::vector<const char*> args_c;
		  args_c.push_back(file.c_str());
		  for(int i = 0; i < args.size(); i++) {
			     args_c.push_back(args[i].c_str());
		  }
		  args_c.push_back(NULL);
		  // I.. i dont even know. For some reason &args_c.front() is char*const* so it needs to be cast?
      std::cout << "none::forkexecwait calling " << file << "\n";
      int err = execvp(file.c_str(), const_cast<char**>(&args_c.front()));
      std::cerr << "none::forkexecwait stil here after execvp " << file << "; execvp error " << err << "\n";
      exit(0);
    } else if(pid < 0) {
		  // Error
      std::cerr << "none:forkexecwait failed to fork";
    } else {
		  // Wait
      waitpid(pid, NULL, 0);
    }
}

pid_t forkexec(std::string file) {
    // Create some vars
    pid_t pid;
    // Fork
    pid = fork();
    // If child..
    if(pid == 0) {
      // Exec
      std::cout << "none::forkexec calling " << file << "\n";
      int err = execlp(file.c_str(), file.c_str(), NULL);
      std::cerr << "none::forkexec still here after execlp " << file << "; execlp error " << err << "\n";
      exit(0);
    } else if(pid < 0) {
      // Error
      std::cerr << "none:forkexec failed to fork";
    }
    return pid;
}

pid_t forkexec(std::string file, std::vector<std::string> args) {
        // Create var
        pid_t pid;
        // Fork
        pid = fork();
        if(pid == 0) {
                // Convert String vector to const char* vector
                std::vector<const char*> args_c;
                args_c.push_back(file.c_str());
                for(int i = 0; i < args.size(); i++) {
                        args_c.push_back(args[i].c_str());
                }
                args_c.push_back(NULL);
                // I.. i dont even know. For some reason &args_c.front() is char*const* so it needs to be cast?
                std::cout << "none::forkexec calling " << file << "\n";
                int err = execvp(file.c_str(), const_cast<char**>(&args_c.front()));
                std::cerr << "none::forkexecwait stil here after execvp " << file << "; execvp error " << err << "\n";
                exit(0);
        } else if(pid < 0) {
                // Error
                std::cerr << "none:forkexecwait failed to fork";
        }
	return pid;
}

std::vector<std::string> split_string(std::string str, std::string delimiter) {
  std::vector<std::string> ret;
  size_t pos = 0;
  std::string token;
  while ((pos = str.find(delimiter)) != std::string::npos) {
    token = str.substr(0, pos);
    ret.push_back(token);
    str.erase(0, pos + delimiter.length());
  }
  return ret;
}
