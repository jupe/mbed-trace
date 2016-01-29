/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * \file mbed_client_trace.h
 * Trace interface for MbedOS applications.
 * This file provide simple but flexible way to handle software traces.
 * Trace library are abstract layer, which use stdout (printf) by default, 
 * but outputs can be easily redirect to custom function, for example to 
 * store traces to memory or other interfaces.
 *
 *  usage example:
 * \code(main.c:)
 *      #include "mbed_client_trace.h"
 *      #define TRACE_GROUP  "main"
 *
 *      int main(void){
 *          mbed_client_trace_init(0);   // initialize trace library
 *          tr_debug("this is debug msg");  //print debug message to stdout: "[DBG]
 *          tr_err("this is error msg");
 *          tr_warn("this is warning msg");
 *          tr_info("this is info msg");
 *          mbed_client_trace_free(0);
 *          return 0;
 *      }
 * \endcode
 * Activate with compiler flag: YOTTA_CFG_MBED_CLIENT_TRACE
 *
 */
#ifndef MBED_CLIENT_TRACE_H_
#define MBED_CLIENT_TRACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef YOTTA_CFG
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#else
#include "ns_types.h"
#endif

struct trace_t;

#ifndef MBED_CLIENT_TRACE_FEA_IPV6
#define MBED_CLIENT_TRACE_FEA_IPV6 1
#endif

#ifndef MBED_CLIENT_TRACE_MANAGEMENT
// if C++ TraceManager is in use, we don't
// need this global tracing obejct
extern trace_t *g_trace;
#endif

/** 3 upper bits are trace modes related,
    and 5 lower bits are trace level configuration */

/** Config mask */
#define TRACE_MASK_CONFIG         0xE0
/** Trace level mask */
#define TRACE_MASK_LEVEL          0x1F

/** plain trace data instead of "headers" */
#define TRACE_MODE_PLAIN          0x80
/** color mode */
#define TRACE_MODE_COLOR          0x40
/** Use print CR before trace line */
#define TRACE_CARRIAGE_RETURN     0x20

/** used to activate all trace levels */
#define TRACE_ACTIVE_LEVEL_ALL    0x1F
/** print all traces same as above */
#define TRACE_ACTIVE_LEVEL_DEBUG  0x1f
/** print info,warn and error traces */
#define TRACE_ACTIVE_LEVEL_INFO   0x0f
/** print warn and error traces */
#define TRACE_ACTIVE_LEVEL_WARN   0x07
/** print only error trace */
#define TRACE_ACTIVE_LEVEL_ERROR  0x03
/** print only cmd line data */
#define TRACE_ACTIVE_LEVEL_CMD    0x01
/** trace nothing  */
#define TRACE_ACTIVE_LEVEL_NONE   0x00

/** this print is some deep information for debug purpose */
#define TRACE_LEVEL_DEBUG         0x10
/** Info print, for general purpose prints */
#define TRACE_LEVEL_INFO          0x08
/** warning prints, which shouldn't causes any huge problems */
#define TRACE_LEVEL_WARN          0x04
/** Error prints, which causes probably problems, e.g. out of mem. */
#define TRACE_LEVEL_ERROR         0x02
/** special level for cmdline. Behaviours like "plain mode" */
#define TRACE_LEVEL_CMD           0x01

//usage macros:
#define tr_info(...)            mbed_tracef(g_trace, TRACE_LEVEL_INFO,    TRACE_GROUP, __VA_ARGS__)   //!< Print info message
#define tr_debug(...)           mbed_tracef(g_trace, TRACE_LEVEL_DEBUG,   TRACE_GROUP, __VA_ARGS__)   //!< Print debug message
#define tr_warning(...)         mbed_tracef(g_trace, TRACE_LEVEL_WARN,    TRACE_GROUP, __VA_ARGS__)   //!< Print warning message
#define tr_warn(...)            mbed_tracef(g_trace, TRACE_LEVEL_WARN,    TRACE_GROUP, __VA_ARGS__)   //!< Alternative warning message
#define tr_error(...)           mbed_tracef(g_trace, TRACE_LEVEL_ERROR,   TRACE_GROUP, __VA_ARGS__)   //!< Print Error Message
#define tr_err(...)             mbed_tracef(g_trace, TRACE_LEVEL_ERROR,   TRACE_GROUP, __VA_ARGS__)   //!< Alternative error message
#define tr_cmdline(...)         mbed_tracef(g_trace, TRACE_LEVEL_CMD,     TRACE_GROUP, __VA_ARGS__)   //!< Special print for cmdline. See more from TRACE_LEVEL_CMD -level

