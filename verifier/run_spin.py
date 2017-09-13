import os
import pickle
from os import listdir
from os.path import isfile, join

mypath = '../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
seeds = pickle.load(open('seed_real.pkl', 'rb'))

for fname in files:
    for p in range(12):
        for n in range(1,3):
            print fname
            cmd = './has_real ../bpmn/%s 1 %d %d %d> result_spin/%s.%d.%d.txt' % (fname, n, p, seeds[(fname, p)], fname, n, p)
            os.system(cmd)


