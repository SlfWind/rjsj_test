if(NOT DEFINED BINARY_DIR)
    message(FATAL_ERROR "BINARY_DIR is required")
endif()

file(GLOB_RECURSE coverage_data "${BINARY_DIR}/*.gcda")

if(coverage_data)
    file(REMOVE ${coverage_data})
endif()