#if defined(BACKWARD_COMPATIBLE)
#define trace_last(...)                (g_trace, __VA_ARGS__)
#define tracef(...)                    (g_trace, __VA_ARGS__)
#define trace_array(...)               (g_trace, __VA_ARGS__)
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
#define trace_ipv6(...)                (g_trace, __VA_ARGS__)
#define trace_ipv6_prefix(...)         (g_trace, __VA_ARGS__)
#endif
#endif

/** Possible to skip all traces in compile time */
#if defined(YOTTA_CFG_TRACE)

#if defined  __GNUC__ || defined __CC_ARM

/**
 * Get Yotta component version
 * @return version number as string (e.g. "0.1.0")
 */
const char* mbed_client_trace_get_version( trace_t* self );
/**
 * Create trace instance and initialize trace functionality
 * @param this  NULL when global 
 * @return trace object when all success, otherwise zero
 * 
 */
trace_t* mbed_client_trace_init( trace_t* self );
/**
 * Free trace memory
 */
trace_t* mbed_client_trace_free(  trace_t* self );
/**
 *  Set trace configurations
 *  Possible parameters:
 *
 *   TRACE_MODE_COLOR
 *   TRACE_MODE_PLAIN (self exclude color mode)
 *   TRACE_CARRIAGE_RETURN (print CR before trace line)
 *
 *   TRACE_ACTIVE_LEVEL_ALL - to activate all trace levels
 *   or TRACE_ACTIVE_LEVEL_DEBUG (alternative)
 *   TRACE_ACTIVE_LEVEL_INFO
 *   TRACE_ACTIVE_LEVEL_WARN
 *   TRACE_ACTIVE_LEVEL_ERROR
 *   TRACE_ACTIVE_LEVEL_CMD
 *   TRACE_LEVEL_NONE - to deactivate all traces
 *
 * @param config  Byte size Bit-mask. Bits are descripted above.
 * usage e.g.
 * @code
 *  mbed_client_trace_config_set(g_trace, TRACE_ACTIVE_LEVEL_ALL|TRACE_MODE_COLOR );
 * @endcode
 */
void mbed_client_trace_config_set( trace_t* self, uint8_t config);
/** get trace configurations 
 * @return trace configuration byte
 */
uint8_t mbed_client_trace_config_get( trace_t* self);
/**
 * Set trace prefix function
 * pref_f -function return string with null terminated
 * Can be used for e.g. time string
 * e.g.
 *   char* trace_time(){ return "rtc-time-in-string"; }
 *   mbed_client_trace_prefix_function_set( &trace_time );
 */
void mbed_client_trace_prefix_function_set(  trace_t* self, char* (*pref_f)(size_t) );
/**
 * Set trace suffix function
 * suffix -function return string with null terminated
 * Can be used for e.g. time string
 * e.g.
 *   char* trace_suffix(){ return " END"; }
 *   mbed_client_trace_suffix_function_set(g_trace,  &trace_suffix );
 */
void mbed_client_trace_suffix_function_set(  trace_t* self, char* (*suffix_f)(void) );
/**
 * Set trace print function
 * By default, trace module print using printf() function,
 * but with self you can write own print function,
 * for e.g. to other IO device.
 */
void mbed_client_trace_print_function_set(  trace_t* self, void (*print_f)(const char*) );
/**
 * Set trace print function for tr_cmdline()
 */
void mbed_client_trace_cmdprint_function_set(  trace_t* self, void (*printf)(const char*) );
/**
 * When trace group contains text in filters,
 * trace print will be ignored.
 * e.g.: 
 *  mbed_client_trace_exclude_filters_set(g_trace, "mygr");
 *  mbed_tracef(g_trace, TRACE_ACTIVE_LEVEL_DEBUG, "ougr", "This is not printed");
 */
void mbed_client_trace_exclude_filters_set( trace_t* self, char* filters);
/** get trace exclude filters
 */
const char* mbed_client_trace_exclude_filters_get( trace_t* self);
/**
 * When trace group contains text in filter,
 * trace will be printed.
 * e.g.:
 *  set_trace_include_filters(g_trace, "mygr");
 *  mbed_tracef(g_trace, TRACE_ACTIVE_LEVEL_DEBUG, "mygr", "Hi There");
 *  mbed_tracef(g_trace, TRACE_ACTIVE_LEVEL_DEBUG, "grp2", "This is not printed");
 */
void mbed_client_trace_include_filters_set( trace_t* self, char* filters);
/** get trace include filters
 */
const char* mbed_client_trace_include_filters_get( trace_t* self);
/**
 * General trace function
 * This should be used every time when user want to print out something important thing
 * Usage e.g.
 *   mbed_tracef( g_trace, TRACE_LEVEL_INFO, "mygr", "Hello world!");
 *
 * @param dlevel debug level
 * @param grp    trace group
 * @param fmt    trace format (like printf)
 * @param ...    variable arguments related to fmt
 */
