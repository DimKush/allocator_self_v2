# allocator_self_v2
[![Build Status](https://travis-ci.org/DimKush/allocator_self_v2.svg?branch=main)](https://travis-ci.org/github/DimKush/allocator_self_v2)

> Build system : sysname='Linux', nodename='pop-os', release='5.8.0-7630-generic', machine='x86_64'

Another implementation of default std::allocator to trick std::map, std::list etc containers, that it's values will be storage in one memory cluster like std::vector or std::array.

## The main purpose
The main purpose of this project is to trick C++ default memory allocating and try to safe all elements near each other.

## Problem
Sometimes, we want, that our containers elements (like std::list, std::map, std::set) situate sequentially.
I heard, that guys in game development want it so much, because they want to safe some bytes of memory and get fast
access to the elements of their self analogs of C++ containers.
When you want to safe an order of elements in one "bucket" in any container, you can't do it, because features of memory
allocating with this types of containers in C++ work as it works.

## Idea of realisation
![Image alt](https://github.com/DimKush/readme_stuff/blob/main/self_allocator_v2/mem_pooler.png)
The main idea - is to build <abbr title="Singleton by Scott Meyers">Singleton</abbr> class memory_controller. This template class will provide all memory operations like: 
+ get the memory
+ drop the memory

## How to use
First of all, you need to add project as submodule in your git project.
After that you need to run command in your home directory:

`git submodule update --init --recursive`

It's nessesary, because my project use googletest for unit tests. 
After you updated submodules you should go to inside of submodule and after that, just run python script:

`./build_project.py`

It built all and if you turn on <abbr title="set(DEMO_READY ON) in CmakeLists.txt">DEMO READY</abbr> packeged it like debian package.

After you build your library, all you need to do is include

## How to install/uninstall as deb
//TODO
## Direction of installing
//TODO

## Unit tests
Unit tests are based on [googletest](http://localhost/ "link title") library.
List of unit tests :
+ Ordinary tests:
    + VersionController - check version of the project from CmakeLists.txt;
    + allocateMemory - allocate memory and search values in memory_controller;
    + dropMemory - delete the allocated memory;
    + fillStlCustomAllocator - fill the stl containers (std::vector<int>, std::map):
        + with std::allocator;
        + with self_allocator;
		
If you turn on The <abbr title="set(DEMO_READY ON) in CmakeLists.txt">DEBUG MOD</abbr>:
+ Demo tests:
    + fillCustomList - fill custom List
		+ with std::alocator 
		+ with self_allocator help.

# Main unresolved problem of the project.
Right know memory_controller and self_allocator can't work with std::thread and std::async.