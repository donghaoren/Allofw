#!/usr/bin/env python

import platform

cmds = []

if platform.system() == "Darwin":
# Mac Users:
    libraries = [
        "yaml-cpp",
        "armadillo",
        "homebrew/versions/glfw3",
        "glew",
        "freeimage",
        "zeromq",
        "boost",
        "ffmpeg"
    ]
    cmds = [ "brew install %s" % lib for lib in libraries ]

print "I'm going to execute the following commands:"
print "\n".join([ "    " + cmd for cmd in cmds ])
yes = raw_input("Input 'YES' to confirm: ")
if yes != "YES":
    print "Canceled."
    exit(0)

import subprocess

for cmd in cmds:
    subprocess.check_call(cmd, shell = True)
