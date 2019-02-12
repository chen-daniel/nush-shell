#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "svec.h"

// Make an ast that represents an operator
nush_ast *make_ast_op(char *op, nush_ast *arg0, nush_ast *arg1)
{
  nush_ast *ast = calloc(1, sizeof(nush_ast));
  ast->op = op;
  ast->arg0 = arg0;
  ast->arg1 = arg1;
  ast->cmd = NULL;
  ast->len = 0;
  ast->redir_in = NULL;
  ast->redir_out = NULL;
  return ast;
}

// Make an ast that represents a command
nush_ast *make_ast_cmd(char **cmd, int len, char *redir_in, char *redir_out)
{
  nush_ast *ast = calloc(1, sizeof(nush_ast));
  ast->op = "cmd";
  ast->arg0 = NULL;
  ast->arg1 = NULL;
  ast->cmd = cmd;
  ast->len = len;
  ast->redir_in = redir_in;
  ast->redir_out = redir_out;
  return ast;
}

// Free up ast memory
void free_ast(nush_ast *ast)
{
  if (ast)
  {
    if (ast->arg0)
    {
      free_ast(ast->arg0);
    }

    if (ast->arg1)
    {
      free_ast(ast->arg1);
    }
    if (ast->cmd)
    {
      for (int ii = 0; ii < ast->len; ii++)
      {
        free(ast->cmd[ii]);
      }
      free(ast->cmd);
    }
    if (ast->redir_in)
    {
      free(ast->redir_in);
    }
    if (ast->redir_out)
    {
      free(ast->redir_out);
    }
    free(ast);
  }
}


// Print out an ast for testing and debugging purposes
void ast_print(nush_ast *ast)
{
  if (ast)
  {
    if (ast->arg0)
    {
      ast_print(ast->arg0);
    }

    if (strcmp(ast->op, "cmd") != 0)
    {
      printf("%s\n", ast->op);
    }
    else
    {
      for (int i = 0; i < ast->len; i++)
      {
        printf("%s ", ast->cmd[i]);
      }
      if (ast->redir_in)
      {
        printf("< %s ", ast->redir_in);
      }
      if (ast->redir_out)
      {
        printf("> %s ", ast->redir_out);
      }
      printf("\n");
    }

    if (ast->arg1)
    {
      ast_print(ast->arg1);
    }
  }
}