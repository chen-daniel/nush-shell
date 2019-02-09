#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ast.h"
#include "parser.h"
#include "svec.h"

nush_ast *parse(svec *tokens)
{
  const char *ops[] = {"<", ">", "&", "&&", "|", "||", ";"};

  for (int ii = 0; ii < 7; ii++)
  {
    const char *op = ops[ii];

    if (svec_contains(tokens, op))
    {
      int jj = svec_find(tokens, op);
      svec *xs = svec_slice(tokens, 0, jj);
      svec *ys = svec_slice(tokens, jj + 1, tokens->size);
      nush_ast* ast = make_ast_op(op[0], parse(xs), parse(ys));
      free_svec(xs);
      free_svec(ys);
      return ast;
    }
  }
  char ** cmd = malloc(tokens->size * sizeof(char*));
  for (int ii = 0; ii < tokens->size; ii++) {
    int len = strlen(tokens->data[ii]);
    cmd[ii] = malloc(len * sizeof(char));
    memcpy(cmd[ii], tokens->data[ii], len);
  }
  return make_ast_cmd(cmd);
}
