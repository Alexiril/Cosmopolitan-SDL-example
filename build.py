# VARIABLES

build_folder = "build_dir"

resources = [
    "libs/*",
    "license/*",
    "resources/*"
]

output_filename = "sdltest.exe"

include_folders = [
    "include",
    "/usr/include/SDL2",
    "/usr/include/x86_64-linux-gnu/SDL2"
]

sources = [
    "sources/application.cpp",
    "sources/cosmo_sdl2.cpp"
]

# IMPLEMENTATION

from subprocess import Popen
from os.path import isdir, isfile, exists, split
from os import mkdir, listdir
from shutil import rmtree, which
from random import randint

def pack_resource(path):
    global build_folder
    resource_path, resource = split(path)
    if resource == "*":
        return [pack_resource(f"{resource_path}/{x}") for x in listdir(resource_path)]
    index = randint(0, 999999)
    zip_name = f"{'0' * (6-len(str(index)))}{index}{resource}.zip.o"
    if exists(f"{build_folder}/.aarch64"):
        if not isdir(f"{build_folder}/.aarch64"):
            raise Exception(f"Path {build_folder}/.aarch64 exists already, but is not a folder.")
    else:
        mkdir(f"{build_folder}/.aarch64")
    zipobj = which("zipobj")
    params = zipobj + f" -o {build_folder}/{zip_name} {resource_path}/{resource}"
    print(f"Call {params}")
    Popen(["sh", "-c", params]).wait()
    aarch64_resource = f"{resource_path}/{resource}" # x86_64 and aarch64 resources are the same by default
    if isdir(f"{resource_path}/.aarch64") and isfile(f"{resource_path}/.aarch64/{resource}"):
        aarch64_resource = f"{resource_path}/.aarch64/{resource}"
    params = zipobj + f" -a aarch64 -o {build_folder}/.aarch64/{zip_name} {aarch64_resource}"
    print(f"Call {params}")
    Popen(["sh", "-c", params]).wait()
    return [zip_name]

def flatten_list(l):
    result = []
    for x in l:
        if type(x) == list: result.extend(flatten_list(x))
        else: result.append(x)
    return result

def start_compiler(packed_resources):
    global build_folder
    global output_filename
    global include_folders
    global sources
    includes = [f"-I {x}" for x in include_folders]
    resources = [f"-s {build_folder}/{x}" for x in packed_resources]
    cosmocpp = f'{which("cosmoc++")} -std=c++20 {" ".join(includes)} {" ".join(resources)} -o {build_folder}/{output_filename} {" ".join(sources)}'
    print(f"Call {cosmocpp}")
    print("-" * 80)
    Popen(["sh", "-c", cosmocpp]).wait()

if exists(build_folder):
    rmtree(build_folder)
mkdir(build_folder)
start_compiler(flatten_list([pack_resource(r) for r in resources]))
