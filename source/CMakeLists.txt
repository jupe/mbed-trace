if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    FIND_PROGRAM( GCOV_PATH gcov )
    FIND_PROGRAM( LCOV_PATH lcov )
    FIND_PROGRAM( GENHTML_PATH genhtml )
    FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/test)
    IF(NOT GCOV_PATH)
	    MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
    ENDIF() # NOT GCOV_PATH

    add_library( mbed-client-trace
        mbed_client_trace.c
        mbed_client_trace_ip6tos.c
    )
    add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
    target_link_libraries(mbed-client-trace gcov)

endif()
