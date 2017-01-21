%{
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

%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":

%start has 

%union {
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
}

// define the constant-string tokens:
%token COMMA LP RP SLP SRP RELATIONS_HD TASKS_HD TASK_ED HIERARCHY_HD INPUT_HD RETURN_HD SERVICES_HD VARIABLES_HD LTLFO_HD UNDERSCORE PROP_HD 
%token CHILDREN_HD GLOBAL_PRECOND_HD PRECOND_HD POSTCOND_HD OPENCOND_HD CLOSECOND_HD SET_HD
%token <sval> BOp EQOp NOT TF IR PLUSMINUS TIMESDIVIDE UnaryLTLOp BinaryLTLOp 
%token COLON AS 

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT
%token <fval> FLOAT
%token <sval> STR STR_CONST

// type declaration
%type <str_pair_val> decl var_map edge
%type <vec_str_pair_val> var_decls var_map_list edge_list
%type <vec_str_val> var_list
%type <vec_expr_val> expr_list
%type <service_val> service
%type <vec_service_val> service_list
%type <task_val> task
%type <vec_task_val> task_list
%type <set_update_val> set_update
%type <vec_set_update_val> set_update_list
%type <string_val> ltlfo

%type <for_val> form
%type <for_val> term
%type <expr_val> expr
%left PLUSMINUS 
%left BOp
%left TIMESDIVIDE
%left NEG

%%
// the first rule defined is the highest-level rule, which in our
has: specification LTLFO_HD ltlfo fo_list 
    { 
        // to handle properties
        properties.push_back(pair<string, string>(*$3, art.tasks[0].name));
        
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
    | specification;

specification:
	RELATIONS_HD relations HIERARCHY_HD edge_list task_list 
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
        art.tasks = *$5;

        // task hierarchy
        map<string, int> task_name_mp;
        int num_tasks = art.tasks.size();
        for (int i = 0; i < num_tasks; i++)
            task_name_mp[art.tasks[i].name] = i;

        // printf("%d\n", (int) $4->size());        
        vector<int> parent_task(num_tasks, -1);
        int num_edge = $4->size();
        for (int i = 0; i < num_edge; i++) {
            pair<string, string> edge = $4->at(i);
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
	;
relations:
    relation relations
    | relation
    ;
relation:
	STR LP var_decls RP { 
        rel_list.push_back(*$3);
        type_map[$1] = art.db.num_rels;

        DBSchema& d = art.db;
        d.num_rels++;
        Relation rel;
        rel.name = $1;
        rel.arity = $3->size();
        int n = rel.arity;
        rel.attr_names = vector<string>(n, "");
        rel.types = vector<int>(n, -1);
        for (int i = 0; i < n; i++)
            rel.attr_names[i] = (*$3)[i].second;
        d.relations.push_back(rel);
    };
task_list:
    task_list task { $$ = $1; $$->push_back(*$2); }
    | task { $$ = new vector<Task>(1, *$1); }
    ;
task: 
    STR COLON VARIABLES_HD var_decls INPUT_HD var_map_list RETURN_HD var_map_list OPENCOND_HD form CLOSECOND_HD form SERVICES_HD service_list TASK_ED {
        $$ = new Task();
        $$->name = $1;
        task_id_map[$$->name] = total_tasks++;

        // variables
        $$->num_var = $4->size();
        for (int i = 0; i < $$->num_var; i++) {
            $$->vars.push_back(lookup_var((*$4)[i].second));
            string type_name = (*$4)[i].first;
            if (type_map.count(type_name) == 0) {
                printf("Type %s not defined!\n", type_name.c_str());
                exit(0);
            }
            $$->var_types.push_back(type_map[type_name]);
        }
        
        // input and return
        $$->input_vars = lookup_var(*$6);
        $$->return_vars = lookup_var(*$8);

        // opening and closing
        $$->open_cond = $10;
        $$->close_cond = $12;

        // services
        $$->services = *$14;
    };

var_decls:
    var_decls COMMA decl { $$ = $1; $$->push_back(*$3); }
    | decl { $$ = new vector<pair<string, string> >(1, *$1); };

decl:
    STR STR { $$ = new pair<string, string>($1, $2); };

service_list:
    service_list service { $$ = $1; $$->push_back(*$2); }
    | service { $$ = new vector<Service>(1, *($1)); }
    | { $$ = new vector<Service>(); };

service:
    STR COLON PRECOND_HD form POSTCOND_HD form SET_HD set_update_list {
        $$ = new Service();
        $$->name = $1;
        $$->pre_cond = $4;
        $$->post_cond = $6;
        $$->set_update = *$8;
    };

var_list:
    var_list COMMA STR { $$ = $1; $$->push_back($3); }
    | STR { $$ = new vector<string>(1, $1); }
    | { $$ = new vector<string>(); };

var_map_list:
    var_map_list COMMA var_map { $$ = $1; $$->push_back(*$3); }
    | var_map { $$ = new vector<pair<string, string> >(1, *$1); }
    | { $$ = new vector<pair<string, string> >(); };

var_map:
    STR AS STR  { $$ = new pair<string, string>($1, $3); }
    | STR { $$ = new pair<string, string>($1, $1); }
    ;

edge_list:
    edge_list COMMA edge { $$ = $1; $$->push_back(*$3); }
    | edge { $$ = new vector<pair<string, string> >(1, *$1); }
    | { $$ = new vector<pair<string, string> >(); };

edge:
    STR BOp STR  { $$ = new pair<string, string>($1, $3); }
    ;

form:
    LP form RP { $$ = $2; }
    | form BOp form { 
                    Internal* intern = new Internal();
                    intern->paras.push_back($1); 
                    intern->paras.push_back($3); 
                    intern->op = $2; 
                    $$ = intern; }
    | NOT form { Internal* intern = new Internal(); intern->paras.push_back($2);
               intern->op = "!"; $$ = intern; }
    | term { $$ = $1; }
    ; 
term:
    expr EQOp expr { 
                    if ($1 == NULL || $3 == NULL) {
                        $$ = new ConstTerm(1); 
                    } else {
                        CmpTerm* term = new CmpTerm();                      
                        term->p1 = *$1;
                        term->p2 = *$3; 
                        term->equal = (strcmp($2, "==") == 0); 
                        $$ = term;
                    }
                   }
    | TF { 
        $$ = new ConstTerm(strcmp($1, "true") == 0 ||
                           strcmp($1, "True") == 0 ||
                           strcmp($1, "TRUE") == 0);
    }
    | STR LP expr_list RP { 
        RelationTerm* term = new RelationTerm(); 
        term->rel_id = type_map[$1];
        term->paras = *$3;
        $$ = term;
    }
    ;
expr_list:
    expr_list COMMA expr { $$ = $1; $$->push_back(*$3); }
    | expr { $$ = new vector<Parameter>(1, *$1); }
    | { $$ = new vector<Parameter>(); }
    ;
expr:
    INT { 
        $$ = new Parameter(); 
        $$->is_const = true; 
        $$->type = -1;

        if (num_const_map.count($1) == 0) {
            int N = num_const_map.size();
            num_const_map[$1] = N;
        }
        $$->id = num_const_map[$1]; }
    | FLOAT { 
        $$ = new Parameter(); 
        $$->is_const = true; 
        $$->type = -1;

        if (num_const_map.count($1) == 0) {
            int N = num_const_map.size();
            num_const_map[$1] = N;
        }
        $$->id = num_const_map[$1]; }
    | STR_CONST { 
        $$ = new Parameter();
        $$->is_const = true;
        $$->type = -2;
        if (str_const_map.count($1) == 0) {
            int N = str_const_map.size();
            str_const_map[$1] = N;
        }
        $$->id = str_const_map[$1]; }
    | STR { 
        $$ = new Parameter();
        if (strcmp($1, "_") == 0) {
            $$->is_wildcard = true;
            $$->id = -1;
        } else {
            $$->is_wildcard = false;
            if (strcmp($1, "NULL") == 0 || strcmp($1, "null") == 0) {
                $$->is_const = true;
                $$->is_null = true;
                $$->type = -3;
            } else {
                $$->is_const = false; 
                $$->is_null = false;
                $$->id = lookup_var($1); 
            }
        } }
    | expr PLUSMINUS expr {
        // arithmetic is ignored
        $$ = NULL; }
    | expr TIMESDIVIDE expr {
        $$ = NULL; }
    | LP expr RP {
        $$ = $2; };


set_update_list:
    set_update_list COMMA set_update { $$ = $1; $$->push_back(*$3); }
    | set_update { $$ = new vector<SetUpdate>(1, *$1); }
    | { $$ = new vector<SetUpdate>(); };

set_update:
    STR LP var_list RP { 
       $$ = new SetUpdate();
       $$->vars = lookup_var(*$3);
       if (strcmp($1, "Prop") == 0)
           $$->type = Prop;
       else
           $$->type = Write; }
    | STR IR LP var_list RP  
    {   $$ = new SetUpdate();
        $$->vars = lookup_var(*$4);
        if (strcmp($2, "++") == 0)
            $$->type = InsertAfter;
        $$->setid = lookup_var($1);
    }
    | IR STR LP var_list RP  
    {   $$ = new SetUpdate();
        $$->vars = lookup_var(*$4);
        if (strcmp($1, "++") == 0)
            $$->type = InsertBefore;
        else
            $$->type = Retrieve;
        $$->setid = lookup_var($2);
    }
ltlfo:
     UnaryLTLOp ltlfo 
       { 
         $$ = new string();   
         $$->assign(string($1) + " " + *$2);
       }
     | ltlfo BinaryLTLOp ltlfo 
       { 
         $$ = new string();
         $$->assign(*$1 + " " + string($2) + " " + *$3);
       }
     | ltlfo BOp ltlfo 
       { 
         $$ = new string();
         $$->assign(*$1 + " " + string($2) + " " + *$3);
       }
     | LP ltlfo RP 
       { 
         $$ = new string();
         $$->assign("(" + *$2 + ")");
       }
     | NOT ltlfo 
       { 
         $$ = new string();   
         $$->assign(string($1) + " " + *$2);
       }
     | STR COLON SLP ltlfo SRP
       {
         int prop_id = properties.size();
         pair<string, string> new_prop(*$4, $1);
         properties.push_back(new_prop);
         char prop_name[30];
         sprintf(prop_name, "prop_%d", prop_id);
         $$ = new string(prop_name);
       }
     | STR {
        $$ = new string($1);
     }
fo_list:
    fo_list STR COLON form
      {
        prop_fo_map[$2] = $4;
      }
    | PROP_HD { }
;
%%

int main(int argc, char** argv) {
	// open a file handle to a particular file:
    int seed = 123;
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
        srand(seed);
        LivenessProperty prop(art, 0, generate_safety(0, art), generate_safety(0, art));
        // printf("%s\n%s\n", prop.form1->to_string().c_str(), prop.form2->to_string().c_str());
        SpinVerifier sver(art, atms, prop, naive);
        // Formula* target = std::generate_safety(0, art);
        
        
        FILE* fout = fopen("output.pml", "w"); 
        fprintf(fout, "%s\n", sver.generate_promela().c_str());
        fclose(fout);
        system("spin -a output.pml && cc -o pan pan.c -O2 -DVECTORSZ=2048 -DMEMLIM=8192 -DCOLLAPSE && timeout 30m ./pan -a");

        gettimeofday(&tv, NULL);
        elapsed = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;

        printf("time = %lf\n", elapsed);
    } else {

    // art.dump();
    // int num_atms = atms.size();
    //for (int aid = 0; aid < num_atms; aid++)
    //    atms[aid].dump();
        if (atms.size() == 0) {
            srand(seed);
            // int num_atms = 1;
            // int num_atm_states = 5;
            // int num_trans = 5;
            // generate_atms(art, num_atms, num_atm_states, num_trans, atms);
            LivenessProperty prop(art, 0, generate_safety(0, art), generate_safety(0, art));
            prop.generate_atms(atms);
            // printf("%s\n%s\n", prop.form1->to_string().c_str(), prop.form2->to_string().c_str());
        }
        
        int num_tasks = art.tasks.size();

        Verifier ver(art, atms, naive, debug);
        ver.preprocess();
        string res = ver.satisfy() ? "FALSE" : "TRUE";
        
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
        printf("seed\ttime\toutput\tcyclomatic\tnum_vstates\tmax_vstates\tnum_states\tmax_states\tnum_counters\tmax_counters\tmax_active_counters\ttime_superstate\ttime_substate\ttime_nextstate\ttime_omega\thit_rate_superstate\thit_rate_substate\thit_rate_reach_map\ttime_scc\n");
        printf("%d\t%lf\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
                seed,
                elapsed,
                res.c_str(),
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
