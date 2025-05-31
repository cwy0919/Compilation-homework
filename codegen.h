#ifndef CODEGEN_H
#define CODEGEN_H

typedef enum { 
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, 
    OP_GT, OP_LT, OP_EQ, OP_GE, OP_LE, OP_NE 
} OpType;

typedef struct code_node {
    char* code;
    struct code_node *next;
} CodeNode;

// 公共函数声明
CodeNode* concat_code(CodeNode *a, CodeNode *b);
CodeNode* gen_assign_code(char *var, CodeNode *expr);
CodeNode* gen_if_code(CodeNode *cond, CodeNode *then_part, CodeNode *else_part);
CodeNode* gen_while_code(CodeNode *cond, CodeNode *body);
CodeNode* gen_binop_code(OpType op, CodeNode *left, CodeNode *right);
CodeNode* gen_cond_code(OpType op, CodeNode *left, CodeNode *right);
CodeNode* gen_var_code(char *var);
CodeNode* gen_const_code(int val);
void print_code(CodeNode *code);

extern CodeNode *root_code;

#endif
