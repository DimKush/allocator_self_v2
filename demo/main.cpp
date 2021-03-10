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
    std::map<int, int, std::less<>, self_allocator<std::pair<const int,hard>>> mp_alloc_self;
    mp_alloc_self.insert({1,1});
    mp_alloc_self.insert({2,1});
    mp_alloc_self.insert({3,1});
    mp_alloc_self.insert({4,1});

    /*
    List<hard> lst{std::move(List<hard>(std::move(hard(count_factorial(5),count_fibonacci(5)))))};                         // TODO container List must to get default allocator
    std::map<int, hard, std::less<const int>, self_allocator<std::pair<const int,hard>>> mp_alloc_self;
    std::map<int,hard> mp_auto_alloc;
    for(auto i = 0 ; i < 10 ; i++){
        lst.insert(std::move(hard(count_factorial(i),count_fibonacci(i))));
        mp_alloc_self.insert({i, std::move(hard(count_factorial(i),count_fibonacci(i)))});
        mp_auto_alloc.insert({i, std::move(hard(count_factorial(i),count_fibonacci(i)))});
    }

    for(auto &i : mp_alloc_self)
        std::cout << i.first << " " << i.second  << " " << &i << '\n';
    
    for(auto &i : mp_auto_alloc)
        std::cout << i.first << " " << i.second  << " " << &i << '\n';    

    for(auto element : lst)
        std::cout << *element.data << '\n';
    */
    /*
   List<int> lst{10,22,33};
   List<int> lst_self{50,60,70};
//   lst.print_adressess();
    std::for_each(lst.begin(),lst.end(),[](auto element){
        std::cout << element.data;
    });
    std::for_each(lst_self.begin(),lst_self.end(),[](auto element){
        std::cout << element.data;
    });
//    for(const auto iter : lst)
//       std::cout << *(iter.data) << std::endl;
    */
   return 0;
}
