import os
from os import listdir
from os.path import isfile, join

mypath = '../../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
naives = [0]

for naive in naives:
    for fname in files:
        res_name = '%s.%d.txt' % (fname, naive)

        f = open(res_name)
        found = False
        out_of_mem = False
        sat = False

        for line in open(res_name).readlines():
            if "errors: 0" in line:
                sat = True
            if "errors: 1" in line:
                sat = False

            if "pan: reached -DMEMLIM bound" in line:
                out_of_mem = True

            if line[:4] == 'time':
                ll = line.strip().split()[2]
                if out_of_mem:
                    print fname + '\t' + ll + '\t' + str(sat) + '\tOUT_OF_MEM'
                else:
                    print fname + '\t' + ll + '\t' + str(sat)
                found = True
                break
        if not found:
            print fname

