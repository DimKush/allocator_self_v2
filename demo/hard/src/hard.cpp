#include "demo/hard/include/hard.h"

hard::hard() {}

hard::hard(const hard & hrd_) : fa{hrd_.fa}, fi{hrd_.fi}{}

hard::hard(const int & val_){
    count(val_);
}

hard::~hard(){}

int hard::count_fact(int fa_) {
    if(fa_ == 0){
        return 0;
    }

    if(fa_ == 1){
        return 1;
    }

    int current_val = 1, res = 1;
    while(current_val <= fa_){
        res *= current_val;
        current_val++;
    }

    return res;
}

int hard::count_fib(int fi_) {
    if(fi_ == 0)
        return 0;
    if(fi_ == 1)
        return 1;

    return count_fib(fi_ - 2) + count_fib(fi_ - 1);
}

void hard::count(const int & val_){
    fa = count_fact(val_);
    fi = count_fib(val_);
}

hard& hard::operator=(hard const & external) {
    fi = external.fi;
    fa = external.fa;
    return *this;
}


std::ostream & operator <<(std::ostream & ostr, hard const & hrd ){
    ostr << "fa = " << hrd.fa << " fi = " << hrd.fi;

    return ostr;
}