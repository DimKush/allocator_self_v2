#pragma once

#include <vector>
#include <memory>

template<typename T, int CLUST_SZ = 5>
class memory_controller{
    using cluster = typename std::vector<std::pair<T*,std::size_t>>;
    typename std::vector<cluster> mem_pool;
    typename std::vector<cluster>::iterator poolIter;
    typename cluster::iterator clusterIter;

    void AppendCluster(std::size_t const & sz){
        mem_pool.emplace_back(cluster{std::make_pair(reinterpret_cast<T*>(std::malloc(sizeof(T) * sz)), sz)});
        poolIter = std::prev(mem_pool.end());
        clusterIter = std::prev(mem_pool.end())->begin();
    }
    void AppendNode(std::size_t const & sz){
        poolIter->emplace_back(std::make_pair(reinterpret_cast<T*>(std::malloc(sizeof(T) * sz)), sz));
        clusterIter = std::prev(poolIter->end());
    }

public:
    memory_controller(){
    }
    ~memory_controller(){
        for(auto iter : mem_pool)
            for(auto &iterCluster : iter)
                free(iterCluster.first);
    }
    static memory_controller& instance(){
        static memory_controller memCntrl;
        return memCntrl;
    }
    T* giveMemory(std::size_t const & sz){
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
    }
    void destroyMemory(T* ptr){
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
                if(debug_log)
                    std::cout << "mem :" << iterCluster->first << " " << iterCluster->second << '\n';
                std::free(iterCluster->first);
                iter.erase(iterCluster);
                break;
            }

        }
        if(debug_log)
            showMemPool();
    }
    void showMemPool(){
        int clustOrder = 0;
        for(auto iter = mem_pool.begin(); iter != mem_pool.end(); iter++){
            if(debug_log)
                std::cout << "clustOrder = " << clustOrder << '\n';

            for(auto iterVect = iter->begin() ; iterVect != iter->end(); iterVect++){
                std::cout << iterVect->first << '\n';
            }
            clustOrder++;
        }
    }
};