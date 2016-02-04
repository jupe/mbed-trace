if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    add_library( mbed_trace
        mbed_trace.c
        mbed_trace_ip6tos.c
    )
    add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
    target_link_libraries(mbed_trace gcov)
else()
    add_library( mbed_trace
        mbed_trace.c
        mbed_trace_ip6tos.c
    )
    target_link_libraries(mbed_trace)
endif()
