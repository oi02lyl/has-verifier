import os
from os import listdir
from os.path import isfile, join

mypath = '../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
verifiers = [0, 2]
# naives = [0]
# files = ['Book-Writing-and-Publishing.txt']

for fname in files:
    for veri in verifiers:
#        for naive in naives:
        print fname
        cmd = './has_real ../bpmn/%s %d > result_km/%s.%d.txt' % (fname, veri, fname, veri)
        os.system(cmd)


