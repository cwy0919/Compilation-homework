#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "codegen.h"

static int temp_counter = 0;
static int label_counter = 0;
CodeNode *root_code = NULL;

// 内部函数声明
static CodeNode* new_code_node(const char *fmt, ...);

// 公共函数实现
CodeNode* concat_code(CodeNode *a, CodeNode *b) {
    if (!a) return b;
    if (!b) return a;
    
    CodeNode *cur = a;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = b;
    return a;
}

// 内部函数实现
static CodeNode* new_code_node(const char *fmt, ...) {
    va_list args;
    char buf[256];
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    CodeNode *node = (CodeNode*)malloc(sizeof(CodeNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->code = strdup(buf);
    if (!node->code) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    node->next = NULL;
    return node;
}

CodeNode* gen_var_code(char *var) {
    return new_code_node("%s", var);
}

CodeNode* gen_const_code(int val) {
    return new_code_node("%d", val);
}

CodeNode* gen_binop_code(OpType op, CodeNode *left, CodeNode *right) {
    char op_char;
    switch (op) {
        case OP_ADD: op_char = '+'; break;
        case OP_SUB: op_char = '-'; break;
        case OP_MUL: op_char = '*'; break;
        case OP_DIV: op_char = '/'; break;
        default: op_char = '?';
    }
    
    char temp[10];
    sprintf(temp, "t%d", temp_counter++);
    
    CodeNode *code = concat_code(left, right);
    CodeNode *op_node = new_code_node("%s := %s %c %s", 
                                     temp, left->code, op_char, right->code);
    
    return concat_code(code, op_node);
}

CodeNode* gen_cond_code(OpType op, CodeNode *left, CodeNode *right) {
    const char *op_str;
    switch (op) {
        case OP_GT: op_str = ">"; break;
        case OP_LT: op_str = "<"; break;
        case OP_EQ: op_str = "=="; break;
        case OP_GE: op_str = ">="; break;
        case OP_LE: op_str = "<="; break;
        case OP_NE: op_str = "!="; break;
        default: op_str = "??";
    }
    
    char temp[10];
    sprintf(temp, "t%d", temp_counter++);
    
    CodeNode *code = concat_code(left, right);
    CodeNode *cond_node = new_code_node("%s := %s %s %s", 
                                       temp, left->code, op_str, right->code);
    
    return concat_code(code, cond_node);
}

CodeNode* gen_assign_code(char *var, CodeNode *expr) {
    return concat_code(expr, new_code_node("%s := %s", var, expr->code));
}

CodeNode* gen_if_code(CodeNode *cond, CodeNode *then_part, CodeNode *else_part) {
    int true_label = label_counter++;
    int end_label = label_counter++;
    
    CodeNode *code = cond;
    code = concat_code(code, new_code_node("if %s goto L%d", cond->code, true_label));
    code = concat_code(code, new_code_node("goto L%d", end_label));
    code = concat_code(code, new_code_node("L%d:", true_label));
    code = concat_code(code, then_part);
    
    if (else_part) {
        int false_label = label_counter++;
        code = concat_code(code, new_code_node("goto L%d", end_label));
        code = concat_code(code, new_code_node("L%d:", false_label));
        code = concat_code(code, else_part);
    }
    
    code = concat_code(code, new_code_node("L%d:", end_label));
    return code;
}

CodeNode* gen_while_code(CodeNode *cond, CodeNode *body) {
    int start_label = label_counter++;
    int true_label = label_counter++;
    int end_label = label_counter++;
    
    CodeNode *code = new_code_node("L%d:", start_label);
    code = concat_code(code, cond);
    code = concat_code(code, new_code_node("if %s goto L%d", cond->code, true_label));
    code = concat_code(code, new_code_node("goto L%d", end_label));
    code = concat_code(code, new_code_node("L%d:", true_label));
    code = concat_code(code, body);
    code = concat_code(code, new_code_node("goto L%d", start_label));
    code = concat_code(code, new_code_node("L%d:", end_label));
    
    return code;
}

void print_code(CodeNode *code) {
    CodeNode *cur = code;
    while (cur) {
        printf("%s\n", cur->code);
        cur = cur->next;
    }
}
