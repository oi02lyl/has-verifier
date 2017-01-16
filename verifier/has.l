%{
#include "Artifact.h"
#include "Formula.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
using namespace std;

#include "has.tab.h"
// #define YY_DECL int yylex()
int line_num = 1;
%}
%%
[ \t]              ;
Tasks:             { return TASKS_HD; }
\-\-\-\-\-\-          { return TASK_ED; }
Hierarchy:         { return HIERARCHY_HD; }
Relations:         { return RELATIONS_HD; }
Services:          { return SERVICES_HD; }
Variables:         { return VARIABLES_HD; }
Properties:        { return LTLFO_HD; }
Propositions:      { return PROP_HD; }
Initial: { return GLOBAL_PRECOND_HD; }
Pre:     { return PRECOND_HD; }
Post:    { return POSTCOND_HD; }
Update:      { return SET_HD; }
Open: { return OPENCOND_HD; }
Close: { return CLOSECOND_HD; }
Input:             { return INPUT_HD; }
Return:            { return RETURN_HD; }
as                 { return AS; }
true|True|TRUE|false|False|FALSE { yylval.sval = strdup(yytext); return TF; }
X|G|F              { yylval.sval = strdup(yytext); return UnaryLTLOp; }
U                  { yylval.sval = strdup(yytext); return BinaryLTLOp; }
\+\+|\-\-          { yylval.sval = strdup(yytext); return IR; }        
\:                 { return COLON; }
\(                 { return LP; }
\)                 { return RP; }
"["                { return SLP; }
"]"                { return SRP; }
,                  { return COMMA; }
[0-9]+\.[0-9]+     { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+             { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9\_]+     { yylval.sval = strdup(yytext); return STR; }
\"[a-zA-Z0-9\_]*\" { yylval.sval = strdup(yytext); return STR_CONST; }
&&|\|\||->         { yylval.sval = strdup(yytext); return BOp; }
=|==|!=|<|>|>=|<=  { yylval.sval = strdup(yytext); return EQOp; }
!                  { yylval.sval = strdup(yytext); return NOT; }
\+|\-              { yylval.sval = strdup(yytext); return PLUSMINUS; }
\*|\\              { yylval.sval = strdup(yytext); return TIMESDIVIDE; }
\n                 { ++line_num; }
.                  ;
%%
