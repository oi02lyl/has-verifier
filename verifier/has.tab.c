/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "has.y"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <sys/time.h>

#include "Artifact.h"
#include "Formula.h"
#include "Verifier.h"
#include "SpinVerifier.h"
#include "LivenessProperty.h"
#include "Automaton.h"
// #include "Automaton.h"
using namespace std;

// stuff from flex that bison needs to know about:
int yylex();
extern FILE *yyin;
extern int line_num;
void yyerror(const char *s);
map<string, int> init_type_map();

// the artifact specfication to be verified
Artifact art;

// the automata of HLTL-FO property to be verified
vector<Automaton> atms;

// list of relations in raw format
vector<vector<pair<string, string> > > rel_list;

// variable names mapping
map<string, int> var_map;

// relation/type names mapping
map<string, int> type_map = init_type_map();

// id of a task
map<string, int> task_id_map;

// child id of a task
map<string, int> task_child_id_map;

// service name to id for each task
map<string, int> service_name_map;

// rename map for formulas
vector<map<int, int> > rename_mp;

// map of constants
map<string, int> str_const_map;
map<double, int> num_const_map;

// total number of tasks
int total_tasks;

// total number of variables
int total_vars = 0;

// option to remove sets
bool without_set;

// the property to be verified. properties[0] is the root prop
// properties[i].first is the ltlfo formula
// properties[i].second is the task name
vector<pair<string, string> > properties;
map<string, Formula*> prop_fo_map;

map<string, int> init_type_map() {
    map<string, int> res;
    res["NUM"] = -1;
    res["STR"] = -2;
    return res;
}

int lookup_var(string var) {
    if (var_map.count(var) == 0)
        var_map[var] = total_vars++;
    return var_map[var];
}

vector<int> lookup_var(vector<string> vars) {
    int N = vars.size();
    vector<int> res(N, 0);
    for (int i = 0; i < N; i++)
        res[i] = lookup_var(vars[i]);
    return res;
}

vector<pair<int, int> > lookup_var(vector<pair<string, string> > vars) {
    int N = vars.size();
    vector<pair<int, int> > res(N, pair<int, int>(0, 0));
    for (int i = 0; i < N; i++) {
        res[i].first = lookup_var(vars[i].first);
        res[i].second = lookup_var(vars[i].second);
    }
    return res;
}

Parameter rename_parameter(Formula* form, Parameter pa, map<int, int>& mp) {
    if (pa.is_const || pa.is_wildcard)
        return pa;
    else {
        Parameter res = pa;
        if (mp.count(res.id) > 0)
            res.id = mp[res.id];
        else {
            string var_name;
            for (auto kv : var_map)
                if (kv.second == res.id)
                    var_name = kv.first;

            printf("%s\nVariable %s does not exist!\n", form->to_string().c_str(), var_name.c_str());
            exit(0);
        }
        return res;
    }
}

void rename_with_map(int& name, map<int, int>& mp) {
    if (mp.count(name) == 0) {
        string var_name = "";
        for (auto kv : var_map) {
            if (kv.second == name) {
                var_name = kv.first;
                break;
            }
        }
        printf("Name %s does not exist!\n", var_name.c_str());
        exit(0);
    }
    name = mp[name];
}

void rename_form(Formula* root, Formula* form, map<int, int>& mp) {
    if (form->is_internal()) {
        Internal* intl = (Internal*) form;
        for (int i = 0; i < (int) intl->paras.size(); i++)
            rename_form(root, intl->paras[i], mp);
    } else if (form->is_relation()) {
        RelationTerm* rel = (RelationTerm*) form;
        for (int i = 0; i < (int) rel->paras.size(); i++)
            rel->paras[i] = rename_parameter(root, rel->paras[i], mp);
    } else if (form->is_cmp()) {
        CmpTerm* cmp = (CmpTerm*) form;
        cmp->p1 = rename_parameter(root, cmp->p1, mp);
        cmp->p2 = rename_parameter(root, cmp->p2, mp);
    }

}


void rename_form(Formula* form, map<int, int>& mp) {
    rename_form(form, form, mp);
}

unordered_set<Formula*> renamed_forms;

void rename_form(Formula* form, int task_id) {
    if (renamed_forms.count(form) == 0) {
        rename_form(form, form, rename_mp[task_id]);
        renamed_forms.insert(form);
    }
}

void parse_ltl3ba_formula(int task_id, string& form, vector<string>& prop_names, vector<vector<AProp*> >& aprops) {
    if (form == "t")
        return;         
    // split by disjunct
    int len = form.length();
    for (int i = 0; i < len; i++) {
        string dis_str = "";

        while (i + 1 < len && form[i + 1] != '|') {
            if (form[i + 1] != '&' && form[i + 1] != '(' && form[i + 1] != ')')
                dis_str += form[i + 1];
            i++;
        }
        stringstream ss(dis_str);
        string lit;
        vector<AProp*> disjunct;

        while (ss >> lit) {
            int prop_id = -1;
            bool negated = false;
            if (lit[0] == '!') {
                prop_id = atoi(lit.substr(1).c_str());
                negated = true;
            } else
                prop_id = atoi(lit.c_str());

            string name = prop_names[prop_id];
            // remove " char
            name = name.substr(1, (int) name.length() - 2);

            if (prop_fo_map.count(name) > 0) {
                // subformula
                Formula* form = prop_fo_map[name];
                rename_form(form, task_id);
                APropFO* prop = new APropFO(form);
                prop->negated = negated;
                disjunct.push_back(prop);
            } else if (name.substr(0, 5) == "prop_") {
                // property of child task local run
                int aid = atoi(name.substr(5).c_str());
                int task_id = atms[aid].taskid;
                string task_name = art.tasks[task_id].name;
                int cid = task_child_id_map[task_name];
                APropSubForm* prop = new APropSubForm(cid, aid);
                prop->negated = negated;
                disjunct.push_back(prop);
            } else if (name.substr(0, 5) == "open_") {
                // opening child
                string task_name = name.substr(5);
                int cid = task_child_id_map[task_name];
                APropChild* prop = new APropChild(cid, true);
                prop->negated = negated;
                disjunct.push_back(prop);
            } else if (name.substr(0, 6) == "close_") {
                // closing child
                string task_name = name.substr(6);
                int cid = task_child_id_map[task_name];
                APropChild* prop = new APropChild(cid, false);
                prop->negated = negated;
                disjunct.push_back(prop);
            } else if (name.substr(0, 8) == "service_") {
                // service call
                int sid = service_name_map[name];
                APropService* prop = new APropService(sid);
                prop->negated = negated;
                disjunct.push_back(prop);
            }
        }

        if (!disjunct.empty())
            aprops.push_back(disjunct);
    }
}

