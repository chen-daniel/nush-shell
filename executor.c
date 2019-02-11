#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"
#include "executor.h"

int execute_cmd(nush_ast *ast, int pipe, int *fds, int bg)
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
      int status = 0;
      if (!bg) {
        waitpid(cpid, &status, 0);
      }
      return status;
    }
    else
    {
      if (fds)
      {
        if (pipe)
        {
          close(1);
          dup(fds[pipe]);
          close(fds[0]);
        }
        else
        {
          close(0);
          dup(fds[pipe]);
          close(fds[1]);
        }
      }
      if (ast->redir_in)
      {
        close(0);
        open(ast->redir_in, O_RDONLY);
      }
      if (ast->redir_out)
      {
        close(1);
        open(ast->redir_out, O_CREAT | O_TRUNC | O_WRONLY, 0644);
      }
      execvp(ast->cmd[0], ast->cmd);
    }
  }
}

int pipe_op(nush_ast *ast, int pip, int *fds, int bg)
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
    if (fds)
    {
      if (pip)
      {
        close(1);
        dup(fds[pip]);
        close(fds[0]);
      }
      else
      {
        close(0);
        dup(fds[pip]);
        close(fds[1]);
      }
    }
    int pipe_fds[2];
    int rv = pipe(pipe_fds);
    if (rv == -1)
    {
      perror("Failed to create pipe.");
      exit(1);
    }
    execute(ast->arg0, 1, pipe_fds, bg);
    close(pipe_fds[1]);
    execute(ast->arg1, 0, pipe_fds, bg);
    close(pipe_fds[0]);
  }
}

int execute(nush_ast *ast, int pipe, int *fds, int bg)
{
  if (strcmp(ast->op, "cmd") == 0)
  {
    if (ast->len > 0)
    {
      return execute_cmd(ast, pipe, fds, bg);
    }
    else
    {
      return 0;
    }
  }
  else if (strcmp(ast->op, ";") == 0)
  {
    execute(ast->arg0, pipe, fds, bg);
    return execute(ast->arg1, pipe, fds, bg);
  }
  else if (strcmp(ast->op, "&") == 0)
  {
    execute_cmd(ast->arg0, pipe, fds, 1);
    return execute(ast->arg1, pipe, fds, bg);
  }
  else if (strcmp(ast->op, "&&") == 0)
  {
    int rv = execute(ast->arg0, 0, NULL, bg);
    if (rv == 0)
    {
      return execute(ast->arg1, pipe, fds, bg);
    }
  }
  else if (strcmp(ast->op, "|") == 0)
  {
    return pipe_op(ast, pipe, fds, bg);
  }
  else if (strcmp(ast->op, "||") == 0)
  {
    int rv = execute(ast->arg0, 0, NULL, bg);
    if (rv != 0)
    {
      return execute(ast->arg1, pipe, fds, bg);
    }
  }
}
