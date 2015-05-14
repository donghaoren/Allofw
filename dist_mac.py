#!/usr/bin/env python
import subprocess
import os
os.chdir("dist")

# Configuration
libraries = [
    "liballofw.dylib",
    "liballofw-graphics.dylib",
    "liballofw-omniapp.dylib"
]

def command(cmd):
    print cmd
    subprocess.check_call(cmd, shell = True)

def should_distribute(lib):
    prefix_list = [ "/usr/local/" ]
    for item in prefix_list:
        if lib.startswith(item):
            return True
    return False

def should_distill(lib):
    if should_distribute(lib):
        return True
    for my_library in libraries:
        if lib.find(my_library) >= 0:
            return True
    return False

command("rm -rf MacOS")
command("mkdir MacOS")

command("cp -r ../node-package/native/* MacOS/")
command("cp -r ../liballofw/samples MacOS/")

def get_dependencies(lib_path):
    deps = subprocess.check_output("otool -L %s" % lib_path, shell = True)
    deps = [ x.strip() for x in deps.split("\n") ][1:]
    return [ l.split(" ")[0] for l in deps ]

def get_dependencies_distribute(lib_path):
    return filter(should_distribute, get_dependencies(lib_path))

def process_dependencies(lib_path, r = set()):
    for dep in get_dependencies_distribute(lib_path):
        if not dep in r:
            r.add(dep)
            r |= process_dependencies(dep, r)
    return r

r = set()
for lib in libraries:
    process_dependencies("MacOS/lib/%s" % lib, r)

print r

def distill(lib_path):
    print "distill: %s" % lib_path
    # Distill
    lib_name = os.path.split(lib_path)[1]
    command("install_name_tool -id %s %s" % (lib_name, lib_path))
    for dep in filter(should_distill, get_dependencies(lib_path)):
        command("install_name_tool -change %s %s %s" % (dep, "@loader_path/" + os.path.split(dep)[1], lib_path))

for dep in r:
    target = os.path.split(dep)[1]
    print target
    command("cp -L %s MacOS/lib/%s" % (dep, target))
    command("chmod 0644 MacOS/lib/%s" % (target))
    distill("MacOS/lib/%s" % target)

for lib in libraries:
    distill("MacOS/lib/%s" % lib)

deps = set()
# Find all dependencies:
for root, dirs, files in os.walk("MacOS/lib"):
    for file in files:
        p = os.path.join(root, file)
        deps |= set(get_dependencies(p))
print "ALL DEPENDENCIES"
print "\n".join(sorted(list(deps)))
print "END ALL DEPENDENCIES"
