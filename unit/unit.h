//
// Created by dim on 3/4/21.
//

#ifndef ALLOCATOR_SELF_V2_UNIT_H
#define ALLOCATOR_SELF_V2_UNIT_H

#include "lib_alloc/lib/memory_controller/memory_controller.h"
#include <gtest/gtest.h>
#include "version.h"
namespace units{
    bool checkPatchVersion();
    void memory_set(int & val);
    void fill_multithread();
};
#endif //ALLOCATOR_SELF_V2_UNIT_H
