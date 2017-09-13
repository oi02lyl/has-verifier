/*
 * LivenessProperty.cpp
 *
 *  Created on: Jan 12, 2017
 *      Author: oi02l
 */

#include "LivenessProperty.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

namespace std {

string LivenessProperty::generate_hltlfo() {
	// root task only
	return "G (p -> F q)\n\nPropositions:\np: " + form1->to_string() + "\nq: " + form2->to_string();
}

void LivenessProperty::generate_atms(vector<Automaton>& atms) {
    // root task only
    Automaton atm(0);
    vector<AState> states(2, AState());

    states[0].id = 0;
    states[0].name = "S0";
    states[0].accept = false;

    states[1].id = 1;
    states[1].name = "S1";
    states[1].accept = true;

    vector<vector<pair<vector<AProp*>, int> > > transition;
    vector<pair<vector<AProp*>, int> > tran1;
    vector<pair<vector<AProp*>, int> > tran2;
    
    // transition 1
    pair<vector<AProp*>, int> edge1(vector<AProp*>(), 0);
    tran1.push_back(edge1);
    
    // transition 2
    vector<AProp*> cond1;
    
    cond1.push_back(new APropFO(form1->copy()));
    Internal* negated = new Internal("!");
    negated->paras[0] = form2->copy();
    cond1.push_back(new APropFO(negated));
    pair<vector<AProp*>, int> edge2(cond1, 1);
    tran1.push_back(edge2);

    // transition 3
    vector<AProp*> cond2;
    cond2.push_back(new APropFO(negated->copy()));
    pair<vector<AProp*>, int> edge3(cond2, 1);
    tran2.push_back(edge3);

    transition.push_back(tran1);
    transition.push_back(tran2);

    atm.num_states = 2;
    atm.states = states;
    atm.transition = transition;
    atms.push_back(atm);
}

void parse_ltl3ba_condition(string& form, vector<Formula*>& prop_to_form, vector<vector<AProp*> >& aprops) {
    if (form == "t") {
        aprops.push_back(vector<AProp*>()) ;
        return;
    }
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

            Formula* form = prop_to_form[prop_id]->copy();
            if (negated) {
                Internal* internal = new Internal("!");
                internal->paras[0] = form;
                form = internal->copy();
            }
            APropFO* prop = new APropFO(form);
            prop->negated = false;
            disjunct.push_back(prop);
        }

        if (!disjunct.empty())
            aprops.push_back(disjunct);
    }
}

void parse_and_generate(vector<string>& lines, Artifact& art, Automaton& atm) {
    int num_prop = 0;
    int start_state = 0;
    vector<Formula*> prop_to_form;
    
    for (int line_num = 0; line_num < (int) lines.size(); line_num++) {
        string line = lines[line_num];
        stringstream ss(line);
        string hd;
        ss >> hd;

        if (hd == "States:") {
            ss >> atm.num_states;
            atm.states = vector<AState>(atm.num_states, AState());
            atm.transition = 
                vector<vector<pair<vector<AProp*>, int> > >(
                        atm.num_states, 
                        vector<pair<vector<AProp*>, int> >());
        } else if (hd == "Start:") {
            ss >> start_state;
            if (start_state != 0) {
                printf("bug: start_state != 0\n");
                exit(0);
            }
        } else if (hd == "AP:") {
            ss >> num_prop;
            //for (int prop_id = 0; 
            //        prop_id < num_prop; prop_id++)
            for (int prop_id = num_prop - 1; 
                    prop_id >= 0; prop_id--)
                prop_to_form.push_back(
                        generate_safety(0, art));
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
                    string cond = line.substr(start_id + 1, end_id - start_id - 1);
                    int next_state = atoi(line.substr(end_id + 2).c_str());
                    vector<vector<AProp*> > aprops;
                    parse_ltl3ba_condition(cond, prop_to_form, aprops);
                    // printf("next_state = %d\n", next_state);
                    // printf("size = %d\n", (int) aprops.size());
                    for (vector<AProp*> aprop : aprops)
                        atm.transition[state_id].push_back(
                        pair<vector<AProp*>, int>(aprop, next_state) );
                }
            }
        }
    }
}

void generate_random_atm(string ltl, Artifact& art, Automaton& atm) {
    int val = (int) getpid();
    char fn1[30];
    char fn2[30];
    char cmd[150];
    sprintf(fn1, "prop%d.txt", val);
    sprintf(fn2, "prop_out%d.txt", val);
    sprintf(cmd, "./ltl3ba-1.1.3/ltl3ba -H3 -F %s > %s", fn1, fn2);

    FILE* fout = fopen(fn1, "w");
    fprintf(fout, "%s", ltl.c_str());
    fclose(fout);
    system(cmd);
    ifstream fin(fn2);
    vector<string> lines;
    string line;
    while (getline(fin, line))
        lines.push_back(line);
    fin.close();
    
    parse_and_generate(lines, art, atm);
    // atm.dump();
    sprintf(cmd, "rm %s %s", fn1, fn2);
    system(cmd);
}


}
