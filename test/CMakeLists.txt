if(DEFINED TARGET_LIKE_X86_LINUX_NATIVE)
    FIND_PROGRAM( GCOV_PATH gcov )
    FIND_PROGRAM( LCOV_PATH lcov )
    FIND_PROGRAM( GENHTML_PATH genhtml )
    FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/test)
    IF(NOT GCOV_PATH)
	    MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
    ENDIF() # NOT GCOV_PATH
endif()


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
        # add coverage definitions
        add_definitions("-g -O0 -fprofile-arcs -ftest-coverage")
        target_link_libraries(mbed_client_trace_test gcov)
        # clean old coverage reports
        EXECUTE_PROCESS(COMMAND ${LCOV_PATH} --directory . --zerocounters)
        SET(TEST_EXECUTABLE "../../../build/x86-linux-native/test/mbed_client_trace_test")
        add_test(mbed_client_trace_test ${TEST_EXECUTABLE})
        #SET(LIB_OBJ "../../../build/x86-linux-native/test/CMakeFiles/mbed_client_trace_test.dir/Test.cpp.o")
        #add_test(mbed_client_trace_test ${GCOV_PATH} ${LIB_OBJ})
        #execute_process(COMMAND mv ../../../build/x86-linux-native/*.gcov .././../)
    endif()
    
endif()
