#ifndef ALLOCATOR_SELF_V2_HARD_H
#define ALLOCATOR_SELF_V2_HARD_H

#include <ostream>
struct hard{
private:
    int fa, fi;
    int count_fib(int fi_);
    int count_fact(int fa_);
public:
    hard();
    hard(const hard & hrd_);
    hard(const int & val_);

    ~hard();

    void count(const int & val_);

    hard& operator =(hard const & external);
    friend std::ostream & operator <<(std::ostream & ostr, hard const & hrd );
};

#endif //ALLOCATOR_SELF_V2_HARD_H
