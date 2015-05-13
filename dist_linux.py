#!/usr/bin/env python
import subprocess
import os
os.chdir("dist")

def command(cmd):
    print cmd
    subprocess.check_call(cmd, shell = True)

command("rm -rf Linux")
command("mkdir Linux")

command("cp -r ../node-package/native/* Linux/")
command("cp -r ../liballofw/samples Linux/")

def getLibraryDeps(lib_path):
    deps = subprocess.check_output("ldd %s" % lib_path, shell = True)
    deps = [ x.strip() for x in deps.split("\n") ][1:]
    for dep in deps:
        if dep.find(" => ") < 0: continue
        dep = dep.split(" => ")[1].split(" ")[0]
        if dep.startswith("/opt") or dep.find("GLEW") >= 0 or dep.find("libstdc++") >= 0 or dep.find("libblas") >= 0 or dep.find("liblapack") >= 0 or dep.find("libcblas") >= 0 or dep.find("libf77blas") >= 0 or dep.find("libatlas") >= 0:
            dep = dep.split(" ")[0]
            yield dep

def processDependencies(lib_path, r = set()):
    for dep in getLibraryDeps(lib_path):
        if not dep in r:
            r.add(dep)
            r |= processDependencies(dep, r)
    return r

r = processDependencies("Linux/lib/liballofw.so")

for dep in r:
    target = os.path.split(dep)[1]
    print target
    command("cp -L %s Linux/lib/%s" % (dep, target))
    command("chmod 0644 Linux/lib/%s" % (target))
    #distill("MacOS/lib/%s" % target)

#distill("MacOS/lib/liballofw.dylib")
