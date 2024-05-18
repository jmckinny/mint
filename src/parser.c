#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "lexer.h"

typedef struct {
    TokenList *t;
    ExprTree *e;
} Parse_t;

static TokenList *match_token(TokenList *tok_l, Tok_t tok) {
    TokenList expected_token = {0}; /* only for token_to_str call below */
    char *expected, *input, *arg;
    
    if (tok_l->token == tok) {
        return tok_l->next;
    }

    expected_token.token = tok;

    expected = token_to_str(&expected_token);
    input = token_list_to_str(tok_l);
    arg = token_to_str(tok_l);

    fprintf(stderr, "Expected %s from input %s, got %s\n", expected, input, arg);
    
    free(expected);
    free(input);
    free(arg);

    exit(EXIT_FAILURE);
}

static Parse_t *parse_expr(TokenList *tok_l);
static Parse_t *parse_function_expr(TokenList *tok_l);
static Parse_t *parse_parameter_expr(TokenList *tok_l);
static Parse_t *parse_assignment_expr(TokenList *tok_l);
static Parse_t *parse_additive_expr(TokenList *tok_l);

ExprTree *parse(TokenList *tok_l) {
    Parse_t *p = parse_expr(tok_l);
    ExprTree *e = p->e;
    free(p);

    return e;
}

static Parse_t *parse_expr(TokenList *tok_l) {
    if (tok_l == NULL) {
        fprintf(stderr, "Empty input\n");
        exit(EXIT_FAILURE);
    }

    switch(tok_l->token) {
        case TOK_FUN:
            return parse_function_expr(tok_l);
        case TOK_ID:
            return parse_assignment_expr(tok_l);
        default:
            return parse_additive_expr(tok_l);
    }
}

static Parse_t *parse_function_expr(TokenList *tok_l) {
    TokenList *t, *t2, *t3, *t4, *t5, *t6, *t7, *t8;
    Parse_t *p, *p2, *p3; 
    ExprTree *param_exp, *body_exp;
    char *id, *id_cpy;

    t = match_token(tok_l, TOK_FUN);

    id = t->value.id;
    id_cpy = malloc(strlen(id) + 1);
    strcpy(id_cpy, id);

    t2 = match_token(t, TOK_ID);
    t3 = match_token(t2, TOK_LPAREN);

    p2 = parse_parameter_expr(t3);
    t4 = p2->t;
    param_exp = p2->e;
    free(p2);

    t5 = match_token(t4, TOK_RPAREN);
    t6 = match_token(t5, TOK_EQUAL);

    p3 = parse_additive_expr(t6);
    t7 = p3->t;
    body_exp = p3->e;
    free(p3);

    t8 = match_token(t7, TOK_ENDLN);

    p = malloc(sizeof(Parse_t));
    p->t = t8;
    p->e = malloc(sizeof(ExprTree));
    p->e->expr = Fun;
    p->e->value.id = id_cpy;
    p->e->left = param_exp;
    p->e->right = body_exp;

    return p;
}

static Parse_t *parse_parameter_expr(TokenList *tok_l) {
    TokenList *t, *t2, *t3;
    Parse_t *p, *p2;
    ExprTree *param_exp;
    char *id, *id_cpy;

    id = tok_l->value.id;
    id_cpy = malloc(strlen(id) + 1);
    strcpy(id_cpy, id);

    t = match_token(tok_l, TOK_ID);

    p = malloc(sizeof(Parse_t));
    p->t = t;
    p->e->expr = ID;
    p->e->value.id = id_cpy;
    p->e->left = NULL;
    p->e->right = NULL;

    if (t->token == TOK_COMMA) {
        t2 = match_token(t, TOK_COMMA);

        p2 = parse_parameter_expr(t2);
        t3 = p2->t;
        param_exp = p2->e;
        free(p2);

        p->t = t3;
        p->e->right = param_exp;
    }

    return p;
}

static Parse_t *parse_assignment_expr(TokenList *tok_l) {}
static Parse_t *parse_additive_expr(TokenList *tok_l) {}
void free_expr_tree(ExprTree *tree) {}
