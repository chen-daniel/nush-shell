#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "tokenizer.c"

void execute(svec *tokens)
{
    int cpid;

    if ((cpid = fork()))
    {
        // parent process
        printf("Parent pid: %d\n", getpid());
        printf("Parent knows child pid: %d\n", cpid);

        // Child may still be running until we wait.

        int status;
        waitpid(cpid, &status, 0);

        printf("== executed program complete ==\n");

        printf("child returned with wait code %d\n", status);
        if (WIFEXITED(status))
        {
            printf("child exited with exit code (or main returned) %d\n", WEXITSTATUS(status));
        }
    }
    else
    {
        // child process
        printf("Child pid: %d\n", getpid());
        printf("Child knows parent pid: %d\n", getppid());

        for (int ii = 0; ii < strlen(cmd); ++ii)
        {
            if (cmd[ii] == ' ')
            {
                cmd[ii] = 0;
                break;
            }
        }

        // The argv array for the child.
        // Terminated by a null pointer.
        char *args[] = {cmd, "one", 0};

        printf("== executed program's output: ==\n");

        execvp(cmd, args);
        printf("Can't get here, exec only returns on error.");
    }
}

int main(int argc, char *argv[])
{
    char cmd[256];

    // Keep prompting for input until EOF
    while (1)
    {
        printf("nush$ ");

        // Read next line
        char *rv = fgets(cmd, 96, stdin);

        // Check for EOF
        if (rv == NULL)
        {
            exit(0);
        }
        fflush(stdout);

        // Tokenize line
        svec *tokens = tokenize(cmd);

        execute(tokens);

        // Free vector memory
        free_svec(tokens);
    }

    return 0;
}
