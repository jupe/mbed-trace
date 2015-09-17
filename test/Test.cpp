/*
 * Copyright (c) 2014 ARM. All rights reserved.
 */
/**
 * \file \test_libTrace\Test.c
 *
 * \brief Unit tests for libTrace
 */
#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#include "mbed-cpputest/CppUTest/TestHarness.h"
#include "mbed-cpputest/CppUTest/SimpleString.h"
#include "mbed-cpputest/CppUTest/CommandLineTestRunner.h"

#define MBED_CLIENT_TRACE_FEA_IPV6 1
#define YOTTA_CFG_MBED_CLIENT_TRACE
#include "mbed-client-trace/mbed_client_trace.h"

int main(int ac, char **av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
char buf[1024];
#include <stdio.h>
void myprint(const char* str)
{
  strcpy(buf, str);
}

TEST_GROUP(trace)
{
  void setup()
  {

    mbed_client_trace_init();
    mbed_client_trace_config_set(TRACE_MODE_PLAIN|TRACE_ACTIVE_LEVEL_ALL);
    mbed_client_trace_print_function_set( myprint ); 
  }
  void teardown()
  {
    mbed_client_trace_free();
  }
};

/* Unity test code starts */
TEST(trace, Array)
{
  unsigned char longStr[1000] = {0x66};
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", mbed_trace_array(longStr, 1000) );
}

TEST(trace, LongString)
{
  const char longStr[200] = {0x36};
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", longStr );
}
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
#ifdef COMMON_FUNCTIONS_FN
TEST(trace, ipv6)
{
    uint8_t prefix[] = { 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int prefix_len = 64;
    char *str = mbed_trace_ipv6_prefix(prefix, prefix_len);
    STRCMP_EQUAL("146e:a00::/64", str);
}
#endif
TEST(trace, active_level_all_ipv6)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  
  uint8_t arr[] = { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0,1,0,0,0,0,0,1 };
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "my addr: %s", mbed_trace_ipv6(arr));
  STRCMP_EQUAL("[DBG ][mygr]: my addr: 2001:db8::1:0:0:1", buf);
}
#endif

TEST(trace, change_levels)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("[DBG ][mygr]: hep", buf);
  
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG|TRACE_MODE_PLAIN);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("hep", buf);
  
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG|TRACE_MODE_COLOR);
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "hep");
  STRCMP_EQUAL("\x1b[31m[ERR ][mygr]: hep\x1b[0m", buf);
  
}

TEST(trace, active_level_debug)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_DEBUG);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("[DBG ][mygr]: hep", buf);
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("[INFO][mygr]: test", buf);
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("[WARN][mygr]: hups", buf);
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}

TEST(trace, active_level_info)
{
  buf[0] = 0;
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_INFO);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("[INFO][mygr]: test", buf);
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("[WARN][mygr]: hups", buf);
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}

TEST(trace, active_level_warn)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_WARN);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("[WARN][mygr]: hups", buf);
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}

TEST(trace, active_level_error)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ERROR);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("[ERR ][mygr]: o'ou", buf);
}
TEST(trace, active_level_none)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_NONE);
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  STRCMP_EQUAL("", mbed_trace_last());
}

TEST(trace, active_level_all_1)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_client_trace_exclude_filters_set("mygr");
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  STRCMP_EQUAL("[DBG ][mygu]: hep", buf);
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
}
TEST(trace, active_level_all_2)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_client_trace_exclude_filters_set("mygr,mygu");
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("", mbed_trace_last());
}
TEST(trace, active_level_all_3)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_client_trace_include_filters_set("mygr");
  
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  STRCMP_EQUAL("", mbed_trace_last());
  
  mbed_tracef(TRACE_LEVEL_INFO, "mygr", "test");
  STRCMP_EQUAL("[INFO][mygr]: test", buf);
}

TEST(trace, active_level_all_array)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  
  uint8_t arr[] = {0x01, 0x02, 0x03};
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "my addr: %s", mbed_trace_array(arr, 3));
  STRCMP_EQUAL("[DBG ][mygr]: my addr: 01:02:03", buf);
}


size_t time_length;
char trace_prefix_str[] = "[<TIME>]";
char* trace_prefix(size_t length){ 
  time_length = length;
  return trace_prefix_str; 
}
TEST(trace, prefix)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_client_trace_prefix_function_set( &trace_prefix );
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  STRCMP_EQUAL("[<TIME>][DBG ][mygr]: test", buf);
  //TEST_ASSERT_EQUAL_INT(4, time_length);
  
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_PLAIN);
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  STRCMP_EQUAL("test", buf);
  
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_COLOR);
  mbed_tracef(TRACE_LEVEL_ERROR, "mygr", "test");
  STRCMP_EQUAL("\x1b[31m[<TIME>][ERR ][mygr]: test\x1b[0m", buf);
}
char trace_suffix_str[] = "[END]";
char* trace_suffix()
{
  return trace_suffix_str;
}
TEST(trace, suffix)
{
  mbed_client_trace_config_set(TRACE_ACTIVE_LEVEL_ALL);
  mbed_client_trace_prefix_function_set( &trace_prefix );
  mbed_client_trace_suffix_function_set( &trace_suffix );
  mbed_tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  STRCMP_EQUAL("[<TIME>][DBG ][mygr]: test[END]", buf);
}


