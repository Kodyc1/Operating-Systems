#!/usr/bin/python

import subprocess
import os
import signal
import sys
from time import sleep

if len(sys.argv) > 1:
    nforks = int(sys.argv[1])
else:
    nforks = 2

subprocess.call("gcc my_fork.c", shell=True)

outputFilename = "output"
proc = subprocess.Popen("./a.out %d" % nforks, shell=True, stdout=subprocess.PIPE)
answer, stderrdata = proc.communicate()

#answer = f.readlines()
#if len(answer) < 1:
#    print "Your output is blank..."
#elif len(answer) > 1:
#    print "Your output should only have one line!"

# split string into chars, sort, reassemble to check
#answer[0].strip()
answer = "".join(sorted(list(answer)))
expectedAnswer = 'A'*nforks + 'B'*nforks + 'C'*nforks + 'D'*nforks
if answer == expectedAnswer:
    print "Your output should parse okay!"
else:
    print "There was a problem parsing your output. We expected AABBCCDD (not necessarilly in order) and got", answer
