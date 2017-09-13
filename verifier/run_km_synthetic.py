import os
import random
import pickle
from os import listdir
from os.path import isfile, join

def check_empty(fn):
    return 'nan' in open(fn).read()


def run(idx, sd1, n, p, sd2):
    output = 'result_km_synthetic/syn.%d.%d.%d.txt' % (idx, n, p)
    cmd = './has_synthetic %d 0 %d %d %d > %s' % (sd1, n, p, sd2, output)
    print cmd
    os.system(cmd)
    return check_empty(output)

random.seed(123)
cnt = 120
seed_file = 'seed_synthetic.pkl'
if os.path.isfile(seed_file):
    f = open(seed_file, 'rb')
    seeds = pickle.load(f)
    f.close()
else:
    seeds = None
    new_seeds = [[0] * 12 for _ in xrange(cnt)]



if seeds == None:
    for idx in xrange(cnt):
        sd1 = random.randint(0, 1000000000)
        while run(idx, sd1, 0, 0, sd1):
            sd1 = random.randint(0, 1000000000)

        for p in range(12):
            sd2 = sd1
            if p > 0:
                # while run(idx, sd1, 0, p, sd2):
                #    sd2 = random.randint(0, 1000000000)
                run(idx, sd1, 0, p, sd2)

            new_seeds[idx][p] = sd2
            for n in range(1, 5):
                run(idx, sd1, n, p, sd2)
    f = open(seed_file, 'wb')
    pickle.dump(new_seeds, f)
    f.close()
else:
    for idx in xrange(cnt):
        sd1 = seeds[idx][0]
        for p in range(12):
            sd2 = seeds[idx][p]
            for n in range(5):
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
