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

//template<typename T>
int count_factorial(int const & count){
    if(count == 1){
        return 1;
    }

    int current_val = 1, res = 1;
    while(current_val <= count){
        res *= current_val;
        current_val++;
    }

    return res;
}

unsigned long long count_fibonacci(int count){
    if(count == 0)
        return 0;
    if(count == 1)
        return 1;

    return count_fibonacci(count - 2) + count_fibonacci(count - 1);
}

struct hard{
private:
    int fa, fi;
public:
    hard(int fa_, int fi_) : fa(fa_), fi(fi_) {}
    friend std::ostream & operator << (std::ostream & os, hard & hard){
        os << hard.fa << " " << hard.fi << " ";
        return os;
    }
};

int main(){
    std::map<int

    return 0;
}
