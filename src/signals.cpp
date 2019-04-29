#include <iostream>
#include <signals.hpp>
#include <demons.hpp>
#include <sys/wait.h>

void sigchld(int signum) {
	std::cout << "None::sigchld caught SIGCHLD, updating afflicted\n";
	std::cout << "None::sigchld demon count: " << demons.size() << "\n";
	pid_t p;
	int status;

	if((p=waitpid(-1, &status, WNOHANG)) != -1) {
		for(int i = 0; i < demons.size(); i++) {
			if(demons[i].pid == p) {
				std::cout << "None::sigchld found demon " << demons[i].name << "; pid " << p << "; status " << status << "\n";
				demons[i].update(status);
				return;
			}
		}
	}
}
