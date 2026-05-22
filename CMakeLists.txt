cmake_minimum_required(VERSION 3.21)

project(rjsj_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RJSJ_ENABLE_COVERAGE "Build project targets with --coverage instrumentation" OFF)

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

if(RJSJ_ENABLE_COVERAGE)
    set(RJSJ_COVERAGE_COMPILER_SUPPORTED OFF)

    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        set(RJSJ_COVERAGE_COMPILER_SUPPORTED ON)
    endif()

    if(CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC")
        set(RJSJ_COVERAGE_COMPILER_SUPPORTED OFF)
    endif()

    if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(RJSJ_COVERAGE_COMPILER_SUPPORTED OFF)
    endif()

    if(NOT RJSJ_COVERAGE_COMPILER_SUPPORTED)
        message(FATAL_ERROR
            "RJSJ_ENABLE_COVERAGE uses --coverage and llvm-cov gcov, "
            "which require GCC or GNU-style Clang/AppleClang. "
            "MSVC and clang-cl are not supported for this coverage mode."
        )
    endif()

    string(REGEX MATCH "^[0-9]+" RJSJ_LLVM_TOOL_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
    find_program(LLVM_COV_EXECUTABLE
        NAMES
            llvm-cov-${RJSJ_LLVM_TOOL_VERSION}
            llvm-cov
        REQUIRED
    )
endif()

function(rjsj_enable_coverage target_name)
    if(NOT RJSJ_ENABLE_COVERAGE)
        return()
    endif()

    target_compile_options(${target_name} PRIVATE --coverage)

    get_target_property(target_type ${target_name} TYPE)
    if(NOT target_type STREQUAL "STATIC_LIBRARY")
        target_link_options(${target_name} PRIVATE --coverage)
    endif()
endfunction()

add_library(rjsj_exercises
    src/exercise_bb.cpp
    src/exercise_wb.cpp
    utils/utils.cpp
)

target_include_directories(rjsj_exercises
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(rjsj_exercises PUBLIC cxx_std_20)
rjsj_enable_coverage(rjsj_exercises)

include(CTest)

if(BUILD_TESTING)
    add_executable(rjsj_tests
        test/test.cpp
    )

    target_link_libraries(rjsj_tests
        PRIVATE
            rjsj_exercises
            Boost::unit_test_framework
    )

    rjsj_enable_coverage(rjsj_tests)

    add_test(NAME rjsj_tests COMMAND rjsj_tests)

    if(RJSJ_ENABLE_COVERAGE)
        set(RJSJ_COVERAGE_OBJECT_DIR
            ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles
        )
        set(RJSJ_COVERAGE_OUTPUT_DIR
            ${CMAKE_CURRENT_BINARY_DIR}/coverage
        )

        add_custom_target(coverage
            COMMAND
                ${CMAKE_COMMAND}
                -DBINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/CleanCoverageData.cmake
            COMMAND
                ${CMAKE_CTEST_COMMAND}
                $<$<BOOL:$<CONFIG>>:-C>
                $<$<BOOL:$<CONFIG>>:$<CONFIG>>
                --output-on-failure
            COMMAND
                ${CMAKE_COMMAND}
                "-DLLVM_COV_EXECUTABLE=${LLVM_COV_EXECUTABLE}"
                "-DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
                "-DOBJECT_DIR=${RJSJ_COVERAGE_OBJECT_DIR}"
                "-DOUTPUT_DIR=${RJSJ_COVERAGE_OUTPUT_DIR}"
                -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/RunLlvmGcov.cmake
            DEPENDS rjsj_tests
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            USES_TERMINAL
            COMMENT "Running tests and generating llvm-cov gcov coverage report"
        )

        add_custom_target(llvm-cov-report DEPENDS coverage)
    endif()
endif()
