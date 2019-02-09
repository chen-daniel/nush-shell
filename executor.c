#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"

void execute_cmd(nush_ast *ast)
{
  int cpid;
  if ((cpid = fork()))
  {
    int status;
    waitpid(cpid, &status, 0);
  }
  else
  {
    execvp(ast->cmd[0], ast->cmd);
  }
}

void execute_redir_out(nush_ast *ast, char *file)
{
}

void execute_redir_in(nush_ast *ast, char *file)
{
}

void execute(nush_ast *ast)
{
  execute_cmd(ast);
}
