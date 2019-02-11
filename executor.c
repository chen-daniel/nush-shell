#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"

int execute_cmd(nush_ast *ast)
{
  if (strcmp(ast->cmd[0], "cd") == 0)
  {
    if (ast->len > 1)
    {
      return chdir(ast->cmd[1]);
    }
    else
    {
      return chdir("~");
    }
  }
  else if (strcmp(ast->cmd[0], "exit") == 0)
  {
    exit(0);
  }
  else
  {
    int cpid;
    if ((cpid = fork()))
    {
      int status;
      waitpid(cpid, &status, 0);
      return status;
    }
    else
    {
      execvp(ast->cmd[0], ast->cmd);
    }
  }
}

void execute_background_cmd(nush_ast *ast)
{
  int cpid;
  if ((cpid = fork()))
  {
    return;
  }
  else
  {
    execvp(ast->cmd[0], ast->cmd);
  }
}

int execute_redir_out(nush_ast *ast, char *file)
{
  int cpid;
  if ((cpid = fork()))
  {
    int status;
    waitpid(cpid, &status, 0);
    return status;
  }
  else
  {
    close(1);
    open(file, O_CREAT | O_TRUNC | O_WRONLY, 0644);
    execvp(ast->cmd[0], ast->cmd);
  }
}

int execute_redir_in(nush_ast *ast, char *file)
{
  int cpid;
  if ((cpid = fork()))
  {
    int status;
    waitpid(cpid, &status, 0);
    return status;
  }
  else
  {
    close(0);
    open(file, O_RDONLY);
    execvp(ast->cmd[0], ast->cmd);
  }
}

int execute(nush_ast *ast)
{
  if (strcmp(ast->op, "cmd") == 0)
  {
    if (ast->len > 0)
    {
      return execute_cmd(ast);
    }
    else
    {
      return 0;
    }
  }
  else if (strcmp(ast->op, ";") == 0)
  {
    execute(ast->arg0);
    execute(ast->arg1);
  }
  else if (strcmp(ast->op, "<") == 0)
  {
    return execute_redir_in(ast->arg0, ast->arg1->cmd[0]);
  }
  else if (strcmp(ast->op, ">") == 0)
  {
    return execute_redir_out(ast->arg0, ast->arg1->cmd[0]);
  }
  else if (strcmp(ast->op, "&") == 0)
  {
    execute_background_cmd(ast->arg0);
    execute(ast->arg1);
  }
  else if (strcmp(ast->op, "&&") == 0)
  {
    int rv = execute(ast->arg0);
    if (rv == 0)
    {
      execute(ast->arg1);
    }
  }
  else if (strcmp(ast->op, "|") == 0)
  {
  }
  else if (strcmp(ast->op, "||") == 0)
  {
    int rv = execute(ast->arg0);
    if (rv != 0)
    {
      execute(ast->arg1);
    }
  }
}
