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
        for(auto i = 0 ; i < 5; i++){
            auto data = alloc.allocate(1);
            alloc.construct(data, i);
        }
    }

    void destroy_all_mem(){
        memory_controller<int>::instance().destroyAllMemory();
    }


    bool memory_controller_sample_insert(){
        self_allocator<int> alloc;
        for(auto i = 0 ; i < 5; i++){
            auto data = alloc.allocate(1);
            alloc.construct(data, i);
        }

        int i = 0;
        for(const auto & iter : memory_controller<int>::instance().return_const_pool()){
            auto  iterCluster = std::find_if(iter.begin(), iter.end(),[&](auto const & cluster) {
                if (*cluster.first == i) {
                    std::cout << "i = " << i << " *cluster.first = " << *cluster.first << std::endl;
                    return true;
                } else {
                    return false;
                }
            });

            if(iterCluster == iter.end()){
                std::cout << "i = " << i << iterCluster->first << std::endl ;
                memory_controller<int>::instance().destroyAllMemory();
                return false;
            }
            i++;
        }
        return true;
    }

    void clear_mem_pool(){

    }
}


TEST(checkPatchVersion, VersionController){
    ASSERT_TRUE(units::checkPatchVersion());
};

TEST(FillByThreads, allocateMemory){
    ASSERT_TRUE(units::memory_controller_sample_insert());
}