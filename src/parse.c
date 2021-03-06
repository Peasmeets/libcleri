/*
 * parser.c - this contains the start for parsing a string to a grammar.
 *
 * author       : Jeroen van der Heijden
 * email        : jeroen@transceptor.technology
 * copyright    : 2016, Transceptor Technology
 *
 * changes
 *  - initial version, 08-03-2016
 *
 */
#include <cleri/expecting.h>
#include <cleri/parse.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/*
 * Return a parse result. In case of a memory allocation error the return value
 * will be NULL.
 */
cleri_parse_t * cleri_parse(cleri_grammar_t * grammar, const char * str)
{
    cleri_parse_t * pr;
    const char * end;
    const char * test;
    bool at_end = true;

    /* prepare parsing */
    pr = (cleri_parse_t *) malloc(sizeof(cleri_parse_t));
    if (pr == NULL)
    {
        return NULL;
    }

    pr->str = str;
    pr->tree = NULL;
    pr->kwcache = NULL;
    pr->expecting = NULL;
    pr->is_valid = 0;

    if (    (pr->tree = cleri__node_new(NULL, str, 0)) == NULL ||
            (pr->kwcache = cleri__kwcache_new()) == NULL ||
            (pr->expecting = cleri__expecting_new(str)) == NULL)
    {
        cleri_parse_free(pr);
        return NULL;
    }

    pr->re_keywords = grammar->re_keywords;
    pr->re_kw_extra = grammar->re_kw_extra;

    /* do the actual parsing */
    cleri__parse_walk(
            pr,
            pr->tree,
            grammar->start,
            NULL,
            CLERI__EXP_MODE_REQUIRED);

    /* When is_valid is -1, an allocation error has occurred. */
    if (pr->is_valid == -1)
    {
        cleri_parse_free(pr);
        return NULL;
    }

    /* process the parse result */
    end = pr->tree->str + pr->tree->len;

    /* check if we are at the end of the string */
    for (test = end; *test; test++)
    {
        if (!isspace(*test))
        {
            at_end = false;
            break;
        }
    }

    pr->is_valid = at_end;
    pr->pos = (pr->is_valid) ? pr->tree->len : pr->expecting->str - pr->str;

    if (!at_end && pr->expecting->required->cl_obj == NULL)
    {
        if (cleri__expecting_set_mode(
                pr->expecting,
                end,
                CLERI__EXP_MODE_REQUIRED) == -1 ||
            cleri__expecting_update(
                pr->expecting,
                CLERI_END_OF_STATEMENT,
                end) == -1)
        {
            cleri_parse_free(pr);
            return NULL;
        }
    }

    cleri__expecting_combine(pr->expecting);

    pr->expect = pr->expecting->required;

    return pr;
}

/*
 * Destroy parser. (parsing NULL is allowed)
 */
void cleri_parse_free(cleri_parse_t * pr)
{
    cleri__node_free(pr->tree);
    cleri__kwcache_free(pr->kwcache);
    if (pr->expecting != NULL)
    {
        cleri__expecting_free(pr->expecting);
    }
    free(pr);
}

/*
 * Reset expect to start
 */
void cleri_parse_expect_start(cleri_parse_t * pr)
{
    pr->expect = pr->expecting->required;
}

/*
 * Walk a parser object.
 * (recursive function, called from each parse_object function)
 * Returns a node or NULL. (In case of error one should check pr->is_valid)
 */
cleri_node_t * cleri__parse_walk(
        cleri_parse_t * pr,
        cleri_node_t * parent,
        cleri_t * cl_obj,
        cleri_rule_store_t * rule,
        int mode)
{
    /* set parent len to next none white space char */
    while (isspace(*(parent->str + parent->len)))
    {
        parent->len++;
    }

    /* set expecting mode */
    if (cleri__expecting_set_mode(pr->expecting, parent->str, mode) == -1)
    {
        pr->is_valid = -1;
        return NULL;
    }

    /* note that the actual node is returned or NULL but we do not
     * actually need the node. (boolean true/false would be enough)
     */
    return (*cl_obj->parse_object)(pr, parent, cl_obj, rule);
}
