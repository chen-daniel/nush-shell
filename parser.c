#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "svec.h"

nush_ast *parse(svec *tokens)
{
  char *ops[] = {";", "<", ">", "&", "&&", "|", "||"};

  for (int ii = 0; ii < 7; ii++)
  {
    char *op = ops[ii];

    if (svec_contains(tokens, op))
    {
      int jj = svec_find(tokens, op);
      svec *xs = svec_slice(tokens, 0, jj);
      svec *ys = svec_slice(tokens, jj + 1, tokens->size);
      nush_ast *ast = make_ast_op(op, parse(xs), parse(ys));
      free_svec(xs);
      free_svec(ys);
      return ast;
    }
  }
  char **cmd = calloc(tokens->size + 1, sizeof(char *));
  for (int ii = 0; ii < tokens->size; ii++)
  {
    int len = strlen(tokens->data[ii]);
    cmd[ii] = calloc((len + 1), sizeof(char));
    memcpy(cmd[ii], tokens->data[ii], len);
    cmd[ii][len] = 0;
  }
  cmd[tokens->size] = 0;
  return make_ast_cmd(cmd, tokens->size);
}
