#include <stdio.h>
#include "parser.tab.h"
#include "codegen.h"

extern int yyparse(void);
extern FILE *yyin;
extern CodeNode *root_code;

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Error: Cannot open file %s\n", argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
        printf("Enter program (Ctrl+D to end):\n");
    }
    
    if (yyparse() == 0) {
        printf("\nGenerated Three-Address Code:\n");
        print_code(root_code);
    } else {
        printf("Compilation failed.\n");
    }
    
    if (yyin != stdin) fclose(yyin);
    return 0;
}
