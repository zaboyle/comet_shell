#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "shell_utils.h"
#include "shell_symbols.h"

#define MAX_INPUT_SIZE 100
#define MAX_PROGRAM_SIZE 20

int main(int argc, char **argv)
{

prompt:
	printf("%s", SHL_PROMPT);

	// read input
	char in[MAX_INPUT_SIZE];
	scanf("%s", in);

	// exit if desired
	if (!strcmp(in, SHL_EXIT_WORD))
	{
		printf("%s\n", SHL_EXIT_MSG);
		return 0;
	}

	// grab program name
	char program[MAX_PROGRAM_SIZE];
	strcpy(program, in);
	// argv is array of strings, used for program arguments
	// envp is array of strings, format is key=value
	// both must be terminated by NULL ptr
	char *p_argv[] = {NULL,}; // TODO: replace w/ parser func call
	char *p_envp[] = {NULL,}; // TODO: replace w/ parser func call

	// set up pipe
	int my_pipe[2];
	if (pipe(my_pipe) == -1)
	{
		perror("error setting up pipe");
		exit(1);
	}

	// grab child process pid to wait for its completion
	pid_t child_pid = fork();

	if (child_pid == 0) // child process
	{
		// child doesnt read
		close(my_pipe[0]);
		// re-route output to second entry in pipe
		// while loop is to account for interrupts
		while ((dup2(my_pipe[1], STDOUT_FILENO) == -1) && (errno == EINTR)) {}
		close(my_pipe[1]);

		// exec program, doesn't return
		execve(program, p_argv, p_envp);
	}
	else // parent process
	{
		// parent doesn't write
		close(my_pipe[1]);

		// read child's output to parent
		char child_output_buf[1];
		while (read(my_pipe[0], child_output_buf, 1) > 0)
		{
			write(STDOUT_FILENO, child_output_buf, 1);
		}
		close(my_pipe[0]);

		// wait for child to finish
		siginfo_t siginfo;
		int retval = waitid(P_PID, child_pid, &siginfo, WEXITED);

		goto prompt;
	}

}