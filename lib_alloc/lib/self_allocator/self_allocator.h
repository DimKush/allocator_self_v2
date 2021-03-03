#pragma once

#include "lib/memory_controller.h"

#define debug_log 0

template<typename Type>
class self_allocator{
public:
    using value_type = Type;
    using pointer = Type*;
    using const_pointer = const Type*;
    using reference = Type&;
    using const_reference = const Type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    self_allocator(){}
    Type * allocate(std::size_t n) const{

        if(debug_log)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        //auto ptr = std::malloc(sizeof(T) * n);
        auto ptr = memory_controller<Type>::instance().giveMemory(n);
        if(ptr == nullptr){
            throw std::bad_alloc();
        }
        else{
            return reinterpret_cast<Type*>(ptr);
        }
    }

    void deallocate(Type *p, std::size_t n = 1) const {
        //std::cout << p << '\n';
        if(debug_log)
            std::cout << __PRETTY_FUNCTION__ << '\n';
        //std::free(p);
        memory_controller<Type>::instance().destroyMemory(p);
    }

    template<typename U, typename ... Args>
    void construct(U* p, Args&& ... args) const {
        //std::cout << __PRETTY_FUNCTION__ << '\n';
        new(p) U(std::forward<Args>(args)...);
    }

    void destroy(Type *p) const {
        if(debug_log)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        p->~Type();
    }
};