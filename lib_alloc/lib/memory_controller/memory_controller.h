#ifndef _MEMORY_CONTROLLER_H_
#define _MEMORY_CONTROLLER_H_

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include "cluster_options.h"
#include <alloc_mem_export.h>
#include <mutex>

std::mutex mute;

template<typename T>
class memory_controller{
    using cluster = typename std::vector<std::pair<T*,std::size_t>>;
    typename std::vector<cluster> mem_pool;
    typename std::vector<cluster>::iterator poolIter;
    typename cluster::iterator clusterIter;

    void ALLOC_MEM_EXPORT AppendCluster(std::size_t const & sz){
        mem_pool.emplace_back(cluster{std::make_pair(reinterpret_cast<T*>(std::malloc(sizeof(T) * sz)), sz)});
        poolIter = std::prev(mem_pool.end());
        clusterIter = std::prev(mem_pool.end())->begin();
    }
    void ALLOC_MEM_EXPORT AppendNode(std::size_t const & sz){
        poolIter->emplace_back(std::make_pair(reinterpret_cast<T*>(std::malloc(sizeof(T) * sz)), sz));
        clusterIter = std::prev(poolIter->end());
    }
    static const ALLOC_MEM_EXPORT int CLUST_SZ = CLUSTER_SIZE; // cluster size
public:
    ALLOC_MEM_EXPORT memory_controller(){}
    ALLOC_MEM_EXPORT ~memory_controller(){
        mute.lock();
        for(auto iter : mem_pool)
            for(auto &iterCluster : iter)
                free(iterCluster.first);
        mute.unlock();
    }
    static memory_controller& ALLOC_MEM_EXPORT instance(){
        static memory_controller memCntrl;
        return memCntrl;
    }
    T* ALLOC_MEM_EXPORT giveMemory(std::size_t & sz){
        mute.lock();
        if(mem_pool.empty()){
            AppendCluster(sz);
            return clusterIter->first;
        }
        else{
            int nodePos = clusterIter - poolIter->begin();
            if(nodePos == CLUST_SZ - 1){
                AppendCluster(sz);
                return clusterIter->first;
            }
            else{
                AppendNode(sz);
                return clusterIter->first;
            }
        }
        mute.unlock();
    }
    void ALLOC_MEM_EXPORT destroyMemory(T* ptr){
        mute.lock();
        for(auto &iter : mem_pool){
            auto iterCluster = std::find_if(iter.begin(), iter.end(),[=](auto cluster){
                if(cluster.first == ptr){
                    return true;
                }
                else{
                    return false;
                }
            });
            if(iterCluster != iter.end()){
                if(debug_log > 0)
                    std::cout << "mem :" << iterCluster->first << " size = " << iterCluster->second << '\n';
                std::free(iterCluster->first);
                iter.erase(iterCluster);
                break;
            }
        }
        if(debug_log > 0)
            showMemPool();
        mute.unlock();
    }
    void ALLOC_MEM_EXPORT showMemPool(){
        std::cout << "=============SHOW MEM POOL=============" << std::endl;
        int clustOrder = 0;
        for(auto iter = mem_pool.begin(); iter != mem_pool.end(); iter++){
            if(debug_log > 0)
                std::cout << "clustOrder = " << clustOrder << '\n';

            for(auto iterVect = iter->begin() ; iterVect != iter->end(); iterVect++){
                std::cout << iterVect->first << '\n';
            }
            clustOrder++;
        }
    }
};


#endif