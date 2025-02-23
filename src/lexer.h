#ifndef Lexer_h
#define Lexer_h

typedef enum {
    TOK_ADD,
    TOK_COMMA,
    TOK_COMMENT,
    TOK_DIV,
    TOK_DOT,
    TOK_EQUAL,
    TOK_EXP,
    TOK_FLOAT,
    TOK_FUN,
    TOK_ID,
    TOK_INT,
    TOK_LPAREN,
    TOK_MULT,
    TOK_RPAREN,
    TOK_SUB
} Tok_t;

typedef struct token {
    Tok_t token;
    union {
        long int i;
        double d;
        char *id;
    } value;
    struct token *next;
} TokenList;

TokenList *tokenize(const char *input);
void compile_regexs();
void free_regexs();
void free_token_list(TokenList *tok_l);
void print_token_list(const TokenList *tok_l);
char *token_to_str(const TokenList *tok_l);
char *token_list_to_str(const TokenList *tok_l);
char *token_value_to_str(const TokenList *tok_l);
char *token_values_to_str(const TokenList *tok_l);

#endif
