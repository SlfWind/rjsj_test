if(NOT DEFINED LLVM_COV_EXECUTABLE)
    message(FATAL_ERROR "LLVM_COV_EXECUTABLE is required")
endif()

if(NOT DEFINED SOURCE_DIR)
    message(FATAL_ERROR "SOURCE_DIR is required")
endif()

if(DEFINED OBJECT_DIRS)
    set(object_dirs ${OBJECT_DIRS})
elseif(DEFINED OBJECT_DIR)
    set(object_dirs ${OBJECT_DIR})
else()
    message(FATAL_ERROR "OBJECT_DIRS is required")
endif()

if(NOT DEFINED OUTPUT_DIR)
    message(FATAL_ERROR "OUTPUT_DIR is required")
endif()

set(coverage_notes)

foreach(object_dir IN LISTS object_dirs)
    file(GLOB_RECURSE object_coverage_notes "${object_dir}/*.gcno")
    list(APPEND coverage_notes ${object_coverage_notes})
endforeach()

if(NOT coverage_notes)
    message(FATAL_ERROR "No .gcno files found. Build with RJSJ_ENABLE_COVERAGE=ON first.")
endif()

list(REMOVE_DUPLICATES coverage_notes)

file(REMOVE_RECURSE "${OUTPUT_DIR}")
file(MAKE_DIRECTORY "${OUTPUT_DIR}")

set(summary_file "${OUTPUT_DIR}/llvm-cov-gcov-summary.txt")
file(WRITE "${summary_file}" "")

foreach(note_file IN LISTS coverage_notes)
    execute_process(
        COMMAND
            "${LLVM_COV_EXECUTABLE}"
            gcov
            -p
            -r
            -s
            "${SOURCE_DIR}"
            "${note_file}"
        WORKING_DIRECTORY "${OUTPUT_DIR}"
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error_output
    )

    file(APPEND "${summary_file}" "$ ${LLVM_COV_EXECUTABLE} gcov -p -r -s ${SOURCE_DIR} ${note_file}\n")
    file(APPEND "${summary_file}" "${output}")

    if(error_output)
        file(APPEND "${summary_file}" "${error_output}")
    endif()

    file(APPEND "${summary_file}" "\n")

    if(NOT result EQUAL 0)
        message(FATAL_ERROR "llvm-cov gcov failed for ${note_file}. See ${summary_file}.")
    endif()
endforeach()

message(STATUS "llvm-cov gcov files written to ${OUTPUT_DIR}")
message(STATUS "llvm-cov gcov summary written to ${summary_file}")
