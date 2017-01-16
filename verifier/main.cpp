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


int main(int argc, char** args) {
    // int seed = 1469215425;
//    int seed = time(NULL);
//    srand(seed);
//    printf("seed = %d\n", seed);

	bool naive = false;
	// int seed = 1479854402;
	int seed = time(NULL);
    // printf("%d\n", seed);
    
	int num_rels = 5;
	int num_tasks = 5;
	int num_const = 10;
	int num_vars = 20;
	int num_services = 10;
	int form_size = 5;
    int num_atms = 1;
    int num_atm_states = 5;
    int num_trans = 5;

	if (argc > 2) {
		seed = atoi(args[1]);
		naive = atoi(args[2]);
	}

	if (argc > 8) {
		num_rels = atoi(args[3]);
		num_tasks = atoi(args[4]);
		num_const = atoi(args[5]);
		num_vars = atoi(args[6]);
		num_services = atoi(args[7]);
		form_size = atoi(args[8]);
	}

	srand(seed);
    // printf("Seed = %d\t", seed);

    DBSchema db;
	Artifact art;
	vector<Automaton> atms;

	std::generate_dbschema(num_rels, 4, db);
	// std::generate_artifact(5, 10, 20, 5, 5, db, art);
	std::generate_artifact(num_tasks, num_const, num_vars, num_services, form_size, db, art);
	// art.dump();
	std::generate_atms(art, num_atms, num_atm_states, num_trans, atms);
	// for (int atm_id = 0; atm_id < (int) atms.size(); atm_id++)
	//	atms[atm_id].dump();

	LivenessProperty prop(art, 0, generate_safety(0, art), generate_safety(0, art));

	SpinVerifier sver(art, atms, prop);
    FILE* fout = fopen("output.pml", "w"); 
	fprintf(fout, "%s\n", sver.generate_promela().c_str());
	// fprintf(fout, "%s\n", sver.generate_promela(0, target).c_str());
    fclose(fout);
    struct timeval tv;
    struct timeval start_tv;

    gettimeofday(&start_tv, NULL);
    system("spin -a output.pml && cc -o pan pan.c -O2 -DVECTORSZ=2048 && ./pan");

    double elapsed = 0.0;
    gettimeofday(&tv, NULL);
    elapsed = (tv.tv_sec - start_tv.tv_sec) + (tv.tv_usec - start_tv.tv_usec) / 1000000.0;

    printf("time = %lf\n", elapsed);

	return 0;



//	clock_t start = clock();
//
//	Verifier ver(art, atms, false, true);
//	vector<tuple<State, State, vector<int> > > res;
//	ver.preprocess();
//	ver.reachable_root(res);
//	int num_vstates = 0;
//	int max_vstates = 0;
//	int num_states = 0;
//	int max_states = 0;
//	int num_counters = 0;
//	int max_counters = 0;
//	int max_cyclomatic = 0;
//
//	vector<int> vec_num_counter_states;
//	vector<int> vec_num_states;
//	ver.profile_get_num_counters(vec_num_counter_states);
//	ver.profile_get_num_states(vec_num_states);
//
//	for (int i = 0; i < num_tasks; i++) {
//		num_vstates += ver.profile_vstate_set[i].size();
//		max_vstates = max(max_vstates, (int) ver.profile_vstate_set[i].size());
//		num_states += vec_num_states[i];
//		max_states = max(max_states, vec_num_states[i]);
//		num_counters += vec_num_counter_states[i];
//		max_counters = max(max_counters, vec_num_counter_states[i]);
//		max_cyclomatic = max(max_cyclomatic, ver.profile_cyclomatic[i]);
//	}
//
//	printf("seed\ttime\tnum_vstates\tmax_vstates\tnum_states\tmax_states\tnum_counters\tmax_counters\tmax_active_counters\ttime_superstate\ttime_substate\ttime_nextstate\ttime_omega\thit_rate_superstate\thit_rate_substate\thit_rate_reach_map\ttime_scc\n");
//	printf("%d\t%f\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n",
//			seed,
//			((float) (clock() - start)) / CLOCKS_PER_SEC,
//			max_cyclomatic,
//			num_vstates,
//			max_vstates,
//			num_states,
//			max_states,
//			num_counters,
//			max_counters,
//			ver.profile_get_max_active_counters(),
//			(float) ver.time_superstate / CLOCKS_PER_SEC,
//			(float) ver.time_substate / CLOCKS_PER_SEC,
//			(float) ver.time_nextstate / CLOCKS_PER_SEC,
//			(float) ver.time_omega / CLOCKS_PER_SEC,
//			(float) ver.num_superstate_hits / ver.num_superstate_tests,
//			(float) ver.num_substate_hits / ver.num_substate_tests,
//			(float) ver.num_reach_map_hits / ver.num_reach_map_tests,
//			(float) ver.time_scc / CLOCKS_PER_SEC);
//
//	// printf("#State = %d\tTime = %f\n", num_states, );
//
//    /*
//	if (!naive) {
//		Verifier ver1(art, false);
//		vector<pair<State, State> > res1;
//		ver1.preprocess();
//		ver1.reachable_root(res1);
//		printf("Seed = %d\t#State = %d\tTime = %f\n", seed, (int) ver1.reach_map[0].size(), ((float) (clock() - start)) / CLOCKS_PER_SEC);
//
//		Verifier ver2(art, true);
//		vector<pair<State, State> > res2;
//		ver2.preprocess();
//		ver2.reachable_root(res2);
//		printf("Seed = %d\t#State = %d\tTime = %f\n", seed, (int) ver2.reach_map[0].size(), ((float) (clock() - start)) / CLOCKS_PER_SEC);
//
//		printf("%d\n", res1 == res2);
//	}
//    */
//
//	return 0;
}
