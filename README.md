# allocator_self_v2
[![Build Status](https://travis-ci.org/DimKush/allocator_self_v2.svg?branch=main)](https://travis-ci.org/github/DimKush/allocator_self_v2)

> Build machine : sysname='Linux', nodename='pop-os', release='5.8.0-7630-generic', machine='x86_64'
> compilers: gcc6, g++6
> Build system: Cmake v3.14

Another implementation of default std::allocator to trick std::map, std::list etc. containers, that it's values will be stored in one memory cluster like in std::vector or std::array.
## The main purpose
The main purpose of this project is to trick C++ default memory allocation and try to save all elements near each other.

## Problem
We choose, that our container elements (like std::list, std::map, std::set) are situate sequentially. Many developers working with game development want it more functional and accessible, they want an easy and safe method to remove selected bytes of memory and have fast and easy access to the elements of their personal analogs of C++ containers. When you want to save an order of elements in one "bucket" in any container, you can't do it, because features of memory is allocating in containers according to specs in C++ work as it works.

## Idea of realisation
![Image alt](https://github.com/DimKush/readme_stuff/blob/main/self_allocator_v2/mem_pooler.png)
The main idea - is to build <abbr title="Singleton by Scott Meyers">Singleton</abbr> class memory_controller. This template class will provide all memory operations like: 
+ get the memory
+ drop the memory

## How to use
First of all, you need to add a project as submodule in your git project. After that you need to run a command in your home directory:

`git submodule update --init --recursive`

It's required, because my project use googletest for unit tests. After you updated submodules you should go to inside the submodule and after that, just run the python script:

`./build_project.py`

It'll build all and if you turn on <abbr title="set(DEMO_READY ON) in CmakeLists.txt">DEMO READY</abbr> and it'll pack like a Debian package.

***To see memory traffic just turn on a <abbr title="set(debug_log 1) in CmakeLists.txt">debug_log</abbr> ***.

After you built your library, all you need to do is to include headers and apply it For example, simple CmakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME test)
project(${PROJECT_NAME} VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# exec
add_executable(${PROJECT_NAME}.out src/main.cpp)

target_include_directories(${PROJECT_NAME}.out PUBLIC ${PROJECT_SOURCE_DIR}/allocator_self_v2/lib_alloc )
```
main.cpp:
```cpp
#include "lib/self_allocator/self_allocator.h"
#include <iostream>
#include <map>

int main(int argc, char **argv){
    int count_iterations = 0;

    if(argc > 1)
        count_iterations = std::stoi(argv[1]);
    else {
        std::cout << "argument isn't installed, set to default = 10.\n";
        count_iterations = 10;
    }

    std::cout << "create std::map<int,int>\n";
    std::map<int,int> defaultMap;
    for(auto i = 0 ; i < count_iterations; i++)
        defaultMap.insert({i, int{i} });

    std::cout << "std::map<int,int, std::less<>, self_allocator<std::pair<int, int>>\n";
    std::map<int,int, std::less<>, self_allocator<std::pair<int, int>>> customAllocMap;
    for(auto i = 0 ; i < count_iterations; i++)
        customAllocMap.insert({i, int{i} });

    std::cout << "std::map<int,int> output\n";
    for(const auto &i : defaultMap)
        std::cout << i.second << '\n';

    std::cout << "std::map<int,int, std::less<>, self_allocator<std::pair<int, int>> output\n";
    for(const auto &i : customAllocMap)
        std::cout << i.second << '\n';

    return 0;
}
```
## How to install/uninstall as deb (DEMO MOD)
If you want to check the solution as installed the Debian package runs cmake in  <abbr title="set(DEMO_READY ON) in CmakeLists.txt">DEMO READY</abbr> mode.
In build directory you will see .deb package. Install it by command:

`sudo dpkg -i allocator_self_v2-0.1.1-Linux.deb`

##### Direction of installing (DEMO MODE)
By default, direction of installing is: `/home` directory. You can also change it :
```cmake
    install(TARGETS ${LIB_NAME} ${PROJECT_NAME}
            RUNTIME DESTINATION "/home/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
            LIBRARY DESTINATION "/home/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
            DESTINATION "/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
            )

```

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