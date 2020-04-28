#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct Pipe {
	int r_fd, w_fd;
} Pipe;

// exits when the given return code corresponds to an error.
void exit_on_err(int return_code, char* action) {
	if(return_code == -1) {
		perror(action);
		exit(1);
	}
}

Pipe get_pipe() {
	int fds[2];
	exit_on_err(pipe(fds), "pipe\n");
	Pipe p;
	p.r_fd = fds[0];
	p.w_fd = fds[1];
	return p;
}

void close_pipe(Pipe p) {
	exit_on_err(close(p.r_fd), "close");
	exit_on_err(close(p.w_fd), "close");
}

// Progs es un array de programas, donde cada programa es un array de caracteres
// en el cual el primero es el nombre del programa y los siguientes sus args.
// Basicamente los args que requiere execvp.
static int run(const char*** progs, size_t count){
	// Pid de cada proceso
	pid_t children[count];

	// Necesito un pipe entre cada proceso
	fprintf(stderr, "pipes:\n");
	Pipe pipes[count - 1];
	for (int i = 0; i < count - 1; i++){
		pipes[i] = get_pipe();
		fprintf(stderr, "[w: %d, r: %d]\n", pipes[i].w_fd, pipes[i].r_fd);
	}

	for (int i = 0; i < count; i++) {
		// Redirecciono los fds
		// El proceso i lee del fd read del pipe i-1, y escribe en el de i
		fprintf(stderr, "corriendo %d (%s)\n", i, progs[i][0]);

		// Corro el proceso
		pid_t pid = fork();
		if (pid == 0) {
			// Soy el hijo

			// Si no es el 1ro cambio stdin
			if (i != 0) exit_on_err(dup2(pipes[i-1].r_fd, STDIN_FILENO), "dup2\n");
			// Si no es el ultimo cambio stdout
			if (i != count - 1) exit_on_err(dup2(pipes[i].w_fd, STDOUT_FILENO), "dup2\n");

			// cierro todos los pipes
			for (int i = 0; i < count - 1; i++) {
				close_pipe(pipes[i]);
			}

			exit_on_err(execvp(progs[i][0], progs[i]), "exec");
		}

		// Soy el padre, me guardo el pid
		children[i] = pid;
	}

	// cierro todos los pipes
	for (int i = 0; i < count - 1; i++) {
		close_pipe(pipes[i]);
	}

	int status;
	// Espero a que terminen todos los hijos
	for (int i = 0; i < count; i++) {
		if (waitpid(children[i], &status, 0) == -1) {
			perror("waitpid");
			return -1;
		}
		if (!WIFEXITED(status)) {
			fprintf(stderr, "proceso %d no terminó correctamente [%d]: ",
			    (int)children[i], WIFSIGNALED(status));
			perror("");
			return -1;
		}
	}

	return 0;
}

int main(int argc, char **argv){
	char* lscmd[] = { "ls", "-al", NULL };
	char* wccmd[] = { "wc", NULL };
	char* awkcmd[] = { "awk", "{ print $2 }", NULL };
	char** progs[] = { lscmd, wccmd, awkcmd };

	printf("status: %d\n", run(progs, sizeof(progs) / sizeof((progs)[0])));
	fflush(stdout);
	fflush(stderr);

	return 0;
}
