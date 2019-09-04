#include <iostream>
#include <signals.hpp>
#include <demons.hpp>
#include <sys/wait.h>
#include <helper.hpp>

void sigchld(int signum) {
	DEBUGPRINT("caught SIGCHLD, updating afflicted");
	DEBUGPRINT("demon count: " << demons.size());
	pid_t p;
	int status;
    // Iterate all demons and check their PIDs
	// Im pretty sure there are better ways of doing this, but it works
	if((p=waitpid(-1, &status, WNOHANG)) != -1) {
		for(int i = 0; i < demons.size(); i++) {
			if(demons[i].pid == p) {
				DEBUGPRINT("found demon " << demons[i].name << "; pid " << p << "; status " << status);
				demons[i].update(status);
				return;
			}
		}
	}
}
