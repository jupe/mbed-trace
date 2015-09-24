if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    add_library( mbed-client-trace
        mbed_client_trace.c
        mbed_client_trace_ip6tos.c
    )
    add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
    target_link_libraries(mbed-client-trace gcov)
else()
    add_library( mbed-client-cli 
        ns_cmdline.c
        ns_list.c
    )
    target_link_libraries(mbed-client-cli)
endif()
