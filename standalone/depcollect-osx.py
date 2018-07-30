#!/usr/bin/env python
import subprocess
import os
import shutil

def command(cmd):
    print cmd
    subprocess.check_call(cmd, shell = True)

output_dir = "Allofw/lib"

try: os.makedirs(output_dir)
except: pass

ld_library_path = [
    "/usr/local/lib",
    "../liballofw/install/lib",
    "../liballofw/dependencies/skia/skia/out/Release"
]

ignore_paths = [
    "/usr/lib",
    "/System"
]

def ResolveLibrary(lib):
    lib = lib.replace("@rpath/", "")
    for p in ld_library_path:
        candidate = os.path.join(p, lib)
        print candidate
        if os.path.exists(candidate):
            return candidate
    raise Exception("Library '%s' not found." % lib)

def GetLibraryDeps(lib_path):
    deps = subprocess.check_output("otool -L %s" % lib_path, shell = True)
    deps = [ x.strip() for x in deps.split("\n") ][1:]
    for dep in deps:
        if dep == "": continue
        if any([ dep.startswith(k) for k in ignore_paths ]): continue
        dep = dep.split(" ")[0]
        yield dep, ResolveLibrary(dep)

def ProcessDependencies(lib_path, r = set()):
    for original, dep in GetLibraryDeps(lib_path):
        if not dep in r:
            r.add(dep)
            r |= ProcessDependencies(dep, r)
    return r

def distill(lib_path):
    print "distill: %s" % lib_path
    # Distill
    for original, dep in GetLibraryDeps(lib_path):
        command("install_name_tool -change %s %s %s" % (original, "@loader_path/" + os.path.split(dep)[1], lib_path))

source_files = [
    "../allofw.node/binaries/allofw.v6.node",
    "../allofw.node/binaries/allofw.v7.node",
    "../allofw.node/binaries/allofw.v8.node",
    "../allofw.node/binaries/allofw.v9.node",
    "../allofw.node/binaries/allofw.v10.node"
]
s = set()

for filename in source_files:
    for lib in ProcessDependencies(filename) | set([filename]):
        name = os.path.basename(lib)
        print name
        target = os.path.join(output_dir, name)
        shutil.copyfile(lib, target)
        os.chmod(target, 0755)
        distill(target)

