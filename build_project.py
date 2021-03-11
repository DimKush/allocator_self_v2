#!/usr/bin/env python3

import os
import shutil

from sys import argv
from pathlib import Path

def get_info_about_machine():
    info = os.uname()
    print("Machine :\n{0}".format(info))


def prepare_build_directory():
    #default path to build directory
    build_dir = "build"

    if os.path.isdir("build"):
        shutil.rmtree(build_dir)
        os.mkdir(build_dir)
        os.chdir(build_dir)
    else :
        os.mkdir(build_dir)
        os.chdir(build_dir)


def build():

    os.system("cmake ..")
    os.system("cmake -DCMAKE_BUILD_TYPE=Release .")
    os.system("cmake --build .")
    os.system("cmake --build . --target test")
    os.system("cmake --build . --target package")


def main():
    get_info_about_machine()
    prepare_build_directory()
    build()

if (__name__ =="__main__"):
    main()
