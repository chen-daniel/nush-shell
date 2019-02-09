#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "svec.h"

nush_ast *make_ast_op(char *op, nush_ast *arg0, nush_ast *arg1)
{
  nush_ast* ast = malloc(sizeof(nush_ast));
  ast->op = op;
  ast->arg0 = arg0;
  ast->arg1 = arg1;
  ast->cmd = NULL;
}

nush_ast *make_ast_cmd(char **cmd)
{
  nush_ast* ast = malloc(sizeof(nush_ast));
  ast->op = "cmd";
  ast->arg0 = NULL;
  ast->arg1 = NULL;
  ast->cmd = cmd;
  return ast;
}

int ast_eval(nush_ast *ast)
{
}

void free_ast(nush_ast *ast)
{
}