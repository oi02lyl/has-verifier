import os
from os import listdir
from os.path import isfile, join

mypath = '../../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
naives = [0, 1, 2]

for naive in naives:
    for fname in files:
        res_name = '%s.%d.txt' % (fname, naive)

        f = open(res_name)
        found = False
        out_of_mem = False
        sat = False
        num_states = '-1'
        avg_as_size = '1'
        avg_as_size2 = '1'


        for line in open(res_name).readlines():
            if "errors: 0" in line:
                sat = True
            if "errors: 1" in line:
                sat = False

            if "pan: reached -DMEMLIM bound" in line:
                out_of_mem = True

            if "states, stored" in line:
                num_states = line.strip().split(' ')[0]
            if "avg_as_size" in line:
                avg_as_size = line.strip().split(' ')[2]
            if "avg_as_naive" in line:
                avg_as_size2 = line.strip().split(' ')[2]

            if line[:4] == 'time':
                ll = line.strip().split()[2]
                ll = ll.replace(',', '\t')
                if out_of_mem:
                    print fname + '\t' + ll + '\t' + str(sat) + '\t' + \
                          num_states + '\t' + avg_as_size + '\t' + avg_as_size2 + '\tOUT_OF_MEM'
                else:
                    print fname + '\t' + ll + '\t' + str(sat) + '\t' + \
                          num_states + '\t' + avg_as_size + '\t' + avg_as_size2
                found = True
                break
        if not found:
            print fname

