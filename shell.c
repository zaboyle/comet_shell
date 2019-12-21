#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "stdbool.h"
#include "shell_utils.h"

#define MAX_INPUT_SIZE 100
#define MAX_PROGRAM_SIZE 20

int main(int argc, char **argv)
{
	while (1)
	{
		// prompt
		printf("zach's term => ");

		// read input
		char in[MAX_INPUT_SIZE];
		scanf("%s", in);

		// exit if desired
		if(!strcmp(in, "exit"))
		{
			printf("thanks for using zach's shell!\n");
			return 0;
		}

		// grab program name
		char program[MAX_PROGRAM_SIZE];
		strcpy(program, in);

		// grab child process pid to wait for its completion
		pid_t child_pid = fork();

		if (child_pid == 0)
		{
			// child process

			// exec program
			// argv is array of strings, used for program arguments
			// envp is array of strings, format is key=value
			// both must be terminated by NULL ptr
			
			bool is_script = true; // TODO: replace w/ parser func call

			char* argv[] = {NULL,}; // TODO: replace w/ parser func call
			char* envp[] = {NULL,}; // TODO: replace w/ parser func call

			if (is_script)
			{
				// bash scripts
				execve(program, argv, envp);
			}
			else
			{
				// programs
				int file_descriptor = open(program, O_RDONLY);
				fexecve(file_descriptor, argv, envp);
			}

		}
		else
		{
			// parent process

			// wait for child to finish
			siginfo_t siginfo;
			int retval = waitid(P_PID, child_pid, &siginfo, WEXITED);
			printf("return value: %i\n", retval);
		}
	}

	return 0;
}