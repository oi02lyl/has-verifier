import os
import pickle
from os import listdir
from os.path import isfile, join

def check_empty(fn):
    return 'nan' in open(fn).read()


def run_real(fn, n, p, sd):
    output = 'result_km_noset/%s.%d.%d.txt' % (fn, n, p)
    cmd = './has_real ../bpmn/%s 2 %d %d %d > %s' % (fn, n, p, sd, output)
    print cmd
    os.system(cmd)
    return check_empty(output)

mypath = '../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
f = open('seed_real.pkl', 'rb')
seeds = pickle.load(f)
f.close()

for fn in files:
    for p in range(12):
        sd = seeds[(fn, p)]
        for n in range(5):
            run_real(fn, n, p, sd)

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
