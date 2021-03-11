set(GOOGLETEST_ROOT ${PROJECT_SOURCE_DIR}/googletest/googletest)

message(DEBUG "${GOOGLETEST_ROOT}")

include_directories(SYSTEM
        ${GOOGLETEST_ROOT}
        ${GOOGLETEST_ROOT}/include
        )

set(GOOGLETEST_SOURCES
        ${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${GOOGLETEST_ROOT}/src/gtest_main.cc
        )


foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletestLib ${GOOGLETEST_SOURCES})