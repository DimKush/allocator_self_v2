cmake_minimum_required(VERSION 3.14)

# cmake --log-level
set( CMAKE_MESSAGE_LOG_LEVEL "DEBUG" )

set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)


message(DEBUG " Build type : ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL Release)
    message(DEBUG " Build type : ${CMAKE_BUILD_TYPE}")

    if(DEFINED $ENV{TRAVIS_BUILD_NUMBER})
        set(PATCH_VERSION $ENV{TRAVIS_BUILD_NUMBER})
    else()
        set(PATCH_VERSION 1)
    endif()
else()
    set(PATCH_VERSION 6)

endif()

set(PROJECT_NAME allocator_self_v2)
set(LIB_NAME alloc_mem)

#option: prepare executable for test functionality in custom List and std::map OFF / ON)
set(DEMO_READY ON)

#option: SIZE OF CLUSTER IN MEMORY POOL
set(CLUSTER_SIZE 5)

#option: turn off (-1) or turn on (1) debug option
set(debug_log 1)

#project declaration
project(${PROJECT_NAME} VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

include(GenerateExportHeader)
include(${PROJECT_SOURCE_DIR}/gtest.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(version.vers ${PROJECT_SOURCE_DIR}/version.h)
configure_file(cluster.options ${PROJECT_SOURCE_DIR}/lib_alloc/lib/memory_controller/cluster_options.h)
configure_file(demo_status.options ${PROJECT_SOURCE_DIR}/lib_alloc/lib/demo_status.h)

#shared_library
add_library(${LIB_NAME} SHARED
        ${PROJECT_SOURCE_DIR}/lib_alloc/lib/memory_controller/memory_controller.h
        ${PROJECT_SOURCE_DIR}/lib_alloc/src/self_allocator/self_allocator.cpp
        ${PROJECT_SOURCE_DIR}/lib_alloc/lib/self_allocator/self_allocator.h
        )

generate_export_header(${LIB_NAME} )
target_include_directories(${LIB_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/lib_alloc/lib ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})

set_target_properties(${LIB_NAME}
        PROPERTIES
        CXX_STANDARD_REQUIRED ON
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;-Wno-attributes")


if("${DEMO_READY}" STREQUAL "OFF")
    message(DEBUG " Build without demo")

else()
    find_package(Threads REQUIRED)
    message(DEBUG " Build with demo")
    #source
    add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/demo/main.cpp
                                   ${PROJECT_SOURCE_DIR}/demo/hard/src/hard.cpp)

    target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/demo)

    #link library
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIB_NAME} ${CMAKE_THREAD_LIBS_INIT})

    set_target_properties(${PROJECT_NAME}
            PROPERTIES
            CXX_STANDARD_REQUIRED ON
            COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra")

endif()

#include gtest cmake file

find_package (Threads REQUIRED)

#unit test
add_executable( unit_test_mem.out
        ${PROJECT_SOURCE_DIR}/unit/unit.cpp
        ${PROJECT_SOURCE_DIR}/unit/unit.h
        ${PROJECT_SOURCE_DIR}/unit/main.cpp
        )

#targets unit tests
target_link_libraries( unit_test_mem.out
                        googletestLib
                        ${CMAKE_THREAD_LIBS_INIT}
                        ${LIB_NAME}
        )

target_include_directories(unit_test_mem.out PUBLIC
        ${PROJECT_SOURCE_DIR}/lib_alloc
        )

set_target_properties(unit_test_mem.out
        PROPERTIES
        CXX_STANDARD_REQUIRED ON)

enable_testing()

add_test(unit_test unit_test_mem.out)

if("${DEMO_READY}" STREQUAL "ON")
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "KK")
    set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

    set(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
    set(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
    set(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")

    set(CPACK_PACKAGE_CONTACT dimkysh@gmail.com)

    install(TARGETS ${LIB_NAME} ${PROJECT_NAME}
            RUNTIME DESTINATION "/home/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
            LIBRARY DESTINATION "/home/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
            DESTINATION "/${PROJECT_NAME}-${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}"
            )

include(CPack)

endif()



