#include <cstdio>
#include <ctime>
#include <algorithm>
#include <map>
#include <cstdlib>
#include <sys/time.h>

#include "DBSchema.h"
#include "Artifact.h"
#include "Verifier.h"
#include "SpinVerifier.h"
#include "LivenessProperty.h"
using namespace std;

const string ltls[] = {"false", "[] p", "(!p U q)", "(!p U q) && [] (p -> X (!p U q))", 
    "[] (p -> (q || X q || X X q))", "[] (p || [] (! p))", "[] (p -> <> q )", "<> p", "([] <> p) -> ([] <> q)",
    "[] <> p", "[] (p || ([] q))", "(<> [] p) -> ([] <> q)"};

int main(int argc, char** argv) {
    int seed = 123;
    int naive = 0;
    int verifier_option = 0;
    bool without_set = false;
    bool debug = false;
    string ltl = "true";
    int ltl_seed = 123;

    if (argc >= 2)
        seed = atoi(argv[1]);

    if (argc >= 3) {
        verifier_option = atoi(argv[2]);
        if (verifier_option == 2 || verifier_option == 1)
            without_set = true;
        else
            without_set = false;
    }

    if (argc >= 4)
        naive = atoi(argv[3]);
    if (argc >= 5) {
        ltl = ltls[atoi(argv[4])];
        ltl = "!( " + ltl + " )";
    }

    if (argc >= 6)
        ltl_seed = atoi(argv[5]);
    
	
	int num_rels = 5;
	int num_tasks = 5;
	int num_const = 10;
	int num_vars = 15;
	int num_services = 15;
	int form_size = 5;


	srand(seed);

    DBSchema db;
	Artifact art;
	vector<Automaton> atms;

	std::generate_dbschema(num_rels, 4, db);
	std::generate_artifact(num_tasks, num_const, num_vars, num_services, form_size, db, without_set, art);

    srand(ltl_seed);
    Automaton atm(0);
    generate_random_atm(ltl, art, atm);
    atms.push_back(atm);
    // atms[0].dump();

    struct timeval tv;
    struct timeval start_tv;
    gettimeofday(&start_tv, NULL);
    double elapsed = 0.0;
	
    if (verifier_option == 1) {
        srand(ltl_seed);
        //LivenessProperty prop(art, 0, generate_safety(0, art), generate_safety(0, art));
        // printf("%s\n%s\n", prop.form1->to_string().c_str(), prop.form2->to_string().c_str());
        vector<Formula*> ltl_forms;
        ltl_forms.push_back(generate_safety(0, art));
        ltl_forms.push_back(generate_safety(0, art));
        LtlfoProperty prop(ltl, ltl_forms);
        SpinVerifier sver(art, atms, prop, naive);
        // Formula* target = std::generate_safety(0, art);
        
        double t1, t2, t3;
        
        FILE* fout = fopen("output.pml", "w"); 
        fprintf(fout, "%s\n", sver.generate_promela().c_str());
        fclose(fout);
        system("spin -a -DNXT output.pml");
        gettimeofday(&tv, NULL);
        t1 = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;
        start_tv = tv;

        system("cc -o pan pan.c -O2 -DVECTORSZ=2048 -DMEMLIM=8192 -DCOLLAPSE");
        gettimeofday(&tv, NULL);
        t2 = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;
        start_tv = tv;

        system("timeout 10m ./pan -a");
        gettimeofday(&tv, NULL);
        t3 = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;
        printf("time = %lf,%lf,%lf\n", t1, t2, t3);
    } else {
        
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
