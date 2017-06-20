import os
from os import listdir
from os.path import isfile, join

mypath = '../../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]


# for naive in [0]:
for naive in [0, 1, 2, 3, 4]:
    for fname in files:
        res_name = '%s.%d.txt' % (fname, naive)

        f = open(res_name)
        f.readline()
        res = f.readline().strip()
        print fname + '\t' + res



