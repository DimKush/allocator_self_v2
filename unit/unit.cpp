//
// Created by dim on 3/4/21.
//

#include <thread>
#include <vector>
#include <array>
#include <exception>

#include "unit.h"
#include "self_allocator/self_allocator.h"
#include "demo_status.h"

#if DEMO_READY == ON
#include "demo/List/include/List.h"
#endif

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
        for(auto i = 0 ; i < 10; i++){
            auto data = alloc.allocate(1);
            alloc.construct(data, i);
        }

        memory_controller<int>::instance().showMemPool();

        for(const auto & iter : memory_controller<int>::instance().return_const_pool()){
            for(const auto & iterVect : iter )
                std::cout << *iterVect.first << std::endl;
        }

        int i = 0;
        bool isSituated = false;

        while(i < 10) {
            for (const auto &iter : memory_controller<int>::instance().return_const_pool())
                for (auto const &iterVect : iter)
                    if (*iterVect.first == i)
                        isSituated = true;
                        break;

            i++;
        }

        memory_controller<int>::instance().destroyAllMemory();
        return isSituated;
    }

    bool clear_mem_pool(){
        self_allocator<int> alloc;
        for(auto i = 0 ; i < 10; i++){
            auto data = alloc.allocate(1);
            alloc.construct(data, i);
        }

        memory_controller<int>::instance().destroyAllMemory();

        for(auto blockIter = memory_controller<int>::instance().cbegin();
        blockIter != memory_controller<int>::instance().cend();blockIter++){
            for (auto iterCluster = blockIter->begin(); iterCluster != blockIter->end(); ++iterCluster) {
                if (iterCluster->first != nullptr) {
                    memory_controller<int>::instance().destroyAllMemory();
                    return false;
                }
            }
        }

        memory_controller<int>::instance().destroyAllMemory();
        return true;
    }
}

TEST(checkPatchVersion, VersionController){
    ASSERT_TRUE(units::checkPatchVersion());
};

TEST(FillSimple, allocateMemory){
    ASSERT_TRUE(units::memory_controller_sample_insert());
}

TEST(FillSimple, dropMemory){
    ASSERT_TRUE(units::clear_mem_pool());
}

TEST(FillSimple, fillStlCustomAllocator){
    std::vector<int> vectCompare;
    std::vector<int,self_allocator<int>> vectSelfAlloc;

    std::map<const int, std::string, std::less<>, self_allocator<std::pair<const int,std::string>>> selfMap;
    std::map<const int, std::string, std::less<>> deaultMap;


    for(auto i = 0; i < 10; i ++){
        vectCompare.push_back(i);
        vectSelfAlloc.push_back(i);

        //selfMap.insert({i,std::to_string(i)});
        deaultMap.insert({i,std::to_string(i)});
    }

    for(auto i = 0 ; i < vectCompare.size() ; i++){
        ASSERT_EQ(vectSelfAlloc.at(i), vectCompare.at(i));
    }

    for(auto i = 0; i < selfMap.size(); i++){
        ASSERT_EQ(selfMap.at(i), deaultMap.at(i));
    }

    auto size_vectCompare = sizeof(std::vector<int>) + sizeof(int) * vectCompare.size();
    auto size_vectSelfAlloc = sizeof(std::vector<int,self_allocator<int>>) + sizeof(int) * vectSelfAlloc.size();
    ASSERT_EQ(size_vectCompare, size_vectSelfAlloc);

    auto size_selfMap = sizeof(std::map<const int, std::string, std::less<>, self_allocator<std::pair<const int,std::string>>>)
                          + sizeof(int) * selfMap.size();

    auto size_deaultMap = sizeof(std::map<const int, std::string, std::less<>>) + sizeof(int) * deaultMap.size();

    ASSERT_TRUE(size_selfMap != size_deaultMap); // if this test will not pass, functionality doesn't have a point
}


// if demo status is ON - the additional unit tests

#if DEMO_READY == ON

TEST(FillSimple, fillCustomList){
    List<int> defAllocList;
    List<int, self_allocator<int>> customAllocList;

    for(auto i = 0; i < 5 ; i++){
        defAllocList.insert(i);
        customAllocList.insert(i);
    }

    auto iterDef = defAllocList.begin();
    auto iterCustom = customAllocList.begin();

    while(iterDef != defAllocList.end() && iterCustom != customAllocList.end()) {
        ASSERT_EQ(*iterDef->data, *iterCustom->data);
        ++iterDef ; ++iterCustom;
    }
}

#endif