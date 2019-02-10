#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "svec.h"

nush_ast *make_ast_op(char *op, nush_ast *arg0, nush_ast *arg1)
{
  nush_ast *ast = calloc(1, sizeof(nush_ast));
  ast->op = op;
  ast->arg0 = arg0;
  ast->arg1 = arg1;
  ast->cmd = NULL;
  ast->len = 0;
}

nush_ast *make_ast_cmd(char **cmd, int len)
{
  nush_ast *ast = calloc(1, sizeof(nush_ast));
  ast->op = "cmd";
  ast->arg0 = NULL;
  ast->arg1 = NULL;
  ast->cmd = cmd;
  ast->len = len;
  return ast;
}

int ast_eval(nush_ast *ast)
{
}

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
    free(ast);
  }
}