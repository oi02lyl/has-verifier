import os
import pickle

seeds = pickle.load(open('seed_synthetic.pkl', 'rb'))

for i in xrange(120):
    for p in range(12):
        cmd = './has_synthetic %d 1 0 %d %d > result_spin_synthetic/syn.%d.%d.txt' % (seeds[i][0], p, seeds[i][p], i, p)
        print cmd
        os.system(cmd)
