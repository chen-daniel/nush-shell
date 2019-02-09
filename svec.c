/* This file is lecture notes from CS 3650, Fall 2018 */
/* Author: Nat Tuck */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "svec.h"

svec *
make_svec()
{
    svec *sv = malloc(sizeof(svec));
    sv->size = 0;
    sv->cap = 4;
    sv->data = malloc(4 * sizeof(char *));
    memset(sv->data, 0, 4 * sizeof(char *));
    return sv;
}

void free_svec(svec *sv)
{
    for (int ii = 0; ii < sv->size; ++ii)
    {
        if (sv->data[ii] != 0)
        {
            free(sv->data[ii]);
        }
    }
    free(sv->data);
    free(sv);
}

char *
svec_get(svec *sv, int ii)
{
    assert(ii >= 0 && ii < sv->size);
    return sv->data[ii];
}

void svec_put(svec *sv, int ii, char *item)
{
    assert(ii >= 0 && ii < sv->size);
    sv->data[ii] = strdup(item);
}

void svec_push_back(svec *sv, char *item)
{
    int ii = sv->size;

    if (ii >= sv->cap)
    {
        sv->cap *= 2;
        sv->data = (char **)realloc(sv->data, sv->cap * sizeof(char *));
    }

    sv->size = ii + 1;
    svec_put(sv, ii, item);
}

svec *svec_slice(svec *xs, int i0, int i1)
{
    svec *ys = make_svec();
    for (int ii = i0; ii < i1; ii++)
    {
        svec_push_back(ys, xs->data[ii]);
    }
    return ys;
}

int svec_find(svec *xs, const char *str)
{
    for (int ii = 0; ii < xs->size; ii++)
    {
        if (strcmp(xs->data[ii], str) == 0)
        {
            return ii;
        }
    }
    return -1;
}

int svec_contains(svec *xs, const char *str)
{
    return find(xs, str) >= 0;
}
