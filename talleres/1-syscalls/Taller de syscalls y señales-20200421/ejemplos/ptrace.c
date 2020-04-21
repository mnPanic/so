#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/reg.h>   

int main()
{   pid_t child;
    long orig_rax;
    int status;
    child = fork();
    if(child == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("/bin/ls", "ls", NULL);
    }
    else {
        wait(NULL);
        orig_rax = ptrace(PTRACE_PEEKUSER,
                          child, 8 * ORIG_RAX,
                          NULL);
	while(1){
	ptrace(PTRACE_CONT, child, NULL, NULL);	
	if(wait(&status)<0){perror("wait");return 0;}
	if(WIFEXITED(status))break; //termino el proceso hijo
        }
	printf("El proceso hijo hizo la system call %ld\n", orig_rax);
    }
    return 0;
}
