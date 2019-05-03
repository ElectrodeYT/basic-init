#include <iostream>
#include <signals.hpp>
#include <demons.hpp>
#include <sys/wait.h>
#include <helper.hpp>

void sigchld(int signum) {
	DODEBUG(std::cout << "None::sigchld caught SIGCHLD, updating afflicted\n";)
	DODEBUG(std::cout << "None::sigchld demon count: " << demons.size() << "\n";)
	pid_t p;
	int status;

	if((p=waitpid(-1, &status, WNOHANG)) != -1) {
		for(int i = 0; i < demons.size(); i++) {
			if(demons[i].pid == p) {
				DODEBUG(std::cout << "None::sigchld found demon " << demons[i].name << "; pid " << p << "; status " << status << "\n";)
				demons[i].update(status);
				return;
			}
		}
	}
}
