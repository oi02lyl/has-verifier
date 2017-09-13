import os
import pickle
from os import listdir
from os.path import isfile, join

def check_empty(fn):
    return 'nan' in open(fn).read()


def run(idx, sd1, n, p, sd2):
    output = 'result_km_noset_synthetic/syn.%d.%d.%d.txt' % (idx, n, p)
    cmd = './has_synthetic %d 2 %d %d %d > %s' % (sd1, n, p, sd2, output)
    print cmd
    os.system(cmd)
    return check_empty(output)

cnt = 120
seed_file = 'seed_synthetic.pkl'
f = open('seed_synthetic.pkl', 'rb')
seeds = pickle.load(f)
f.close()

for idx in xrange(cnt):
    sd1 = seeds[idx][0]
    for p in range(12):
        sd2 = seeds[idx][p]
        for n in range(1):
            run(idx, sd1, n, p, sd2)

    """
    print cmd
    os.system(cmd)

    if not check_empty(fn):
        sd2 = sd
        for p in range(12):
            while (True):
                fn = 'result_km_synthetic/syn.%d.0.%d.txt' % (i, p)
                cmd = './has_synthetic %d 0 0 %d %d > %s' % (sd, p, sd2, fn)
                print cmd
                os.system(cmd)
                if not check_empty(fn):
                    for n in range(5):
                        fn = 'result_km_synthetic/syn.%d.%d.%d.txt' % (i,n, p)
                        cmd = './has_synthetic %d 0 %d %d %d > %s' % (sd,n, p, sd2, fn)
                        print cmd
                        os.system(cmd)
                    break
                sd2 = random.randint(0, 1000000000)

    """
