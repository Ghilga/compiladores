// Autor: Marcelo Johann

#ifndef HASH_HEADER
#define HASH_HEADER

#define HASH_SIZE 997
enum{
    SYMBOL_IDENTIFIER = 100,
    SYMBOL_LIT_INT,
    SYMBOL_LIT_CHAR,
    SYMBOL_STRING,
    SYMBOL_VARIABLE,
    SYMBOL_FUNCTION,
    SYMBOL_VECTOR,
    SYMBOL_ARGUMENT,
    SYMBOL_LABEL
};

enum{
    DATATYPE_INT = 200,
    DATATYPE_CHAR,
    DATATYPE_FLOAT
};

typedef struct hash_node
{
    int type;
    int dataType;
    int totalArguments;
    char *text;
    struct hash_node * next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
int hashCheckUndeclared(void);
HASH_NODE *makeTemp(void);

#endif

//END