import os
import random
import pickle
from os import listdir
from os.path import isfile, join

def check_empty(fn):
    return 'nan' in open(fn).read()


def run_real(fn, n, p, sd):
    output = 'result_km/%s.%d.%d.txt' % (fn, n, p)
    cmd = './has_real ../bpmn/%s 0 %d %d %d > %s' % (fn, n, p, sd, output)
    print cmd
    os.system(cmd)
    return check_empty(output)

mypath = '../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
random.seed(123)
seeds = {}
seed_file = 'seed_real.pkl'
if os.path.isfile(seed_file):
    f = open(seed_file, 'rb')
    seeds = pickle.load(f)
    f.close()

for fn in files:
    for p in range(12):
        if (fn, p) in seeds:
            sd = seeds[(fn, p)]
        else:
            sd = random.randint(0, 1000000000)

        while run_real(fn, 0, p, sd):
            sd = random.randint(0, 1000000000)
        seeds[(fn, p)] = sd

        for n in range(1, 5):
            run_real(fn, n, p, sd)

f = open(seed_file, 'wb')
pickle.dump(seeds, f)
f.close()


"""
        naive = 0
        print fname, naive, p
        fn = 'result_km/%s.%d.%d.txt' % (fname, 0, p)
        seed = random.randint(0, 1000000000)
        while (True):
            cmd = './has_real ../bpmn/%s 0 %d %d %d > %s' % (fname, naive, p, seed, fn)
            os.system(cmd)
            if not check_empty(fn):
                for naive in range(1, 5):
                    print fname, naive, p
                    fn = 'result_km/%s.%d.%d.txt' % (fname, naive, p)
                    cmd = './has_real ../bpmn/%s 0 %d %d %d > %s' % (fname, naive, p, seed, fn)
                    os.system(cmd)

                break
            seed = random.randint(0, 1000000000)
"""

"""
for fname in files:
    for naiv in naives:
        print fname
        cmd = './has_real ../bpmn/%s 0 %d > result_km/%s.%d.txt' % (fname, naiv, fname, naiv)
        os.system(cmd)
"""

"""
for fname in files:
    for naiv in naives:
        print fname
        cmd = './has_real ../bpmn/%s 0 %d > result_km/%s.%d.txt' % (fname, naiv, fname, naiv)
        os.system(cmd)
"""