void mbed_tracef( trace_t* self, uint8_t dlevel, const char* grp, const char *fmt, ...) __attribute__ ((__format__(__printf__, 4, 5)));
/**
 *  Get last trace from buffer
 */
const char* mbed_trace_last(trace_t* self);
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
/**
 * tracef helping function for convert ipv6
 * table to human readable string.
 * usage e.g.
 * char ipv6[16] = {...}; // ! array length is 16 bytes !
 * mbed_tracef(g_trace, TRACE_LEVEL_INFO, "mygr", "ipv6 addr: %s", trace_ipv6(g_trace, ipv6));
 *
 * @param add_ptr  IPv6 Address pointer
 * @return temporary buffer where ipv6 is in string format
 */
char* mbed_trace_ipv6( trace_t* self, const void *addr_ptr);
/**
 * tracef helping function for print ipv6 prefix
 * usage e.g.
 * char ipv6[16] = {...}; // ! array length is 16 bytes !
 * mbed_tracef(g_trace, TRACE_LEVEL_INFO, "mygr", "ipv6 addr: %s", trace_ipv6_prefix(g_trace, ipv6, 4));
 *
 * @param prefix        IPv6 Address pointer
 * @param prefix_len    prefix length
 * @return temporary buffer where ipv6 is in string format
 */
char* mbed_trace_ipv6_prefix( trace_t* self, const uint8_t *prefix, uint8_t prefix_len);
#endif
/**
 * tracef helping function for convert hex-array to string.
 * usage e.g.
 *  char myarr[] = {0x10, 0x20};
 *  mbed_tracef(g_trace, TRACE_LEVEL_INFO, "mygr", "arr: %s", trace_array(myarr, 2));
 *
 * @param buf  hex array pointer
 * @param len  buffer length
 * @return temporary buffer where string copied
 */
char* mbed_trace_array( trace_t* self, const uint8_t* buf, uint16_t len);

#else //__GNUC__ || __CC_ARM
const char* mbed_client_trace_get_version(void);
trace_t*  mbed_client_trace_init(  trace_t* self );
trace_t*  mbed_client_trace_free(  trace_t* self );
void mbed_client_trace_config_set( trace_t* self, uint8_t config);
void mbed_client_trace_prefix_function_set(  trace_t* self, char* (*pref_f)(size_t) );
void mbed_client_trace_suffix_function_set(  trace_t* self, char* (*suffix_f)(void) );
void mbed_client_trace_print_function_set(  trace_t* self, void (*print_f)(const char*) );
void mbed_client_trace_cmdprint_function_set(  trace_t* self, void (*printf)(const char*) );
void mbed_client_trace_exclude_filters_set( trace_t* self, char* filters);
const char* mbed_client_trace_exclude_filters_get( trace_t* self);
void mbed_client_trace_include_filters_set( trace_t* self, char* filters);
const char* mbed_client_trace_include_filters_get( trace_t* self);
void mbed_tracef( trace_t* self, uint8_t dlevel, const char* grp, const char *fmt, ...);
const char* mbed_trace_last( trace_t* self);
char* mbed_trace_ipv6(trace_t* self, const void *addr_ptr);
char* mbed_trace_array(trace_t* self, const uint8_t* buf, uint16_t len);
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
char* mbed_trace_ipv6(trace_t* self, const void *addr_ptr);
char* mbed_trace_ipv6_prefix(trace_t* self, const uint8_t *prefix, uint8_t prefix_len);       
#endif

#endif


#else // __GNUC__ || __CC_ARM

// trace functionality not supported
#define mbed_client_trace_init(...)                ((void) 0)
#define mbed_client_trace_free(...)                ((void) 0)
#define mbed_client_trace_config_set(...)          ((void) 0)
#define mbed_client_trace_prefix_function_set(...) ((void) 0)
#define mbed_client_trace_suffix_function_set(...) ((void) 0)
#define mbed_client_trace_print_function_set(...)  ((void) 0)
#define mbed_client_trace_cmdprint_function_set(...)  ((void) 0)
#define mbed_client_trace_exclude_filters_get(...) ((void) 0)
#define mbed_client_trace_include_filters_set(...) ((void) 0)
#define mbed_client_trace_include_filters_get(...) ((void) 0)

#define mbed_trace_last(...)                ((void) 0)
#define mbed_tracef(...)                    ((void) 0)
#define mbed_trace_array(...)               ((void) 0)
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
#define mbed_trace_ipv6(...)                ((void) 0)
#define mbed_trace_ipv6_prefix(...)         ((void) 0)
#endif

#endif // __GNUC__ || __CC_ARM

#ifdef __cplusplus
}
#endif

#endif /* MBED_CLIENT_TRACE_H_ */
