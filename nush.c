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

    char cmd[256];

    // Keep prompting for input until EOF
    while (1)
    {
        if (prompt)
        {
            printf("nush$ ");
        }

        // Read next line
        char *rv = fgets(cmd, 96, stdin);

        // Check for EOF
        if (rv == NULL)
        {
            printf("\n");
            exit(0);
        }
        fflush(stdout);

        // Tokenize line
        svec *tokens = tokenize(cmd);

        nush_ast* ast = parse(tokens);

        free_svec(tokens);

        if (tokens->size > 0) {
            execute(tokens);
        }

        free_ast(ast);
    }

    return 0;
}