void parse_ltl3ba_output(string task_name, vector<string>& lines, Automaton& atm) {
    // build child id map
    if (task_child_id_map.empty()) {
        for (Task& task : art.tasks)
            for (int i = 0; i < (int) task.children.size(); i++) {
                string child_name = art.tasks[task.children[i]].name;
                task_child_id_map[child_name] = i;
            }
    }

    // lookup task name
    if (task_id_map.count(task_name) == 0) {
        printf("Task %s not found in HLTL-FO property\n", task_name.c_str());
        exit(0);
    } else {
        atm.taskid = task_id_map[task_name];
        // build service name map
        Task& task = art.tasks[atm.taskid];
        for (int i = 0; i < (int) task.services.size(); i++)
            service_name_map[task.services[i].name] = i;
    }

    int num_prop = 0;
    int start_state = 0;
    vector<string> prop_names;
    
    for (int line_num = 0; line_num < (int) lines.size(); line_num++) {
        string line = lines[line_num];
        stringstream ss(line);
        string hd;
        ss >> hd;

        if (hd == "States:") {
            ss >> atm.num_states;
            atm.states = vector<AState>(atm.num_states, AState());
            atm.transition = vector<vector<pair<vector<AProp*>, int> > >(atm.num_states, 
                                vector<pair<vector<AProp*>, int> >());
        } else if (hd == "Start:") {
            ss >> start_state;
            if (start_state != 0) {
                printf("bug: start_state != 0\n");
                exit(0);
            }
        } else if (hd == "AP:") {
            ss >> num_prop;
            for (int i = 0; i < num_prop; i++) {
                string name;
                ss >> name;
                prop_names.push_back(name);
            }
        } else if (hd == "State:") {
            int state_id;
            string state_name;
            bool accept = false;
            ss >> state_id >> state_name;
            string acc_sig;
            if (ss >> acc_sig)
                accept = true;
            atm.states[state_id].name = state_name;
            atm.states[state_id].id = state_id;
            atm.states[state_id].accept = accept;
            while (line_num + 1 < (int) lines.size() && lines[line_num + 1][0] == ' ') {
                line_num++;
                line = lines[line_num];
                // cout << line << endl;
                int start_id = -1, end_id = -1;
                for (int i = 0; i < (int) line.length(); i++) {
                    if (line[i] == '[')
                        start_id = i;
                    else if (line[i] == ']')
                        end_id = i;
                }
                
                if (start_id >= 0 && end_id >= 0) {
                    string cond = line.substr(start_id, end_id - start_id - 1);
                    int next_state = atoi(line.substr(end_id + 2).c_str());
                    vector<vector<AProp*> > aprops;
                    parse_ltl3ba_formula(atm.taskid, cond, prop_names, aprops);
                    for (vector<AProp*> aprop : aprops)
                        atm.transition[state_id].push_back(
                        pair<vector<AProp*>, int>(aprop, next_state) );
                }
            }
        }
    }
}



/* Line 268 of yacc.c  */
#line 408 "has.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
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



