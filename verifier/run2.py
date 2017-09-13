import os
import random

random.seed(123)

num_runs = 30
seeds = range(num_runs)

for i in range(num_runs):
    seeds[i] = random.randint(0, 1000000000)

for i in range(num_runs):
    cmd = './has %d 0 5 5 10 20 10 5 > perf_%d.txt' % (seeds[i], i)
    print cmd
    os.system(cmd)


