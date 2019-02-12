#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#include "tokenizer.h"
#include "executor.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    int prompt = 1;

    if (argc > 1)
    {
        close(0);
        open(argv[1], O_RDONLY);
        prompt = 0;
    }

    char cmd[500];

    // Keep prompting for input until EOF
    while (1)
    {
        if (prompt)
        {
            printf("nush$ ");
        }

        // Read next line
        char *rv = fgets(cmd, 496, stdin);

        // Check for EOF
        if (rv == NULL)
        {
            if (prompt)
            {
                printf("\n");
            }
            exit(0);
        }
        fflush(stdout);

        // Tokenize line
        svec *tokens = tokenize(cmd);

        // Parse tokens to ast
        nush_ast *ast = parse(tokens);

        // Free tokens memory
        free_svec(tokens);

        // Execute ast
        execute(ast, 0, NULL, 0);

        // Free ast memory
        free_ast(ast);
    }

    return 0;
}
