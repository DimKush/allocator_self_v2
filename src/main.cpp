#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <vector>
#include <array>
#include <deque>
#include <stdio.h>
#include <map>

#define debug_log 1

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

template<typename T>
struct Node{
public:
    T * data = nullptr;
    Node * prev = nullptr;
    Node * next = nullptr;
    Node(){}
    Node(T && val){
        data = new T(std::forward<T>(val));
    }
    Node(Node<T> const & val) {
        data = new T(*val.data);
    }
    ~Node(){
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        delete data;
    }
    // using that no one can create instances of Node
    virtual void virtual_plug() {}
};

template< typename T, typename allocator = self_allocator<Node<T>> >
class List : public Node<T>{
    public:
        //using Node = Node<T>;
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
    private:
        allocator allocator_obj;
        Node<T> * top = nullptr;
        Node<T> * bottom = nullptr;
        
    
    public:
        List(std::initializer_list<T> init_list){
            for(auto iter : init_list){
                insert(std::forward<T>(iter));
            }
        }
        List(T && val){
            insert(std::forward<T>(val));
        }

        List(){}
        ~List(){
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            Node<T> * topTmp = top;
            
            while(topTmp != nullptr){
                Node<T> * nextTop = topTmp->next;
                allocator_obj.destroy(topTmp);
                allocator_obj.deallocate(topTmp, 1);
                topTmp = nextTop;
            }
        }
        
        void insert(T && val){
            if(top != nullptr){
                Node<T> * current = top;
                while(current->next != nullptr && current->next->data != nullptr){
                    current = current->next;
                }
                
                auto data = allocator_obj.allocate(1);
                allocator_obj.construct(data, std::forward<T>(val));
                
                current->next = data;
                current->next->next = bottom;
                current->next->prev = current;
                bottom->prev = current->next;
            }
            else{
                top = new Node<T>();
                bottom = new Node<T>();
                insert(std::forward<T>(val));
            }
        }
        void print_adressess(){
            Node<T> * current = top;
            while(current != nullptr){
                std::cout << current << '\n';
                current = current->next;
            }
            std::cout << '\n';
        }
    class iterator{
        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = Node<T>;
            using pointer = Node<T>*;
            using reference = Node<T>&;
            
            iterator(pointer ptr) : it_ptr(ptr){}
            reference operator *() const { return *it_ptr; }
            pointer operator->() const{ return it_ptr; }
            iterator& operator++() { std::cout << __PRETTY_FUNCTION__ << std::endl; it_ptr= it_ptr->next ; return *this; }
            iterator operator++(int) { iterator tmp = *this; it_ptr = it_ptr->next; return tmp; }
            friend bool operator == (iterator const & first, iterator const & second){ return first.it_ptr == second.it_ptr; }
            friend bool operator != (iterator const & first, iterator const & second){std::cout << __PRETTY_FUNCTION__ << std::endl; return first.it_ptr != second.it_ptr; }
        private:
            pointer it_ptr;
    };
    class const_iterator{
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = const Node<T>;
        using pointer = const Node<T>*;
        using reference = const Node<T>&;
        
        const_iterator(pointer ptr) : it_ptr(ptr){}
        reference operator *() const { return *it_ptr; }
        pointer operator->(){ return it_ptr; }
        const_iterator& operator++(){ it_ptr= it_ptr->next ; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; it_ptr = it_ptr->next; return tmp; }
        friend bool operator == (const_iterator const & first, const_iterator const & second){ return first.it_ptr == second.it_ptr; }
        friend bool operator != (const_iterator const & first, const_iterator const & second){ return first.it_ptr != second.it_ptr; }
    private:
        pointer it_ptr;
    };
    
    iterator begin(){ std::cout << __PRETTY_FUNCTION__ << std::endl; return iterator(top->next); }
    iterator end(){ std::cout << __PRETTY_FUNCTION__ << std::endl; return iterator(bottom); }
};

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
