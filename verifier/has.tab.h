/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     COMMA = 258,
     LP = 259,
     RP = 260,
     SLP = 261,
     SRP = 262,
     RELATIONS_HD = 263,
     TASKS_HD = 264,
     TASK_ED = 265,
     HIERARCHY_HD = 266,
     INPUT_HD = 267,
     RETURN_HD = 268,
     SERVICES_HD = 269,
     VARIABLES_HD = 270,
     LTLFO_HD = 271,
     UNDERSCORE = 272,
     PROP_HD = 273,
     CHILDREN_HD = 274,
     GLOBAL_PRECOND_HD = 275,
     PRECOND_HD = 276,
     POSTCOND_HD = 277,
     OPENCOND_HD = 278,
     CLOSECOND_HD = 279,
     SET_HD = 280,
     BOp = 281,
     EQOp = 282,
     NOT = 283,
     TF = 284,
     IR = 285,
     PLUSMINUS = 286,
     TIMESDIVIDE = 287,
     UnaryLTLOp = 288,
     BinaryLTLOp = 289,
     COLON = 290,
     AS = 291,
     INT = 292,
     FLOAT = 293,
     STR = 294,
     STR_CONST = 295,
     NEG = 296
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 345 "has.y"

	int ival;
	float fval;
	char* sval;

    string* string_val;
    vector<string>* vec_str_val;
    pair<string, string>* str_pair_val;
    vector<pair<string, string> >* vec_str_pair_val;
    Service* service_val;
    vector<Service>* vec_service_val;
    Task* task_val;
    vector<Task>* vec_task_val;
    Relation* relation_val;
    DBSchema* dbschema_val;
    Formula* for_val;
    Parameter* expr_val;
    vector<Parameter>* vec_expr_val;
    SetUpdate* set_update_val;
    vector<SetUpdate>* vec_set_update_val;



/* Line 2068 of yacc.c  */
#line 115 "has.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


