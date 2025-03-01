//
// Created by dim on 3/3/21.
//

#ifndef ALLOCATOR_SELF_V2_LIST_DEMO_H
#define ALLOCATOR_SELF_V2_LIST_DEMO_H

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
        if(debug_log > 0)
            std::cout << __PRETTY_FUNCTION__ << std::endl;
        delete data;
    }
    // using that no one can create instances of Node
    virtual void virtual_plug() {}
};

// small metatric to change the type of allocator from alloc<T> to alloc<Node<T>>
template<typename T>
struct change_allocator_t{
    using type = T;
};

template<typename T>
struct change_allocator_t<std::allocator<T>>{
    using type = std::allocator<Node<T>>;
};

template<typename T>
struct change_allocator_t<self_allocator<T>>{
using type = self_allocator<Node<T>>;
};

template< typename T,
          typename allocator = std::allocator<T>,
          typename allocator_t = typename change_allocator_t<allocator>::type
        >

class List : public Node<T>{
public:
    //using Node = Node<T>;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
private:
    allocator_t allocator_obj;
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
        if(debug_log > 0)
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
    void insert(T & val){
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
        iterator& operator++() { it_ptr= it_ptr->next ; return *this; }
        iterator operator++(int) { iterator tmp = *this; it_ptr = it_ptr->next; return tmp; }
        friend bool operator == (iterator const & first, iterator const & second){ return first.it_ptr == second.it_ptr; }
        friend bool operator != (iterator const & first, iterator const & second){ return first.it_ptr != second.it_ptr; }
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

    iterator begin(){ return iterator(top->next); }
    iterator end(){ return iterator(bottom); }
};


#endif //ALLOCATOR_SELF_V2_LIST_DEMO_H