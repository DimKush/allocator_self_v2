#ifndef _SELF_ALLOCATOR_H_
#define _SELF_ALLOCATOR_H_

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

    ALLOC_MEM_EXPORT Type * allocate(size_type n) {
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

    ALLOC_MEM_EXPORT void deallocate(Type *p, size_type) {
        memory_controller<Type>::instance().destroyMemory(p);
    }
    template<typename U, typename ... Args>
    ALLOC_MEM_EXPORT void construct(U* p, Args&& ... args){
        if(debug_log > 0)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        new(p) U(std::forward<Args>(args)...);
    }
    ALLOC_MEM_EXPORT void destroy(Type *p){
        if(debug_log > 0)
            std::cout << __PRETTY_FUNCTION__ << '\n';

        p->~Type();
    }
};

#endif