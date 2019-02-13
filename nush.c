#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#include "tokenizer.h"
#include "executor.h"
#include "parser.h"

char *readcmd(int prompt)
{
    char *cmd;
    char line_buf[500];
    int nl = 1;
    line_buf[0] = 0;
    while (1)
    {
        // Read next line
        char *rv = fgets(line_buf, 496, stdin);
        // Check for EOF
        if (rv == NULL)
        {
            if (prompt)
            {
                printf("\n");
            }
            exit(0);
        }
        
        if (line_buf[strlen(line_buf) - 2] == '\\')
        {
            line_buf[strlen(line_buf) - 2] = 0;
            if (!nl)
            {
                char *temp = calloc(strlen(line_buf) + strlen(cmd) + 1, sizeof(char));
                strcpy(temp, cmd);
                strcat(temp, line_buf);
                free(cmd);
                cmd = temp;
            }
            else
            {
                cmd = calloc(strlen(line_buf) + 1, sizeof(char));
                strcpy(cmd, line_buf);
            }
            nl = 0;
            continue;
        }

        if (!nl)
        {
            char *temp = calloc(strlen(line_buf) + strlen(cmd) + 1, sizeof(char));
            strcpy(temp, cmd);
            strcat(temp, line_buf);
            free(cmd);
            cmd = temp;
        }
        else
        {
            cmd = calloc(strlen(line_buf) + 1, sizeof(char));
            strcpy(cmd, line_buf);
        }
        
        break;
    }
    return cmd;
}

int main(int argc, char *argv[])
{
    int prompt = 1;

    if (argc > 1)
    {
        close(0);
        open(argv[1], O_RDONLY);
        prompt = 0;
    }


    // Keep prompting for input until EOF
    while (1)
    {
        if (prompt)
        {
            printf("nush$ ");
        }

        char * cmd = readcmd(prompt);
        fflush(stdout);

        // Tokenize line
        svec *tokens = tokenize(cmd);
        free(cmd);

        // Parse tokens to ast
        nush_ast *ast = parse(tokens);

        // Free tokens memory
        free_svec(tokens);
        
        // Save current position in file
        int pos = lseek(0, 0, SEEK_CUR);
        // Execute ast
        execute(ast, 0, NULL, 0);

        // Reset current position in file
        lseek(0, pos, SEEK_SET);
        // Free ast memory
        free_ast(ast);
    }

    return 0;
}
