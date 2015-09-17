# only build tests on targets that declare they are like posix
if(DEFINED TARGET_LIKE_X86_WINDOWS_NATIVE OR DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    
    # describe the test executable
    add_executable(mbed_client_trace_test Test.cpp)
    
    include_directories("../yotta_modules/cpputest")
    
    # describe what the test executable needs to link with
    target_link_libraries(mbed_client_trace_test "mbed-client-trace" cpputest)
    
    # describe what is actual test binary
    if(DEFINED TARGET_LIKE_X86_WINDOWS_NATIVE)
        add_test(mbed_client_trace_test "build/x86-windows-native/test/mbed_client_trace_test")
    endif()
    if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
        add_test(mbed_client_trace_test "../../../build/x86-linux-native/test/mbed_client_trace_test")
    endif()
    
endif()