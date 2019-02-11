#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "ast.h"
#include "parser.h"
#include "svec.h"

nush_ast *parse(svec *tokens)
{
  char *ops[] = {";", "&", "&&", "|", "||"};

  for (int ii = 0; ii < 5; ii++)
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

  int num_redir = 0;

  char *redir_ops[] = {"<", ">"};
  for (int i = 0; i < 2; i++)
  {
    if (svec_contains(tokens, redir_ops[i]))
    {
      num_redir++;
    }
  }
  char **redir = calloc(num_redir + 1, sizeof(char *));
  redir[0] = NULL;
  redir[1] = NULL;
  char **cmd = calloc(tokens->size + 1 - (num_redir * 2), sizeof(char *));
  for (int ii = 0, jj = 0; ii < tokens->size; ii++, jj++)
  {
    if (num_redir > 0)
    {
      for (int i = 0; i < 2; i++)
      {
        if (strcmp(tokens->data[ii], redir_ops[i]) == 0)
        {
          ii++;
          int len = strlen(tokens->data[ii]);
          redir[i] = calloc((len + 1), sizeof(char));
          memcpy(redir[i], tokens->data[ii], len);
          redir[i][len] = 0;
          continue;
        }
      }
    }
    int len = strlen(tokens->data[ii]);
    cmd[jj] = calloc((len + 1), sizeof(char));
    memcpy(cmd[jj], tokens->data[ii], len);
    cmd[jj][len] = 0;
  }
  cmd[tokens->size - (num_redir * 2)] = 0;
  redir[num_redir] = 0;

  return make_ast_cmd(cmd, tokens->size, redir[0], redir[1]);
}
