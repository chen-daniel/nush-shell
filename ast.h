#ifndef NUSH_AST_H
#define NUSH_AST_H

#include "svec.h"

typedef struct nush_ast
{
  char *op;
  // Op is either:
  // one of: "<", ">", "&", "&&", "|", "||", ";"
  //     or: "cmd" for command
  struct nush_ast *arg0;
  struct nush_ast *arg1;
  char **cmd;
  int len;
} nush_ast;

nush_ast *make_ast_op(char *op, nush_ast *arg0, nush_ast *arg1);
nush_ast *make_ast_cmd(char **cmd, int len);
nush_ast *make_ast_svec(svec *tokens);
int ast_eval(nush_ast *ast);
void free_ast(nush_ast *ast);

#endif