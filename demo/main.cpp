#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <vector>
#include <array>
#include <deque>
#include <stdio.h>
#include <map>

#include "self_allocator/self_allocator.h"
#include "List/include/List.h"
#include "demo/hard/include/hard.h"

int main(int argc, char **argv){
    int count_iterations = 0;

    if(argc > 1)
        count_iterations = std::stoi(argv[1]);
    else {
        std::cout << "argument isn't installed, set to default = 10.\n";
        count_iterations = 10;
    }

    std::cout << "create std::map<int,hard>\n";
    std::map<int,hard> defaultMap;
    for(auto i = 0 ; i < count_iterations; i++)
        defaultMap.insert({i, hard{i} });

    std::cout << "std::map<int,hard, std::less<>, self_allocator<std::pair<int, hard>>\n";
    std::map<int,hard, std::less<>, self_allocator<std::pair<int, hard>>> customAllocMap;
    for(auto i = 0 ; i < count_iterations; i++)
        customAllocMap.insert({i, hard{i} });

    std::cout << "std::map<int,hard> output\n";
    for(const auto &i : defaultMap)
        std::cout << i.second << '\n';

    std::cout << "std::map<int,hard, std::less<>, self_allocator<std::pair<int, hard>> output\n";
    for(const auto &i : customAllocMap)
        std::cout << i.second << '\n';

    std::cout << "create custom container List<hard>\n";
    List<hard> defAllocList;
    for(auto i = 0 ; i < count_iterations; i++)
        defAllocList.insert(hard{i});

    std::cout << "List<hard> output\n";
    for(const auto &i : defAllocList)
        std::cout << *i.data << '\n';

    std::cout << "create custom container List<hard, self_allocator<hard>>\n";
    List<hard,self_allocator<hard>> custAllocList;
    for(auto i = 0 ; i < count_iterations; i++)
        custAllocList.insert(hard{i});

    std::cout << "List<hard, self_allocator<hard>> output\n";
    for(const auto &i : defAllocList)
        std::cout << *i.data << '\n';

    return 0;
}
