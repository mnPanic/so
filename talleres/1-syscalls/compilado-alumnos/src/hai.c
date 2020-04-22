#include <signal.h> /* constantes como SIGINT*/
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/reg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


void parent();
void child();

int main(int argc, char* argv[]) {
  	pid_t pid = fork();
	if(pid == 0) {
		child();
	} else {
		parent(pid);
	}
	return 0;
}

void parent(pid_t child_pid) {
	write(1, "padre\n", 6);
}
void child() {
	write(1, "hijo\n", 5);
}
