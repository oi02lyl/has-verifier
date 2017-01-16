/*
 * Automaton.cpp
 *
 *  Created on: Oct 26, 2016
 *      Author: lyl
 */

#include "Automaton.h"
#include <cstdlib>

namespace std {


Automaton::~Automaton() {
	// TODO Auto-generated destructor stub
}

void Automaton::dump() {
    printf("Task id = %d\n", taskid);
    for (int i = 0; i < num_states; i++) {
        printf("State %d : %s\n", i, states[i].to_string().c_str());

        int num_trans = transition[i].size();
        for (int j = 0; j < num_trans; j++) {
            int target = transition[i][j].second;
            vector<AProp*>& prop_list = transition[i][j].first;
                        
            for (AProp* prop : prop_list)
                printf("%s, ", prop->to_string().c_str());
            printf(" -> State %d\n", target);
        }
    } 
}

bool AProp::is_fo() const {
	return false;
}

bool AProp::is_service() const {
	return false;
}

bool AProp::is_subform() const {
	return false;
}

bool AProp::is_child() const {
	return false;
}

string AProp::to_string() const {
    return "";
}

bool APropFO::is_fo() const {
	return true;
}

string APropFO::to_string() const {
    return fo->to_string();
}

bool APropService::is_service() const {
	return true;
}

string APropService::to_string() const {
    char res[30];
    if (negated)
        sprintf(res, "Ser %d", ser_id);
    else
        sprintf(res, "!Ser %d", ser_id);
    return res;
}

bool APropSubForm::is_subform() const {
	return true;
}

string APropSubForm::to_string() const {
    char res[30];
    if (negated)
        sprintf(res, "Atm %d", auto_id);
    else
        sprintf(res, "!Atm %d", auto_id);
    return res;
}

bool APropChild::is_child() const {
	return true;
}

string APropChild::to_string() const {
    char res[30];
    if (negated)
        sprintf(res, "Child %d", child_id);
    else
        sprintf(res, "!Child %d", child_id);
    return res;
}

string AState::to_string() const {
    char res[30];
    sprintf(res, "%s_%d", name.c_str(), (int) accept);
    return res;
}

void generate_atms(Artifact& art, int num_atms, int num_states, int num_trans, vector<Automaton>& results) {
	const int form_size = 2;
	const int tran_size = 1;
	// Formula* generate_formula(int size, Task& task, Artifact& schema);

	int num_tasks = art.tasks.size();

	vector<vector<int> > task_atms(num_tasks, vector<int>());
	int atm_cnt = 0;
	for (int task_id = 0; task_id < num_tasks; task_id++) {
		int to_gen = (task_id == 0)? 1 : num_atms;
		for (int i = 0; i < to_gen; i++)
			task_atms[task_id].push_back(atm_cnt++);
	}

	for (int task_id = 0; task_id < num_tasks; task_id++) {
		int to_gen = (task_id == 0)? 1 : num_atms;
		Task& task = art.tasks[task_id];

		for (int i = 0; i < to_gen; i++) {
			Automaton atm(task_id);

			// states
			atm.num_states = num_states;
			for (int sid = 0; sid < num_states; sid++) {
				AState state;
				state.accept = 1;
				state.id = sid;
				char state_name[30];
				sprintf(state_name, "S%d", sid);
				state.name = state_name;
				atm.states.push_back(state);
			}

			// transitions
			atm.transition = vector<vector<pair<vector<AProp*>, int> > >(num_states,
					vector<pair<vector<AProp*>, int> >());
			for (int sid = 0; sid < num_states; sid++) {

				for (int tid = 0; tid < num_trans; tid++) {
					pair<vector<AProp*>, int> trans;
					vector<AProp*>& prop_list = trans.first;
					trans.second = rand() % num_states;

					for (int tt = 0; tt < tran_size; tt++) {
						int base = 4;
						if (task.children.empty())
							base -= 2;
						// TODO with no services
//						if (task.services.empty())
//							base -= 1;

						int coin = rand() % base;
						switch (coin) {
						case 0: {
							// FO
							Formula* form = generate_formula(form_size, task, art);
							APropFO* prop = new APropFO(form);
							prop->negated = false;
							prop_list.push_back(prop);
							break;
						}
						case 1: {
							// Service
							int ser_id = rand() % (int) task.services.size();
							APropService* prop = new APropService(ser_id);
							prop->negated = rand() % 2;
							prop_list.push_back(prop);
							break;
						}
						case 2: {
							// Child task
							int child_id = rand() % (int) task.children.size();
							bool is_open = rand() % 2;
							APropChild* prop = new APropChild(child_id, is_open);
							prop->negated = rand() % 2;
							prop_list.push_back(prop);
							break;
						}
						case 3: {
							// Sub property
							int child_id = rand() % (int) task.children.size();
							int child_task_id = task.children[child_id];
							int aid = task_atms[child_task_id][rand() % num_atms];

							APropSubForm* prop = new APropSubForm(child_id, aid);
                            // prop->negated = rand() % 2;
							prop->negated = false;
							prop_list.push_back(prop);
							break;
						}
						}
					}
					atm.transition[sid].push_back(trans);
				}
			}
			results.push_back(atm);
		}
	}
}


}
