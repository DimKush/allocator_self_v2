# allocator_self_v2
> Build system : sysname='Linux', nodename='pop-os', release='5.8.0-7630-generic', version='#32~1609193707~20.10~781bb80-Ubuntu SMP Tue Jan 5 21:29:56 UTC 2', machine='x86_64'

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

//TODO
## Unit tests
Unit tests are based on [googletest](http://localhost/ "link title") library.
List of unit tests :
+ Ordinary tests:
    + VersionController - check version of project from CmakeLists.txt;
    + allocateMemory - allocate memory and search values in memory_controller;
    + dropMemory - delete allocated memory;
    + fillStlCustomAllocator - fill stl containers (std::vector<int>, std::map):
        + with std::allocator;
        + with self_allocator;
If you turn on The <abbr title="set(DEMO_READY ON) in CmakeLists.txt">DEBUG MOD</abbr>:
+ Demo tests:
    + fillCustomList - fill custom List with std::alocator and self_allocator help.

//TODO

## How to use

## How to install/uninstall as deb
## Direction of installing
//TODO

## MAIN UNSOLVED PROBLEM OF THE PROJECT