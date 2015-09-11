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

#include "unity.h"
#include "ns_trace.h"

char buf[1024];

static char* prefix(int bodylen)
{
}
static char* suffix(void)
{
}
static void myprint(const char* str)
{
  strcpy(buf, str);
}

/* Unity test code starts */
void setUp(void)
{
  trace_init();
  set_trace_print_function( myprint ); 
  
}

void tearDown(void)
{
  trace_free();
}

void test_0(void)
{
  unsigned char longStr[1000] = {0x66};
  tracef(TRACE_LEVEL_DEBUG, "mygr", "%s", trace_array(longStr, 1000) );
}
void test_0_1(void)
{
  const char longStr[200] = {0x36};
  tracef(TRACE_LEVEL_DEBUG, "mygr", longStr );
}
void test_0_2(void)
{
    uint8_t prefix[] = { 0x14, 0x6e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00 };
    int prefix_len = 64;
    char *str = trace_ipv6_prefix(prefix, prefix_len);
    TEST_ASSERT_EQUAL_STRING("146e:a00::/64", str);
}

void test_1_0(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_DEBUG);
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("[DBG ][mygr]: hep", buf);
  
  set_trace_config(TRACE_ACTIVE_LEVEL_DEBUG|TRACE_MODE_PLAIN);
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("hep", buf);
  
  set_trace_config(TRACE_ACTIVE_LEVEL_DEBUG|TRACE_MODE_COLOR);
  tracef(TRACE_LEVEL_ERROR, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("\x1b[31m[ERR ][mygr]: hep\x1b[0m", buf);
  
}

void test_1_1(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_DEBUG);
  
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("[DBG ][mygr]: hep", buf);
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("[INFO][mygr]: test", buf);
  
  tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  TEST_ASSERT_EQUAL_STRING("[WARN][mygr]: hups", buf);
  
  tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  TEST_ASSERT_EQUAL_STRING("[ERR ][mygr]: o'ou", buf);
}

void test_1_2(void)
{
  buf[0] = 0;
  set_trace_config(TRACE_ACTIVE_LEVEL_INFO);
  
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("[INFO][mygr]: test", buf);
  
  tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  TEST_ASSERT_EQUAL_STRING("[WARN][mygr]: hups", buf);
  
  tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  TEST_ASSERT_EQUAL_STRING("[ERR ][mygr]: o'ou", buf);
}

void test_1_3(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_WARN);
  
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  TEST_ASSERT_EQUAL_STRING("[WARN][mygr]: hups", buf);
  
  tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  TEST_ASSERT_EQUAL_STRING("[ERR ][mygr]: o'ou", buf);
}

void test_1_4(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ERROR);
  
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  TEST_ASSERT_EQUAL_STRING("[ERR ][mygr]: o'ou", buf);
}

void test_1_5(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ERROR);
  
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  TEST_ASSERT_EQUAL_STRING("[ERR ][mygr]: o'ou", buf);
}
void test_1_6(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_NONE);
  
  tracef(TRACE_LEVEL_DEBUG, "mygr", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_WARN, "mygr", "hups");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_ERROR, "mygr", "o'ou");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
}

void test_2_1(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  set_trace_exclude_filters("mygr");
  
  tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  TEST_ASSERT_EQUAL_STRING("[DBG ][mygu]: hep", buf);
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
}
void test_2_2(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  set_trace_exclude_filters("mygr,mygu");
  
  tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
}
void test_2_3(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  set_trace_include_filters("mygr");
  
  tracef(TRACE_LEVEL_DEBUG, "mygu", "hep");
  TEST_ASSERT_EQUAL_STRING("", trace_last());
  
  tracef(TRACE_LEVEL_INFO, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("[INFO][mygr]: test", buf);
}

void test_3_1(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  
  uint8_t arr[] = {0x01, 0x02, 0x03};
  tracef(TRACE_LEVEL_DEBUG, "mygr", "my addr: %s", trace_array(arr, 3));
  TEST_ASSERT_EQUAL_STRING("[DBG ][mygr]: my addr: 01:02:03", buf);
}
void test_3_2(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  
  uint8_t arr[] = { 0x20, 0x01, 0xd, 0xb8, 0,0,0,0,0,1,0,0,0,0,0,1 };
  tracef(TRACE_LEVEL_DEBUG, "mygr", "my addr: %s", trace_ipv6(arr));
  TEST_ASSERT_EQUAL_STRING("[DBG ][mygr]: my addr: 2001:db8::1:0:0:1", buf);
}

size_t time_length;
char* trace_time(size_t length){ 
  time_length = length;
  return "[<TIME>]"; 
}
void test_4_1(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  set_trace_prefix_function( &trace_time );
  tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("[<TIME>][DBG ][mygr]: test", buf);
  //TEST_ASSERT_EQUAL_INT(4, time_length);
  
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_PLAIN);
  tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("test", buf);
  
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_COLOR);
  tracef(TRACE_LEVEL_ERROR, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("\x1b[31m[<TIME>][ERR ][mygr]: test\x1b[0m", buf);
}

char* trace_suffix()
{
  return "[END]";
}
void test_4_2(void)
{
  set_trace_config(TRACE_ACTIVE_LEVEL_ALL);
  set_trace_prefix_function( &trace_time );
  set_trace_suffix_function( &trace_suffix );
  tracef(TRACE_LEVEL_DEBUG, "mygr", "test");
  TEST_ASSERT_EQUAL_STRING("[<TIME>][DBG ][mygr]: test[END]", buf);
}