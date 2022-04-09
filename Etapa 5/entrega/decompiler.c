#include <stdio.h>
#include <stdlib.h>
#include "decompiler.h"

FILE *output;

void decompile(AST *root, FILE * outputFile){
    if (root->type != AST_DECL && root->type != AST_FUNC_DECL){
        fprintf(stderr, "Error: This node is not the root node\n");
        return;
    }
    output = outputFile;

    for (int i=0; i < MAX_SONS; i++)
        decompileDecl(root->son[i]);
    
    fclose(output);
}

void decompileDecl(AST *node){
    if (node == 0)
        return;

    switch (node->type){
        case AST_FUNC_DECL: 
        case AST_DECL: 
            for (int i=0; i < MAX_SONS; i++)
                decompileDecl(node->son[i]); 
            break;
        case AST_DECINT:
            fprintf(output,"int "); 
            decompileDecIntChar(node); break;
        case AST_DECCHAR: 
            fprintf(output,"char "); 
            decompileDecIntChar(node); break;
        case AST_DECFLOAT: 
            fprintf(output,"float "); 
            decompileDecFloat(node); break;
        case AST_DEC_INTFUNC: 
            fprintf(output,"int %s", node->symbol->text);
            decompileDecFunc(node); break;
        case AST_DEC_CHARFUNC: 
            fprintf(output,"char %s", node->symbol->text); 
            decompileDecFunc(node); break;
        case AST_DEC_FLOATFUNC:
            fprintf(output,"float %s", node->symbol->text);
            decompileDecFunc(node); break;
        default: fprintf(stderr, "Error: Wrong node type (%d)\n", node->type); return;
    }

    if (node->type == AST_DECINT || node->type == AST_DECCHAR || node->type == AST_DECFLOAT)
        fprintf(output, ";\n");
}

void decompileDecIntChar(AST *node){
    if (node->son[0]->type == AST_ARRAY){
        fprintf(output, "%s[%s]:", node->symbol->text, node->son[0]->symbol->text);
        decompileArray(node->son[0]->son[0]);
    }
    else
        fprintf(output, "%s: %s", node->symbol->text, node->son[0]->symbol->text);
}

void decompileDecFloat(AST *node){
    char *values[2] = { node->son[0]->son[0]->symbol->text, node->son[0]->son[1]->symbol->text };
    fprintf(output, " %s: %s/%s", node->symbol->text, values[0], values[1]);
}

void decompileArray(AST *node){
    if (node == 0)
        return;

    fprintf(output, " %s", node->symbol->text);
    decompileArray(node->son[0]);
}

void decompileDecFunc(AST *node){
    fprintf(output, "(");
    decompileArgList(node->son[0]);
    fprintf(output, ")");
    decompileCmd(node->son[1]);
    fprintf(output, "\n");
    
}

void decompileArgList(AST *node){
    if (node == 0 || node->son[0] == 0)
        return;

    switch (node->son[0]->type){
        case AST_ARGINT: fprintf(output, "int %s", node->son[0]->symbol->text); break;
        case AST_ARGCHAR: fprintf(output, "char %s", node->son[0]->symbol->text); break;
        case AST_ARGFLOAT: fprintf(output, "float %s", node->son[0]->symbol->text); break;
    }
    
    if (node->son[1] != 0)
        fprintf(output, ", ");

    decompileArgList(node->son[1]);
    
}   

void decompileCmd(AST *node){
    if (node == 0)
        return;

    switch (node->type){
        case AST_CMD:
            fprintf(output,"{\n");
            decompileCmd(node->son[0]);
            fprintf(output,"}");
            break;
        case AST_LCMD:
            decompileCmd(node->son[0]);
            fprintf(output,";\n");
            decompileCmd(node->son[1]);
            break;
        case AST_LABEL: 
            fprintf(output,"%s:\n", node->symbol->text);
            decompileCmd(node->son[0]);
            break;
        case AST_ATTR: 
            fprintf(output,"%s =", node->symbol->text);
            decompileExpr(node->son[0]);
            break;
        case AST_ARR_ATTR: 
            fprintf(output,"%s[", node->symbol->text);
            decompileExpr(node->son[0]);
            fprintf(output,"] =");
            decompileExpr(node->son[1]);
            break;
        case AST_PRINT:
            fprintf(output,"print");
            decompileExpr(node->son[0]);
            break;
        case AST_WHILE: 
            fprintf(output,"while");
            decompileExpr(node->son[0]);
            decompileCmd(node->son[1]);
            break;
        case AST_IF_ELSE:
            fprintf(output,"if");
            decompileExpr(node->son[0]);
            fprintf(output," then\n");
            decompileCmd(node->son[1]);
            fprintf(output," else\n");
            decompileCmd(node->son[2]);
            break;
        case AST_IF:
            fprintf(output,"if");
            decompileExpr(node->son[0]);
            fprintf(output," then\n");
            decompileCmd(node->son[1]);
            break;
        case AST_GOTO: 
            fprintf(output,"goto %s", node->son[0]->symbol->text);
            break;
        case AST_RETURN: 
            fprintf(output,"return");
            decompileExpr(node->son[0]);
            break;
    }
}

void decompileExpr(AST *node){
    if (node == 0)
        return;

    switch (node->type){
        case AST_ARR_ELEMENT:
            fprintf(output," %s[",node->symbol->text);
            decompileExpr(node->son[0]);
            fprintf(output,"]");
            break;
        case AST_FUNC_CALL:
            fprintf(output," %s(",node->symbol->text);
            decompileExpr(node->son[0]);
            fprintf(output,")");
            break;
        case AST_EXPR_LIST:
            decompileExpr(node->son[0]);
            if(node->son[1] != 0){
                fprintf(output,",");
                decompileExpr(node->son[1]);
            }
            break;
        case AST_READ: 
            fprintf(output,"read");
            break;
        case AST_PRINTARGS:
            decompileListExpr(node,",");
            break;
        case AST_ADD: 
            decompileListExpr(node,"+");
            break;
        case AST_SUB:
            decompileListExpr(node,"-");
            break;
        case AST_MUL:
            decompileListExpr(node,"*");
            break;
        case AST_DIV:
            decompileListExpr(node,"/");
            break;
        case AST_LESS:
            decompileListExpr(node,"<");
            break;
        case AST_GREATER:
            decompileListExpr(node,">");
            break;
        case AST_DIF:
            decompileListExpr(node,"!=");
            break;
        case AST_EQ:
            decompileListExpr(node,"==");
            break;
        case AST_GE:
            decompileListExpr(node,">=");
            break;
        case AST_LE:
            decompileListExpr(node,"<=");
            break;
        case AST_SYMBOL:
            fprintf(output, " %s", node->symbol->text);
            break;
    }
}

void decompileListExpr(AST *node, char* text){
    decompileExpr(node->son[0]);
    fprintf(output," %s", text);
    decompileExpr(node->son[1]);
}