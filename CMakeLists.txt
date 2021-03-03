cmake_minimum_required(VERSION 3.7)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)

set(PROJECT_NAME allocator_self_v2)
set(LIB_NAME alloc_mem)

#SIZE OF CLUSTER IN MEMORY POOL
set(CLUSTER_SIZE 5)

#turn off (-1) or turn on (1) debug option
set(debug_log 1)

#prepare executable for test functionality -1 (no) / 1(yes)

project(${PROJECT_NAME} VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

include(GenerateExportHeader)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(version.vers ${PROJECT_SOURCE_DIR}/version.h)
configure_file(cluster.options ${PROJECT_SOURCE_DIR}/lib_alloc/lib/cluster_options.h)

#shared_library
add_library(${LIB_NAME} SHARED
        ${PROJECT_SOURCE_DIR}/lib_alloc/lib/memory_controller/memory_controller.h
        ${PROJECT_SOURCE_DIR}/lib_alloc/src/self_allocator/self_allocator.cpp
        ${PROJECT_SOURCE_DIR}/lib_alloc/lib/self_allocator/self_allocator.h
        )
generate_export_header(${LIB_NAME} )

target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib_alloc/lib ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})



#find_library(SHARED_SELF_ALLOC NAMES alloc_mem PATHS ${PROJECT_SOURCE_DIR}/build)


#source
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME})


set_target_properties(${LIB_NAME}
        PROPERTIES
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Wno-attributes")

set_target_properties(${PROJECT_NAME}
        PROPERTIES
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")

#target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib_alloc/lib)
