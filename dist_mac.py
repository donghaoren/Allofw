#!/usr/bin/env python
import subprocess
import os
os.chdir("dist")

def command(cmd):
    print cmd
    subprocess.check_call(cmd, shell = True)

command("rm -rf MacOS")
command("mkdir MacOS")

command("cp -r ../node-package/native/* MacOS/")
command("cp -r ../liballofw/samples MacOS/")

def getLibraryDeps(lib_path):
    deps = subprocess.check_output("otool -L %s" % lib_path, shell = True)
    deps = [ x.strip() for x in deps.split("\n") ][1:]
    for dep in deps:
        if dep.startswith("/usr/local/lib/") or dep.startswith("/usr/local/Cellar/"):
            dep = dep.split(" ")[0]
            yield dep

def processDependencies(lib_path, r = set()):
    for dep in getLibraryDeps(lib_path):
        if not dep in r:
            r.add(dep)
            r |= processDependencies(dep, r)
    return r

r = processDependencies("MacOS/lib/liballofw.dylib")

def distill(lib_path):
    print "distill: %s" % lib_path
    # Distill
    for dep in getLibraryDeps(lib_path):
        command("install_name_tool -change %s %s %s" % (dep, "./" + os.path.split(dep)[1], lib_path))

for dep in r:
    target = os.path.split(dep)[1]
    print target
    command("cp -L %s MacOS/lib/%s" % (dep, target))
    command("chmod 0644 MacOS/lib/%s" % (target))
    distill("MacOS/lib/%s" % target)

distill("MacOS/lib/liballofw.dylib")