/* Line 293 of yacc.c  */
#line 509 "has.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 521 "has.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  7
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   148

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  42
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNRULES -- Number of states.  */
#define YYNSTATES  132

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   296

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     8,    10,    16,    19,    21,    26,    29,
      31,    47,    51,    53,    56,    59,    61,    62,    71,    75,
      77,    78,    82,    84,    85,    89,    91,    95,    97,    98,
     102,   106,   110,   113,   115,   119,   121,   126,   130,   132,
     133,   135,   137,   139,   141,   145,   149,   153,   157,   159,
     160,   165,   171,   177,   180,   184,   188,   192,   195,   201,
     203,   208
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      43,     0,    -1,    44,    16,    64,    65,    -1,    44,    -1,
       8,    45,    11,    56,    47,    -1,    46,    45,    -1,    46,
      -1,    39,     4,    49,     5,    -1,    47,    48,    -1,    48,
      -1,    39,    35,    15,    49,    12,    54,    13,    54,    23,
      58,    24,    58,    14,    51,    10,    -1,    49,     3,    50,
      -1,    50,    -1,    39,    39,    -1,    51,    52,    -1,    52,
      -1,    -1,    39,    35,    21,    58,    22,    58,    25,    62,
      -1,    53,     3,    39,    -1,    39,    -1,    -1,    54,     3,
      55,    -1,    55,    -1,    -1,    39,    36,    39,    -1,    39,
      -1,    56,     3,    57,    -1,    57,    -1,    -1,    39,    26,
      39,    -1,     4,    58,     5,    -1,    58,    26,    58,    -1,
      28,    58,    -1,    59,    -1,    61,    27,    61,    -1,    29,
      -1,    39,     4,    60,     5,    -1,    60,     3,    61,    -1,
      61,    -1,    -1,    37,    -1,    38,    -1,    40,    -1,    39,
      -1,    61,    31,    61,    -1,    61,    32,    61,    -1,     4,
      61,     5,    -1,    62,     3,    63,    -1,    63,    -1,    -1,
      39,     4,    53,     5,    -1,    39,    30,     4,    53,     5,
      -1,    30,    39,     4,    53,     5,    -1,    33,    64,    -1,
      64,    34,    64,    -1,    64,    26,    64,    -1,     4,    64,
       5,    -1,    28,    64,    -1,    39,    35,     6,    64,     7,
      -1,    39,    -1,    65,    39,    35,    58,    -1,    18,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   402,   402,   425,   428,   559,   560,   563,   580,   581,
     584,   614,   615,   618,   621,   622,   623,   626,   635,   636,
     637,   640,   641,   642,   645,   646,   650,   651,   652,   655,
     659,   660,   666,   668,   671,   682,   687,   695,   696,   697,
     700,   710,   720,   729,   746,   749,   751,   756,   757,   758,
     761,   768,   775,   785,   790,   795,   800,   805,   810,   819,
     823,   827
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COMMA", "LP", "RP", "SLP", "SRP",
  "RELATIONS_HD", "TASKS_HD", "TASK_ED", "HIERARCHY_HD", "INPUT_HD",
  "RETURN_HD", "SERVICES_HD", "VARIABLES_HD", "LTLFO_HD", "UNDERSCORE",
  "PROP_HD", "CHILDREN_HD", "GLOBAL_PRECOND_HD", "PRECOND_HD",
  "POSTCOND_HD", "OPENCOND_HD", "CLOSECOND_HD", "SET_HD", "BOp", "EQOp",
  "NOT", "TF", "IR", "PLUSMINUS", "TIMESDIVIDE", "UnaryLTLOp",
  "BinaryLTLOp", "COLON", "AS", "INT", "FLOAT", "STR", "STR_CONST", "NEG",
  "$accept", "has", "specification", "relations", "relation", "task_list",
  "task", "var_decls", "decl", "service_list", "service", "var_list",
  "var_map_list", "var_map", "edge_list", "edge", "form", "term",
  "expr_list", "expr", "set_update_list", "set_update", "ltlfo", "fo_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    42,    43,    43,    44,    45,    45,    46,    47,    47,
      48,    49,    49,    50,    51,    51,    51,    52,    53,    53,
      53,    54,    54,    54,    55,    55,    56,    56,    56,    57,
      58,    58,    58,    58,    59,    59,    59,    60,    60,    60,
      61,    61,    61,    61,    61,    61,    61,    62,    62,    62,
      63,    63,    63,    64,    64,    64,    64,    64,    64,    64,
      65,    65
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     1,     5,     2,     1,     4,     2,     1,
      15,     3,     1,     2,     2,     1,     0,     8,     3,     1,
       0,     3,     1,     0,     3,     1,     3,     1,     0,     3,
       3,     3,     2,     1,     3,     1,     4,     3,     1,     0,
       1,     1,     1,     1,     3,     3,     3,     3,     1,     0,
       4,     5,     5,     2,     3,     3,     3,     2,     5,     1,
       4,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     3,     0,     0,     6,     1,     0,     0,
      28,     5,     0,     0,     0,    59,     0,     0,     0,    12,
       0,     0,    27,     0,    57,    53,     0,    61,     0,     0,
       2,    13,     0,     7,     0,     0,     0,     4,     9,    56,
       0,    55,    54,     0,    11,    29,    26,     0,     8,     0,
       0,     0,    58,     0,     0,    35,    40,    41,    43,    42,
      60,    33,     0,     0,     0,     0,    32,    39,     0,     0,
       0,     0,    23,    30,    46,     0,    43,     0,    38,    31,
      34,    44,    45,    25,     0,    22,     0,     0,    36,     0,
       0,    23,    37,    24,    21,     0,     0,     0,     0,     0,
      16,     0,     0,    15,     0,    10,    14,     0,     0,     0,
       0,    49,     0,     0,    17,    48,     0,    20,     0,     0,
      20,    19,     0,    20,    47,     0,     0,    50,     0,    52,
      18,    51
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     5,     6,    37,    38,    18,    19,   102,
     103,   122,    84,    85,    21,    22,    60,    61,    77,    62,
     114,   115,    16,    30
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -54
static const yytype_int8 yypact[] =
{
      40,   -27,    24,    39,    63,    62,   -27,   -54,    18,    60,
      64,   -54,    18,    18,    18,    49,    -5,    65,    77,   -54,
      79,     5,   -54,    -1,    36,    36,   100,   -54,    18,    18,
      68,   -54,    60,   -54,    69,    64,    74,    71,   -54,   -54,
      18,    78,    36,    80,   -54,   -54,   -54,    96,   -54,    59,
      -2,    60,   -54,    -2,    -2,   -54,   -54,   -54,   109,   -54,
      88,   -54,    56,    16,     4,    45,    88,     2,    -2,     2,
       2,     2,    81,   -54,   -54,     2,   -54,    86,   -14,   -54,
     -14,    84,   -54,    82,     7,   -54,    47,     2,   -54,    83,
      81,    81,   -14,   -54,   -54,    11,    -2,    66,    -2,    -3,
      85,    90,    -7,   -54,    98,   -54,   -54,    -2,    27,    -2,
      76,   -23,    87,     1,   114,   -54,   117,    89,   119,   -23,
      89,   -54,    91,    89,   -54,    92,    93,   -54,    95,   -54,
     -54,   -54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -54,   -54,   -54,   121,   -54,   -54,    94,    97,   101,   -54,
      28,   -52,    38,    44,   -54,   102,   -53,   -54,   -54,    -6,
     -54,    17,    46,   -54
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      64,    66,    53,   105,    39,   117,    75,   112,    35,    73,
      90,   100,     4,    27,    90,    79,   113,    70,    71,    32,
      91,    28,    12,    68,     7,    28,    54,    55,    72,    29,
      68,   118,   101,    29,    96,    56,    57,    58,    59,    56,
      57,    76,    59,    97,    36,    99,    13,    65,     1,   109,
      74,    14,    74,    68,   108,     8,   110,    15,    23,    24,
      25,    78,    28,    80,    81,    82,    52,     9,   125,    86,
      29,   128,    69,    10,    41,    42,    70,    71,    70,    71,
      32,    92,    33,    69,    26,    28,    49,    70,    71,    87,
      98,    88,    68,    29,   126,   126,   127,   129,   126,    17,
     131,   111,    68,    20,    31,    34,    40,    43,    45,    47,
      36,    51,    29,    67,    68,    50,    71,   119,    89,   107,
      83,   120,    93,   123,   101,   104,   116,    11,   121,    95,
     106,    48,   130,    44,    94,     0,   124,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-54))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int8 yycheck[] =
{
      53,    54,     4,    10,     5,     4,     4,    30,     3,     5,
       3,    14,    39,    18,     3,    68,    39,    31,    32,     3,
      13,    26,     4,    26,     0,    26,    28,    29,    12,    34,
      26,    30,    39,    34,    23,    37,    38,    39,    40,    37,
      38,    39,    40,    96,    39,    98,    28,    53,     8,    22,
       5,    33,     5,    26,   107,    16,   109,    39,    12,    13,
      14,    67,    26,    69,    70,    71,     7,     4,   120,    75,
      34,   123,    27,    11,    28,    29,    31,    32,    31,    32,
       3,    87,     5,    27,    35,    26,    40,    31,    32,     3,
      24,     5,    26,    34,     3,     3,     5,     5,     3,    39,
       5,    25,    26,    39,    39,    26,     6,    39,    39,    35,
      39,    15,    34,     4,    26,    35,    32,     3,    36,    21,
      39,     4,    39,     4,    39,    35,    39,     6,    39,    91,
     102,    37,    39,    32,    90,    -1,   119,    35,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     8,    43,    44,    39,    45,    46,     0,    16,     4,
      11,    45,     4,    28,    33,    39,    64,    39,    49,    50,
      39,    56,    57,    64,    64,    64,    35,    18,    26,    34,
      65,    39,     3,     5,    26,     3,    39,    47,    48,     5,
       6,    64,    64,    39,    50,    39,    57,    35,    48,    64,
      35,    15,     7,     4,    28,    29,    37,    38,    39,    40,
      58,    59,    61,    49,    58,    61,    58,     4,    26,    27,
      31,    32,    12,     5,     5,     4,    39,    60,    61,    58,
      61,    61,    61,    39,    54,    55,    61,     3,     5,    36,
       3,    13,    61,    39,    55,    54,    23,    58,    24,    58,
      14,    39,    51,    52,    35,    10,    52,    21,    58,    22,
      58,    25,    30,    39,    62,    63,    39,     4,    30,     3,
       4,    39,    53,     4,    63,    53,     3,     5,    53,     5,
      39,     5
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1806 of yacc.c  */
#line 403 "has.y"
    { 
        // to handle properties
        properties.push_back(pair<string, string>(*(yyvsp[(3) - (4)].string_val), art.tasks[0].name));
        
        for (int i = 0; i < (int) properties.size(); i++) {
            FILE* fout = fopen("prop.txt", "w");
            fprintf(fout, "%s", properties[i].first.c_str());
            fclose(fout);
            system("./ltl3ba-1.1.3/ltl3ba -H3 -F prop.txt > prop_out.txt");
            ifstream fin("prop_out.txt");
            vector<string> lines;
            string line;
            while (getline(fin, line))
                lines.push_back(line);
            fin.close();
            Automaton atm(-1);
            parse_ltl3ba_output(properties[i].second, lines, atm);
            atms.push_back(atm);
        }
        system("rm prop.txt prop_out.txt");
        reverse(atms.begin(), atms.end());
    }
    break;

  case 4:

/* Line 1806 of yacc.c  */
#line 429 "has.y"
    { 
        // set relations types
        DBSchema& d = art.db;
        int num_rels = rel_list.size();
        for (int i = 0; i < num_rels; i++) {
            int sz = rel_list[i].size();
            // maybe it should start from 0?
            for (int j = 1; j < sz; j++)
                d.relations[i].types[j] = type_map[rel_list[i][j].first];
        }
        
        // global pre-condition
        // art.global_pre_cond = $4;
        art.global_pre_cond = new ConstTerm(true);

        // tasks
        art.tasks = *(yyvsp[(5) - (5)].vec_task_val);

        // task hierarchy
        map<string, int> task_name_mp;
        int num_tasks = art.tasks.size();
        for (int i = 0; i < num_tasks; i++)
            task_name_mp[art.tasks[i].name] = i;

        // printf("%d\n", (int) $4->size());        
        vector<int> parent_task(num_tasks, -1);
        int num_edge = (yyvsp[(4) - (5)].vec_str_pair_val)->size();
        for (int i = 0; i < num_edge; i++) {
            pair<string, string> edge = (yyvsp[(4) - (5)].vec_str_pair_val)->at(i);
            // printf("%s %s\n", edge.first.c_str(), edge.second.c_str());
            if (task_name_mp.count(edge.first) && task_name_mp.count(edge.second)) {
                int par = task_name_mp[edge.first];
                int ch = task_name_mp[edge.second];
                art.tasks[par].children.push_back(ch);
                parent_task[ch] = par;
            }
        }

        // renaming variables of each task
        rename_mp = vector<map<int, int> >(num_tasks, map<int, int>());
        for (int task_id = 0; task_id < num_tasks; task_id++) {
            Task& task = art.tasks[task_id];
            for (int i = 0; i < task.num_var; i++) {
                rename_mp[task_id][task.vars[i]] = i;
                task.vars[i] = i;
            }
        }

        for (int task_id = 0; task_id < num_tasks; task_id++) {
            Task& task = art.tasks[task_id];
            if (parent_task[task_id] >= 0)
                rename_form(task.open_cond, rename_mp[parent_task[task_id]]);
            rename_form(task.close_cond, rename_mp[task_id]);
        
            for (int i = 0; i < (int) task.input_vars.size(); i++) {
                rename_with_map(task.input_vars[i].first, rename_mp[parent_task[task_id]]);
                rename_with_map(task.input_vars[i].second, rename_mp[task_id]);
            }
            
            for (int i = 0; i < (int) task.return_vars.size(); i++) {
                rename_with_map(task.return_vars[i].first, rename_mp[task_id]);
                rename_with_map(task.return_vars[i].second, rename_mp[parent_task[task_id]]);
            }

            for (int i = 0; i < (int) task.services.size(); i++) {
                Service& ser = task.services[i];
                rename_form(ser.pre_cond, rename_mp[task_id]);
                rename_form(ser.post_cond, rename_mp[task_id]);

                vector<SetUpdate> new_set_update;
                for (int j = 0; j < (int) ser.set_update.size(); j++) {
                    SetUpdate& su = ser.set_update[j];
                    for (int k = 0; k < (int) su.vars.size(); k++) {
                        rename_with_map(su.vars[k], rename_mp[task_id]);
                    }
                    if (su.type == Prop || su.type == Write) {
                        if (su.type == Prop)
                            ser.var_prop = su.vars;
                        else {
                            set<int> var_write(su.vars.begin(), su.vars.end());
                            for (int k = 0; k < (int) task.vars.size(); k++)
                                if (var_write.count(task.vars[k]) == 0)
                                    ser.var_prop.push_back(task.vars[k]);
                        }
                    } else
                        if (!without_set)
                           new_set_update.push_back(su);
                }
                ser.set_update = new_set_update;
            }
            // rename setid
            map<int, int> setid_rename_mp;
            for (int i = 0; i < (int) task.services.size(); i++) {
                Service& ser = task.services[i];
                for (int j = 0; j < (int) ser.set_update.size(); j++) {
                    SetUpdate& su = ser.set_update[j];
                    if (setid_rename_mp.count(su.setid) == 0) {
                        int sid = setid_rename_mp.size();
                        setid_rename_mp[su.setid] = sid;
                        ser.set_update[j].setid = sid;
                    } else
                        rename_with_map(su.setid, setid_rename_mp);
                }
            }
        }

        // collect constants
        if (str_const_map.count("\"\"") == 0) {
            int N = str_const_map.size();
            str_const_map["\"\""] = N;
        }

        if (num_const_map.count(0) == 0) {
            int N = num_const_map.size();
            num_const_map[0] = N;
        }

        art.str_consts = vector<string>(str_const_map.size(), "");
        for (map<string, int>::iterator it = str_const_map.begin();
             it != str_const_map.end(); it++)
            art.str_consts[it->second] = it->first;

        art.num_consts = vector<double>(num_const_map.size(), 0);
        for (map<double, int>::iterator it = num_const_map.begin();
             it != num_const_map.end(); it++) 
            art.num_consts[it->second] = it->first;
        // cout << "done with a has file!" << endl; 
    }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 563 "has.y"
    { 
        rel_list.push_back(*(yyvsp[(3) - (4)].vec_str_pair_val));
        type_map[(yyvsp[(1) - (4)].sval)] = art.db.num_rels;

        DBSchema& d = art.db;
        d.num_rels++;
        Relation rel;
        rel.name = (yyvsp[(1) - (4)].sval);
        rel.arity = (yyvsp[(3) - (4)].vec_str_pair_val)->size();
        int n = rel.arity;
        rel.attr_names = vector<string>(n, "");
        rel.types = vector<int>(n, -1);
        for (int i = 0; i < n; i++)
            rel.attr_names[i] = (*(yyvsp[(3) - (4)].vec_str_pair_val))[i].second;
        d.relations.push_back(rel);
    }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 580 "has.y"
    { (yyval.vec_task_val) = (yyvsp[(1) - (2)].vec_task_val); (yyval.vec_task_val)->push_back(*(yyvsp[(2) - (2)].task_val)); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 581 "has.y"
    { (yyval.vec_task_val) = new vector<Task>(1, *(yyvsp[(1) - (1)].task_val)); }
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 584 "has.y"
    {
        (yyval.task_val) = new Task();
        (yyval.task_val)->name = (yyvsp[(1) - (15)].sval);
        task_id_map[(yyval.task_val)->name] = total_tasks++;

        // variables
        (yyval.task_val)->num_var = (yyvsp[(4) - (15)].vec_str_pair_val)->size();
        for (int i = 0; i < (yyval.task_val)->num_var; i++) {
            (yyval.task_val)->vars.push_back(lookup_var((*(yyvsp[(4) - (15)].vec_str_pair_val))[i].second));
            string type_name = (*(yyvsp[(4) - (15)].vec_str_pair_val))[i].first;
            if (type_map.count(type_name) == 0) {
                printf("Type %s not defined!\n", type_name.c_str());
                exit(0);
            }
            (yyval.task_val)->var_types.push_back(type_map[type_name]);
        }
        
        // input and return
        (yyval.task_val)->input_vars = lookup_var(*(yyvsp[(6) - (15)].vec_str_pair_val));
        (yyval.task_val)->return_vars = lookup_var(*(yyvsp[(8) - (15)].vec_str_pair_val));

        // opening and closing
        (yyval.task_val)->open_cond = (yyvsp[(10) - (15)].for_val);
        (yyval.task_val)->close_cond = (yyvsp[(12) - (15)].for_val);

        // services
        (yyval.task_val)->services = *(yyvsp[(14) - (15)].vec_service_val);
    }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 614 "has.y"
    { (yyval.vec_str_pair_val) = (yyvsp[(1) - (3)].vec_str_pair_val); (yyval.vec_str_pair_val)->push_back(*(yyvsp[(3) - (3)].str_pair_val)); }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 615 "has.y"
    { (yyval.vec_str_pair_val) = new vector<pair<string, string> >(1, *(yyvsp[(1) - (1)].str_pair_val)); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 618 "has.y"
    { (yyval.str_pair_val) = new pair<string, string>((yyvsp[(1) - (2)].sval), (yyvsp[(2) - (2)].sval)); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 621 "has.y"
    { (yyval.vec_service_val) = (yyvsp[(1) - (2)].vec_service_val); (yyval.vec_service_val)->push_back(*(yyvsp[(2) - (2)].service_val)); }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 622 "has.y"
    { (yyval.vec_service_val) = new vector<Service>(1, *((yyvsp[(1) - (1)].service_val))); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 623 "has.y"
    { (yyval.vec_service_val) = new vector<Service>(); }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 626 "has.y"
    {
        (yyval.service_val) = new Service();
        (yyval.service_val)->name = (yyvsp[(1) - (8)].sval);
        (yyval.service_val)->pre_cond = (yyvsp[(4) - (8)].for_val);
        (yyval.service_val)->post_cond = (yyvsp[(6) - (8)].for_val);
        (yyval.service_val)->set_update = *(yyvsp[(8) - (8)].vec_set_update_val);
    }
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 635 "has.y"
    { (yyval.vec_str_val) = (yyvsp[(1) - (3)].vec_str_val); (yyval.vec_str_val)->push_back((yyvsp[(3) - (3)].sval)); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 636 "has.y"
    { (yyval.vec_str_val) = new vector<string>(1, (yyvsp[(1) - (1)].sval)); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 637 "has.y"
    { (yyval.vec_str_val) = new vector<string>(); }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 640 "has.y"
    { (yyval.vec_str_pair_val) = (yyvsp[(1) - (3)].vec_str_pair_val); (yyval.vec_str_pair_val)->push_back(*(yyvsp[(3) - (3)].str_pair_val)); }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 641 "has.y"
    { (yyval.vec_str_pair_val) = new vector<pair<string, string> >(1, *(yyvsp[(1) - (1)].str_pair_val)); }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 642 "has.y"
    { (yyval.vec_str_pair_val) = new vector<pair<string, string> >(); }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 645 "has.y"
    { (yyval.str_pair_val) = new pair<string, string>((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 646 "has.y"
    { (yyval.str_pair_val) = new pair<string, string>((yyvsp[(1) - (1)].sval), (yyvsp[(1) - (1)].sval)); }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 650 "has.y"
    { (yyval.vec_str_pair_val) = (yyvsp[(1) - (3)].vec_str_pair_val); (yyval.vec_str_pair_val)->push_back(*(yyvsp[(3) - (3)].str_pair_val)); }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 651 "has.y"
    { (yyval.vec_str_pair_val) = new vector<pair<string, string> >(1, *(yyvsp[(1) - (1)].str_pair_val)); }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 652 "has.y"
    { (yyval.vec_str_pair_val) = new vector<pair<string, string> >(); }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 655 "has.y"
    { (yyval.str_pair_val) = new pair<string, string>((yyvsp[(1) - (3)].sval), (yyvsp[(3) - (3)].sval)); }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 659 "has.y"
    { (yyval.for_val) = (yyvsp[(2) - (3)].for_val); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 660 "has.y"
    { 
                    Internal* intern = new Internal();
                    intern->paras.push_back((yyvsp[(1) - (3)].for_val)); 
                    intern->paras.push_back((yyvsp[(3) - (3)].for_val)); 
                    intern->op = (yyvsp[(2) - (3)].sval); 
                    (yyval.for_val) = intern; }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 666 "has.y"
    { Internal* intern = new Internal(); intern->paras.push_back((yyvsp[(2) - (2)].for_val));
               intern->op = "!"; (yyval.for_val) = intern; }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 668 "has.y"
    { (yyval.for_val) = (yyvsp[(1) - (1)].for_val); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 671 "has.y"
    { 
                    if ((yyvsp[(1) - (3)].expr_val) == NULL || (yyvsp[(3) - (3)].expr_val) == NULL) {
                        (yyval.for_val) = new ConstTerm(1); 
                    } else {
                        CmpTerm* term = new CmpTerm();                      
                        term->p1 = *(yyvsp[(1) - (3)].expr_val);
                        term->p2 = *(yyvsp[(3) - (3)].expr_val); 
                        term->equal = (strcmp((yyvsp[(2) - (3)].sval), "==") == 0); 
                        (yyval.for_val) = term;
                    }
                   }
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 682 "has.y"
    { 
        (yyval.for_val) = new ConstTerm(strcmp((yyvsp[(1) - (1)].sval), "true") == 0 ||
                           strcmp((yyvsp[(1) - (1)].sval), "True") == 0 ||
                           strcmp((yyvsp[(1) - (1)].sval), "TRUE") == 0);
    }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 687 "has.y"
    { 
        RelationTerm* term = new RelationTerm(); 
        term->rel_id = type_map[(yyvsp[(1) - (4)].sval)];
        term->paras = *(yyvsp[(3) - (4)].vec_expr_val);
        (yyval.for_val) = term;
    }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 695 "has.y"
    { (yyval.vec_expr_val) = (yyvsp[(1) - (3)].vec_expr_val); (yyval.vec_expr_val)->push_back(*(yyvsp[(3) - (3)].expr_val)); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 696 "has.y"
    { (yyval.vec_expr_val) = new vector<Parameter>(1, *(yyvsp[(1) - (1)].expr_val)); }
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 697 "has.y"
    { (yyval.vec_expr_val) = new vector<Parameter>(); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 700 "has.y"
    { 
        (yyval.expr_val) = new Parameter(); 
        (yyval.expr_val)->is_const = true; 
        (yyval.expr_val)->type = -1;

        if (num_const_map.count((yyvsp[(1) - (1)].ival)) == 0) {
            int N = num_const_map.size();
            num_const_map[(yyvsp[(1) - (1)].ival)] = N;
        }
        (yyval.expr_val)->id = num_const_map[(yyvsp[(1) - (1)].ival)]; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 710 "has.y"
    { 
        (yyval.expr_val) = new Parameter(); 
        (yyval.expr_val)->is_const = true; 
        (yyval.expr_val)->type = -1;

        if (num_const_map.count((yyvsp[(1) - (1)].fval)) == 0) {
            int N = num_const_map.size();
            num_const_map[(yyvsp[(1) - (1)].fval)] = N;
        }
        (yyval.expr_val)->id = num_const_map[(yyvsp[(1) - (1)].fval)]; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 720 "has.y"
    { 
        (yyval.expr_val) = new Parameter();
        (yyval.expr_val)->is_const = true;
        (yyval.expr_val)->type = -2;
        if (str_const_map.count((yyvsp[(1) - (1)].sval)) == 0) {
            int N = str_const_map.size();
            str_const_map[(yyvsp[(1) - (1)].sval)] = N;
        }
        (yyval.expr_val)->id = str_const_map[(yyvsp[(1) - (1)].sval)]; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 729 "has.y"
    { 
        (yyval.expr_val) = new Parameter();
        if (strcmp((yyvsp[(1) - (1)].sval), "_") == 0) {
            (yyval.expr_val)->is_wildcard = true;
            (yyval.expr_val)->id = -1;
        } else {
            (yyval.expr_val)->is_wildcard = false;
            if (strcmp((yyvsp[(1) - (1)].sval), "NULL") == 0 || strcmp((yyvsp[(1) - (1)].sval), "null") == 0) {
                (yyval.expr_val)->is_const = true;
                (yyval.expr_val)->is_null = true;
                (yyval.expr_val)->type = -3;
            } else {
                (yyval.expr_val)->is_const = false; 
                (yyval.expr_val)->is_null = false;
                (yyval.expr_val)->id = lookup_var((yyvsp[(1) - (1)].sval)); 
            }
        } }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 746 "has.y"
    {
        // arithmetic is ignored
        (yyval.expr_val) = NULL; }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 749 "has.y"
    {
        (yyval.expr_val) = NULL; }
    break;

  case 46:

/* Line 1806 of yacc.c  */
#line 751 "has.y"
    {
        (yyval.expr_val) = (yyvsp[(2) - (3)].expr_val); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 756 "has.y"
    { (yyval.vec_set_update_val) = (yyvsp[(1) - (3)].vec_set_update_val); (yyval.vec_set_update_val)->push_back(*(yyvsp[(3) - (3)].set_update_val)); }
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 757 "has.y"
    { (yyval.vec_set_update_val) = new vector<SetUpdate>(1, *(yyvsp[(1) - (1)].set_update_val)); }
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 758 "has.y"
    { (yyval.vec_set_update_val) = new vector<SetUpdate>(); }
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 761 "has.y"
    { 
       (yyval.set_update_val) = new SetUpdate();
       (yyval.set_update_val)->vars = lookup_var(*(yyvsp[(3) - (4)].vec_str_val));
       if (strcmp((yyvsp[(1) - (4)].sval), "Prop") == 0)
           (yyval.set_update_val)->type = Prop;
       else
           (yyval.set_update_val)->type = Write; }
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 769 "has.y"
    {   (yyval.set_update_val) = new SetUpdate();
        (yyval.set_update_val)->vars = lookup_var(*(yyvsp[(4) - (5)].vec_str_val));
        if (strcmp((yyvsp[(2) - (5)].sval), "++") == 0)
            (yyval.set_update_val)->type = InsertAfter;
        (yyval.set_update_val)->setid = lookup_var((yyvsp[(1) - (5)].sval));
    }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 776 "has.y"
    {   (yyval.set_update_val) = new SetUpdate();
        (yyval.set_update_val)->vars = lookup_var(*(yyvsp[(4) - (5)].vec_str_val));
        if (strcmp((yyvsp[(1) - (5)].sval), "++") == 0)
            (yyval.set_update_val)->type = InsertBefore;
        else
            (yyval.set_update_val)->type = Retrieve;
        (yyval.set_update_val)->setid = lookup_var((yyvsp[(2) - (5)].sval));
    }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 786 "has.y"
    { 
         (yyval.string_val) = new string();   
         (yyval.string_val)->assign(string((yyvsp[(1) - (2)].sval)) + " " + *(yyvsp[(2) - (2)].string_val));
       }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 791 "has.y"
    { 
         (yyval.string_val) = new string();
         (yyval.string_val)->assign(*(yyvsp[(1) - (3)].string_val) + " " + string((yyvsp[(2) - (3)].sval)) + " " + *(yyvsp[(3) - (3)].string_val));
       }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 796 "has.y"
    { 
         (yyval.string_val) = new string();
         (yyval.string_val)->assign(*(yyvsp[(1) - (3)].string_val) + " " + string((yyvsp[(2) - (3)].sval)) + " " + *(yyvsp[(3) - (3)].string_val));
       }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 801 "has.y"
    { 
         (yyval.string_val) = new string();
         (yyval.string_val)->assign("(" + *(yyvsp[(2) - (3)].string_val) + ")");
       }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 806 "has.y"
    { 
         (yyval.string_val) = new string();   
         (yyval.string_val)->assign(string((yyvsp[(1) - (2)].sval)) + " " + *(yyvsp[(2) - (2)].string_val));
       }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 811 "has.y"
    {
         int prop_id = properties.size();
         pair<string, string> new_prop(*(yyvsp[(4) - (5)].string_val), (yyvsp[(1) - (5)].sval));
         properties.push_back(new_prop);
         char prop_name[30];
         sprintf(prop_name, "prop_%d", prop_id);
         (yyval.string_val) = new string(prop_name);
       }
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 819 "has.y"
    {
        (yyval.string_val) = new string((yyvsp[(1) - (1)].sval));
     }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 824 "has.y"
    {
        prop_fo_map[(yyvsp[(2) - (4)].sval)] = (yyvsp[(4) - (4)].for_val);
      }
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 827 "has.y"
    { }
    break;



/* Line 1806 of yacc.c  */
#line 2572 "has.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 829 "has.y"


int main(int argc, char** argv) {
	// open a file handle to a particular file:
	FILE *myfile = fopen(argv[1], "r");
    bool debug = false;
    int naive = 0;
    int verifier_option = 0;
    without_set = false;

    if (argc >= 3) {
        verifier_option = atoi(argv[2]);
        if (verifier_option == 2)
            without_set = true;
        else
            without_set = false;
    }
    

    if (argc >= 4)
        naive = atoi(argv[3]);
    if (argc >= 5)
        debug = atoi(argv[4]);
	
    // make sure it's valid:
	if (!myfile) {
		cout << "Can't open input file!" << endl;
		return -1;
	}

	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;

	// parse through the input until there is no more:
	yyparse();

    struct timeval tv;
    struct timeval start_tv;
    gettimeofday(&start_tv, NULL);
    double elapsed = 0.0;
	
    if (verifier_option == 1) {
        LivenessProperty prop(art, 0, generate_safety(0, art), generate_safety(0, art));
        SpinVerifier sver(art, atms, prop, naive);
        // Formula* target = std::generate_safety(0, art);
        
        
        FILE* fout = fopen("output.pml", "w"); 
        fprintf(fout, "%s\n", sver.generate_promela().c_str());
        fclose(fout);
        system("spin -a output.pml && cc -o pan pan.c -O2 -DVECTORSZ=2048 -DMEMLIM=8192 -DCOLLAPSE && timeout 10m ./pan -a");

        gettimeofday(&tv, NULL);
        elapsed = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;

        printf("time = %lf\n", elapsed);
    } else {

    // art.dump();
    // int num_atms = atms.size();
    //for (int aid = 0; aid < num_atms; aid++)
    //    atms[aid].dump();
        if (atms.size() == 0) {
            int num_atms = 1;
            int num_atm_states = 5;
            int num_trans = 5;
            // generate_atms(art, num_atms, num_atm_states, num_trans, atms);
        }
        
        int seed = 0;
        int num_tasks = art.tasks.size();

        Verifier ver(art, atms, naive, debug);
        vector<tuple<State, State, vector<int> > > res;
        ver.preprocess();
        ver.reachable_root(res);
        
        gettimeofday(&tv, NULL);
        elapsed = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;

        int max_vstates = 0;
        int num_vstates = 0;
        int num_states = 0;
        int max_states = 0;
        int num_counters = 0;
        int max_counters = 0;
        int max_cyclomatic = 0;
            

        vector<int> vec_num_counter_states;
        vector<int> vec_num_states;
        ver.profile_get_num_counters(vec_num_counter_states);
        ver.profile_get_num_states(vec_num_states);

        for (int i = 0; i < num_tasks; i++) {
            num_vstates += ver.profile_vstate_set[i].size();
            max_vstates = max(max_vstates, (int) ver.profile_vstate_set[i].size());
            num_states += vec_num_states[i];
            max_states = max(max_states, vec_num_states[i]);
            num_counters += vec_num_counter_states[i];
            max_counters = max(max_counters, vec_num_counter_states[i]);
            max_cyclomatic = max(max_cyclomatic, ver.profile_cyclomatic[i]);
        }
        printf("seed\ttime\tcyclomatic\tnum_vstates\tmax_vstates\tnum_states\tmax_states\tnum_counters\tmax_counters\tmax_active_counters\ttime_superstate\ttime_substate\ttime_nextstate\ttime_omega\thit_rate_superstate\thit_rate_substate\thit_rate_reach_map\ttime_scc\n");
        printf("%d\t%lf\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
                seed,
                elapsed,
                max_cyclomatic,
                num_vstates,
                max_vstates,
                num_states,
                max_states,
                num_counters,
                max_counters,
                ver.profile_get_max_active_counters(),
                (float) ver.time_superstate / CLOCKS_PER_SEC,
                (float) ver.time_substate / CLOCKS_PER_SEC,
                (float) ver.time_nextstate / CLOCKS_PER_SEC,
                (float) ver.time_omega / CLOCKS_PER_SEC,
                (float) ver.num_superstate_hits / ver.num_superstate_tests,
                (float) ver.num_substate_hits / ver.num_substate_tests,
                (float) ver.num_reach_map_hits / ver.num_reach_map_tests,
                (float) ver.time_scc / CLOCKS_PER_SEC);
   
    }
}

void yyerror(const char *s) {
	cout << "Parse error on line " << line_num << "!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}

