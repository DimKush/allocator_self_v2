//
// Created by dim on 3/4/21.
//

#include <thread>
#include <vector>
#include <array>
#include <exception>

#include "unit.h"
#include "self_allocator/self_allocator.h"


namespace units{
    bool checkPatchVersion(){
        return PATCH_VERSION > 0;
    }

    void memory_set(int & val){
        self_allocator<int> alloc;

        auto data = alloc.allocate(1);
        alloc.construct(data,val);
        int * ptr = data;
    }

    void destroy_all_mem(){
        memory_controller<int>::instance().destroyAllMemory();
    }

    void fill_multithread(){

        int i = 10;
        /*
        std::thread t1(memory_set, std::ref(i));
        std::thread t2(memory_set, std::ref(i));
        std::thread t3(memory_set, std::ref(i));
        std::thread t4(memory_set, std::ref(i));

        t1.join();
        t2.join();
        t3.join();
        t4.join();

        memory_controller<int>::instance().showMemPool();
         */
        memory_set(std::ref(i));
    }
    void clear_mem_pool(){

    }
}


TEST(checkPatchVersion, VersionController){
    ASSERT_TRUE(units::checkPatchVersion());
};

TEST(FillByThreads, ThreadsWork){
    ASSERT_ANY_THROW(units::fill_multithread());
}