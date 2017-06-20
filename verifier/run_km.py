import os
from os import listdir
from os.path import isfile, join

mypath = '../bpmn/'
files = [f for f in listdir(mypath) if isfile(join(mypath, f))]
# naives = [0]
naives = [0]
# files = ['Commercial-Financing.txt']

"""
for fname in files:
    for veri in verifiers:
        print fname
        cmd = './has_real ../bpmn/%s %d > result_km/%s.%d.txt' % (fname, veri, fname, veri)
        os.system(cmd)
"""

for fname in files:
    for naiv in naives:
        print fname
        cmd = './has_real ../bpmn/%s 0 %d > result_km/%s.%d.txt' % (fname, naiv, fname, naiv)
        os.system(cmd)


"""
for fname in files:
    for naiv in naives:
        print fname
        cmd = './has_real ../bpmn/%s 0 %d > result_km/%s.%d.txt' % (fname, naiv, fname, naiv)
        os.system(cmd)
"""
