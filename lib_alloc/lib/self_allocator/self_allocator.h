#pragma once

#include "memory_controller/memory_controller.h"
#include <alloc_mem_export.h>

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

    ALLOC_MEM_EXPORT self_allocator(){}
    ALLOC_MEM_EXPORT ~self_allocator(){}

    Type * ALLOC_MEM_EXPORT allocate(size_type n) {
        if(debug_log > 0)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        auto ptr = memory_controller<Type>::instance().giveMemory(n);
        if(ptr == nullptr){
            throw std::bad_alloc();
        }
        else{
            return reinterpret_cast<Type*>(ptr);
        }
    }

    void ALLOC_MEM_EXPORT deallocate(Type *p, size_type) {
        if(debug_log > 0) {
            std::cout << __PRETTY_FUNCTION__ << '\n';
            std::cout << p << '\n';
        }
        memory_controller<Type>::instance().destroyMemory(p);
    }
    template<typename U, typename ... Args>
    void ALLOC_MEM_EXPORT construct(U* p, Args&& ... args){
        if(debug_log > 0)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        new(p) U(std::forward<Args>(args)...);
    }
    void ALLOC_MEM_EXPORT destroy(Type *p){
        if(debug_log)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        p->~Type();
    }
};