#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define PROMPT "> "

int main(void)
{
	char *buffer = malloc(sizeof(char) * MAX_COMMAND_LENGTH);

	if (!buffer)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		printf(PROMPT);
		if (!fgets(buffer, MAX_COMMAND_LENGTH, stdin))
		{
			if (feof(stdin))
			{
				printf("\n");
				break;
			}
			perror("fgets");
			exit(EXIT_FAILURE);
		}

		/*   Remove newline character from buffer*/
		buffer[strcspn(buffer, "\n")] = 0;
		pid_t pid = fork();

		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		} else if (pid == 0)
		{
			/* Child process*/
			if (execve(buffer, NULL, NULL) == -1)
			{
				perror("execve");
				exit(EXIT_FAILURE);
			}
		} else
		{
			/*Parent process*/
			int status;

			if (wait(&status) == -1)
			{
				perror("wait");
				exit(EXIT_FAILURE);
			}
			if (WIFEXITED(status))
			{
				printf("Program exited with status %d\n", WEXITSTATUS(status));
			}
			else if (WIFSIGNALED(status))
			{
				printf("Program was terminated by signal %d\n", WTERMSIG(status));
			}
		}
	}
	free(buffer);
	exit(EXIT_SUCCESS);
}
