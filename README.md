# allocator_self_v2
Another implementation of default std::allocator to trick std::map, std::list etc containers, that it's values will be storage in one memory cluster like std::vector or std::array.

## The main purpose
The main purpose of this project is to trick C++ default memory allocating and try to safe all elements near each other.

## Problem
Sometimes, we want, that  our containers elements (like std::list, std::map, std::set) situate sequentially.
I heard, that guys in game development want it so much, because they want to safe some bytes of memory and get fast
access to the elements of their self analogs of C++ containers.
When you want to safe an order of elements in one "bucket" in any container, you can't do it because features of memory
allocating with this types of containers in C++ work as it works.

## Idea of realisation
![Image alt](https://github.com/DimKush/readme_stuff/blob/main/self_allocator_v2/mem_pooler.png)

//TODO
## Unit tests
Unit tests are based on googletest library.
//TODO

## How to use

## How to install/uninstall as deb

## How to uninstall as deb

## Direction of installing
//TODO

## MAIN UNSOLVED PROBLEM OF PROJECT