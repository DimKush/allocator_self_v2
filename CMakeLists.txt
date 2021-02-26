cmake_minimum_required(VERSION 3.7)

set(MAJOR_VERSION 0)
set(MINOR_VERSION 0)
set(PATCH_VERSION 1)

set(PROJECT_NAME allocator_self_v2)

project(${PROJECT_NAME} VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})
set(CMAKE_CXX_STANDARD 17)

configure_file(version.vers ${PROJECT_SOURCE_DIR}/version.h)


#source
add_executable(${PROJECT_NAME}.out ${PROJECT_SOURCE_DIR}/src/main.cpp)
