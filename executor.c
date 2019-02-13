#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "ast.h"
#include "executor.h"

// Execute a command and change stdin and stdout appropriately
int execute_cmd(nush_ast *ast, int pipe, int *fds, int bg)
{
  // Handle cd and exit commands
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
      if (!bg)
      {
        waitpid(cpid, &status, 0);
      }
      return status;
    }
    else
    {
      // If this command is attached to a pipe, then
      // use the correct end of pipe
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
      // If this command has a file redir in and/or
      // out, use those as stdin and/or stdout
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

// For a pipe operator, fork first, then construct the pipe
// and continue executing commands in the child.
int pipe_op(nush_ast *ast, int pip, int *fds, int bg)
{
  int cpid;
  if ((cpid = fork()))
  {
    int status = 0;
    if (!bg)
    {
      waitpid(cpid, &status, 0);
    }
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
    execute(ast->arg0, 1, pipe_fds, 1);
    close(pipe_fds[1]);
    rv = execute(ast->arg1, 0, pipe_fds, bg);
    close(pipe_fds[0]);

    exit(rv);
  }
}

// Execute background operations
int background_op(nush_ast *ast, int pipe, int *fds, int bg)
{
  int cpid;
  if ((cpid = fork()))
  {
    return execute(ast->arg1, pipe, fds, bg);
  }
  else
  {
    execute(ast->arg0, pipe, fds, 1);
    exit(0);
  }
}

// Execute an ast
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
    return background_op(ast, pipe, fds, bg);
  }
  else if (strcmp(ast->op, "&&") == 0)
  {
    int rv = execute(ast->arg0, pipe, fds, 0);
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
    int rv = execute(ast->arg0, pipe, fds, 0);
    if (rv != 0)
    {
      return execute(ast->arg1, pipe, fds, bg);
    }
  }
}
