%{
#include "parser.tab.h"   // Bison 生成的头文件
#include "codegen.h"      // 三地址代码生成函数
#include <string.h>       //添加字符串函数支持

#undef BEGIN
#undef YY_BEGIN
char *strdup(const char *s);
%}

%option noyywrap          

DIGIT           [0-9]
OCT_DIGIT       [0-7]
HEX_DIGIT       [0-9A-Fa-f]
NON_ZERO_DIGIT  [1-9]
LETTER          [A-Za-z_]
WS              [ \t\n\r]

%%

if              { return IF; }
then            { return THEN; }
else            { return ELSE; }
while           { return WHILE; }
do              { return DO; }
begin           { return BEG; }
end             { return END; }

{LETTER}({LETTER}|{DIGIT})* {
    yylval.str = strdup(yytext);
    return IDN;
}

0               { yylval.num = 0; return DEC; }

0[xX]{HEX_DIGIT}+ {
    yylval.num = (int)strtol(yytext, NULL, 16);
    return HEX;
}

0[xX]{HEX_DIGIT}*[G-Zg-z]{HEX_DIGIT}* {
    yylval.str = strdup(yytext);
    return ILHEX;
}

0{OCT_DIGIT}+   {
    yylval.num = (int)strtol(yytext, NULL, 8);
    return OCT;
}

0{OCT_DIGIT}*[89]{DIGIT}* {
    yylval.str = strdup(yytext);
    return ILOCT;
}

{NON_ZERO_DIGIT}{DIGIT}* {
    yylval.num = atoi(yytext);
    return DEC;
}

"+"             { return ADD; }
"-"             { return SUB; }
"*"             { return MUL; }
"/"             { return DIV; }
">"             { return GT; }
"<"             { return LT; }
"="             { return EQ; }
">="            { return GE; }
"<="            { return LE; }
"<>"            { return NEQ; }
"("             { return SLP; }
")"             { return SRP; }
";"             { return SEMI; }

{WS}+           { /* 忽略空白 */ }
.               { fprintf(stderr, "Error: Invalid character '%s'\n", yytext); }

%%
