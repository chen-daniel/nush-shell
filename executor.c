#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "svec.h"

void execute_cmd(svec *tokens)
{
  int cpid;
  if ((cpid = fork()))
  {
    int status;
    waitpid(cpid, &status, 0);
  }
  else
  {
    char** args = malloc(tokens->size * sizeof(char*));
    memcpy(args, tokens->data, tokens->size * sizeof(char*));
    execvp(svec_get(tokens, 0), tokens->data);
  }
}

void execute_redir_out(svec *tokens, char *file)
{
  printf("%s", file);
  int cpid;
  if ((cpid = fork()))
  {
    int status;
    waitpid(cpid, &status, 0);
  }
  else
  {
    char** args = malloc(tokens->size * sizeof(char*));
    memcpy(args, tokens->data, tokens->size * sizeof(char*));
    close(1);
    open(file, O_CREAT | O_APPEND | O_WRONLY, 0644);
    execvp(svec_get(tokens, 0), tokens->data);
  }
}

void execute_redir_in(svec *tokens, char *file)
{
  printf("%s", file);
  char** args = malloc(tokens->size);
  memcpy(args, tokens->data, tokens->size * sizeof(char*));
  printf("%s", args[100]);
  int cpid;
  if ((cpid = fork()))
  {
    int status;
    waitpid(cpid, &status, 0);
  }
  else
  {
    close(0);
    open(file, O_RDONLY);
    execvp(svec_get(tokens, 0), tokens->data);
  }
}

void execute(svec *tokens)
{
  int flag = 1;
  for (int ii = 0; ii < tokens->size; ii++)
  {
    if (strcmp(tokens->data[ii], ">") == 0)
    {
      flag = 0;
      char *file = malloc(strlen(tokens->data[ii + 1]) * sizeof(char));
      strcpy(file, tokens->data[ii + 1]);
      svec_pop(tokens);
      svec_pop(tokens);
      execute_redir_out(tokens, file);
    }
    else if (strcmp(tokens->data[ii], "<") == 0)
    {
      flag = 0;
      char *file = malloc(strlen(tokens->data[ii + 1]) * sizeof(char));
      strcpy(file, tokens->data[ii + 1]);
      svec_pop(tokens);
      svec_pop(tokens);
      execute_redir_in(tokens, file);
    }
  }
  if (flag) {
    execute_cmd(tokens);
  }
}
