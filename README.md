# Compilation-homework
flex+bison
编译过程为
d:
cd D:\flex\_demo
清理命令（可选）
{del *.exe
del parser.tab.*
del lex.yy.c}
bison -d parser.y
flex lexer.l
gcc -o compiler.exe parser.tab.c lex.yy.c codegen.c main.c
compiler.exe test_program2.txt
