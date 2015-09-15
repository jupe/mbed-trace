/*
 * Copyright (c) 2014-2015 ARM Limited. All rights reserved.
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
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef YOTTA_CFG_MBED_CLIENT_TRACE
#define YOTTA_CFG_MBED_CLIENT_TRACE
#endif

#include "mbed-client-trace/mbed_client_trace.h"
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
#include "mbed-client-trace/mbed_client_trace_ip6string.h"
#endif
#if defined(_WIN32) || defined(__unix__) || defined(__unix) || defined(unix) || defined(YOTTA_CFG)
// typedef
#include <stdint.h>
#include <stddef.h>

#ifndef MEM_ALLOC
#define MEM_ALLOC malloc
#endif
#ifndef MEM_FREE
#define MEM_FREE free
#endif

#else // _WIN32|__unix__|__unix|unix

#include "nsdynmemLIB.h"
#ifndef MEM_ALLOC
#define MEM_ALLOC ns_dyn_mem_alloc
#endif
#ifndef MEM_FREE
#define MEM_FREE ns_dyn_mem_free
#endif

#endif

#define VT100_COLOR_ERROR "\x1b[31m"
#define VT100_COLOR_WARN  "\x1b[33m"
#define VT100_COLOR_INFO  "\x1b[39m"
#define VT100_COLOR_DEBUG "\x1b[90m"

/** default max trace line size in bytes */
#define DEFAULT_TRACE_LINE_LENGTH         1024
/** default max temporary buffer size in bytes, used in
    trace_ipv6, trace_array and trace_strn */
#define DEFAULT_TRACE_TMP_LINE_LEN        128
/** default max filters (include/exclude) length in bytes */
#define DEFAULT_TRACE_FILTER_LENGTH       24

/** default print function, just redirect str to printf */
static void mbed_client_trace_default_print(const char *str);

typedef struct trace_s {
    /** trace configuration bits */
    uint8_t trace_config;
    /** exclude filters list, related group name */
    char *filters_exclude;
    /** include filters list, related group name */
    char *filters_include;
    /** Filters length */
    int filters_length;
    /** trace line */
    char *line;
    /** trace line length */
    int line_length;
    /** temporary data */
    char *tmp_data;
    /** temporary data array length */
    int tmp_data_length;
    /** temporary data pointer */
    char *tmp_data_ptr;

    /** prefix function, which can be used to put time to the trace line */
    char *(*prefix_f)(size_t);
    /** suffix function, which can be used to some string to the end of trace line */
    char *(*suffix_f)(void);
    /** print out function. Can be redirect to flash for example. */
    void (*printf)(const char *);
    /** print out function for TRACE_LEVEL_CMD */
    void (*cmd_printf)(const char *);
} trace_t;

#define YOTTA_CFG_TRACE
#include "mbed-client-trace/mbed_client_trace.h"
#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
#include "mbed-client-trace/mbed_client_trace_ip6string.h"
#endif


trace_t *g_trace;

const char* mbed_client_trace_get_version(void)
{
    return YOTTA_COMPONENT_VERSION;
}

trace_t* mbed_client_trace_init(trace_t *self)
{
    if( !self ){
        g_trace = malloc(sizeof(trace_t));
        if( !g_trace ) {
            return NULL;
        }
        self = g_trace;
    }
    self->filters_exclude = 0;
    self->filters_include = 0;
    self->line = 0;
    self->tmp_data = 0;
    self->prefix_f = 0;
    self->suffix_f = 0;
    self->printf  = 0;
    self->cmd_printf = 0;

    self->trace_config = TRACE_MODE_COLOR | TRACE_ACTIVE_LEVEL_ALL | TRACE_CARRIAGE_RETURN;
    self->line_length = DEFAULT_TRACE_LINE_LENGTH;
    if (self->line == NULL) {
        self->line = MEM_ALLOC(self->line_length);
    }
    self->tmp_data_length = DEFAULT_TRACE_TMP_LINE_LEN;
    if (self->tmp_data == NULL) {
        self->tmp_data = MEM_ALLOC(self->tmp_data_length);
    }
    self->tmp_data_ptr = self->tmp_data;
    self->filters_length = DEFAULT_TRACE_FILTER_LENGTH;
    if (self->filters_exclude == NULL) {
        self->filters_exclude = MEM_ALLOC(self->filters_length);
    }
    if (self->filters_include == NULL) {
        self->filters_include = MEM_ALLOC(self->filters_length);
    }

    if (self->line == NULL ||
        self->tmp_data == NULL ||
        self->filters_exclude == NULL  ||
        self->filters_include == NULL) {
        //memory allocation fail
        self = mbed_client_trace_free(self);
        return self;
    }
    memset(self->tmp_data, 0, self->tmp_data_length);
    memset(self->filters_exclude, 0, self->filters_length);
    memset(self->filters_include, 0, self->filters_length);
    memset(self->line, 0, self->line_length);

    self->prefix_f = 0;
    self->suffix_f = 0;
    self->printf = mbed_client_trace_default_print;
    self->cmd_printf = 0;

    return self;
}
trace_t* mbed_client_trace_free(trace_t *self)
{
    if(!self){
        self = g_trace;
    }
    if(!self){
        return NULL;
    }
    MEM_FREE(self->line);
    self->line_length = 0;
    self->line = 0;
    MEM_FREE(self->tmp_data);
    self->tmp_data = 0;
    self->tmp_data_ptr = 0;
    MEM_FREE(self->filters_exclude);
    self->filters_exclude = 0;
    MEM_FREE(self->filters_include);
    self->filters_include = 0;
    self->filters_length = 0;
    free(self);
    self = NULL;
    return self;
}
/** @TODO do we need dynamically change trace buffer sizes ?
// reconfigure trace buffer sizes
void set_trace_buffer_sizes(int lineLength, int tmpLength)
{
  REALLOC( self->line, dataLength );
  REALLOC( self->tmp_data, tmpLength);
  self->tmp_data_length = tmpLength;
}
*/
void mbed_client_trace_config_set( trace_t* self, uint8_t config)
{
    self->trace_config = config;
}
uint8_t mbed_client_trace_config_get( trace_t* self)
{
    return self->trace_config;
}
void mbed_client_trace_prefix_function_set( trace_t* self, char *(*pref_f)(size_t))
{
    self->prefix_f = pref_f;
}
void mbed_client_trace_suffix_function_set( trace_t* self, char *(*suffix_f)(void))
{
    self->suffix_f = suffix_f;
}
void mbed_client_trace_print_function_set( trace_t* self, void (*printf)(const char *))
{
    self->printf = printf;
}
void mbed_client_trace_cmdprint_function_set( trace_t* self, void (*printf)(const char *))
{
    self->cmd_printf = printf;
}
void mbed_client_trace_exclude_filters_set( trace_t* self, char *filters)
{
    if (filters) {
        (void)strncpy(self->filters_exclude, filters, self->filters_length);
    } else {
        self->filters_exclude[0] = 0;
    }
}
const char *mbed_client_trace_exclude_filters_get( trace_t* self)
{
    return self->filters_exclude;
}
const char *mbed_client_trace_include_filters_get( trace_t* self)
{
    return self->filters_include;
}
void mbed_client_trace_include_filters_set( trace_t* self, char *filters)
{
    if (filters) {
        (void)strncpy(self->filters_include, filters, self->filters_length);
    } else {
        self->filters_include[0] = 0;
    }
}
static int8_t mbed_client_trace_skip( trace_t* self, int8_t dlevel, const char *grp)
{
    if (dlevel >= 0 && grp != 0) {
        // filter debug prints only when dlevel is >0 and grp is given

        /// @TODO self could be much better..
        if (self->filters_exclude[0] != '\0' &&
                strstr(self->filters_exclude, grp) != 0) {
            //grp was in exclude list
            return 1;
        }
        if (self->filters_include[0] != '\0' &&
                strstr(self->filters_include, grp) == 0) {
            //grp was in include list
            return 1;
        }
    }
    return 0;
}
static void mbed_client_trace_default_print(const char *str)
{
    puts(str);
}
void mbed_tracef( trace_t* self, uint8_t dlevel, const char *grp, const char *fmt, ...)
{
    self->line[0] = 0; //by default trace is empty
    if (mbed_client_trace_skip(self, dlevel, grp) || fmt == 0 || grp == 0) {
        return;
    }
    if ((self->trace_config & TRACE_MASK_LEVEL) &  dlevel) {
        bool color = (self->trace_config & TRACE_MODE_COLOR) != 0;
        bool plain = (self->trace_config & TRACE_MODE_PLAIN) != 0;
        bool cr    = (self->trace_config & TRACE_CARRIAGE_RETURN) != 0;

        int retval = 0, bLeft = self->line_length;
        char *ptr = self->line;
        if (plain == true || dlevel == TRACE_LEVEL_CMD) {
            va_list ap;
            va_start(ap, fmt);
            //add trace data
            retval = vsnprintf(ptr, bLeft, fmt, ap);
            va_end(ap);
            if (dlevel == TRACE_LEVEL_CMD && self->cmd_printf) {
                self->cmd_printf(self->line);
                self->cmd_printf("\n");
            } else {
                //print out whole data
                self->printf(self->line);
            }
        } else {
            if (color) {
                if (cr) {
                    retval = snprintf(ptr, bLeft, "\r\x1b[2K");
                    if (retval >= bLeft) {
                        retval = 0;
                    }
                    if (retval > 0) {
                        ptr += retval;
                        bLeft -= retval;
                    }
                }
                if (bLeft > 0) {
                    //include color in ANSI/VT100 escape code
                    switch (dlevel) {
                        case (TRACE_LEVEL_ERROR):
                            retval = snprintf(ptr, bLeft, "%s", VT100_COLOR_ERROR);
                            break;
                        case (TRACE_LEVEL_WARN):
                            retval = snprintf(ptr, bLeft, "%s", VT100_COLOR_WARN);
                            break;
                        case (TRACE_LEVEL_INFO):
                            retval = snprintf(ptr, bLeft, "%s", VT100_COLOR_INFO);
                            break;
                        case (TRACE_LEVEL_DEBUG):
                            retval = snprintf(ptr, bLeft, "%s", VT100_COLOR_DEBUG);
                            break;
                        default:
                            color = 0; //avoid unneeded color-terminate code
                            retval = 0;
                            break;
                    }
                    if (retval >= bLeft) {
                        retval = 0;
                    }
                    if (retval > 0 && color) {
                        ptr += retval;
                        bLeft -= retval;
                    }
                }

            }
            if (bLeft > 0 && self->prefix_f) {
                va_list ap;
                va_start(ap, fmt);
                //find out length of body
                size_t sz = 0;
                sz = vsnprintf(NULL, 0, fmt, ap) + retval + (retval ? 4 : 0);
                //add prefix string
                retval = snprintf(ptr, bLeft, "%s", self->prefix_f(sz));
                if (retval >= bLeft) {
                    retval = 0;
                }
                if (retval > 0) {
                    ptr += retval;
                    bLeft -= retval;
                }
                va_end(ap);
            }
            if (bLeft > 0) {
                //add group tag
                switch (dlevel) {
                    case (TRACE_LEVEL_ERROR):
                        retval = snprintf(ptr, bLeft, "[ERR ][%-4s]: ", grp);
                        break;
                    case (TRACE_LEVEL_WARN):
                        retval = snprintf(ptr, bLeft, "[WARN][%-4s]: ", grp);
                        break;
                    case (TRACE_LEVEL_INFO):
                        retval = snprintf(ptr, bLeft, "[INFO][%-4s]: ", grp);
                        break;
                    case (TRACE_LEVEL_DEBUG):
                        retval = snprintf(ptr, bLeft, "[DBG ][%-4s]: ", grp);
                        break;
                    default:
                        retval = snprintf(ptr, bLeft, "              ");
                        break;
                }
                if (retval >= bLeft) {
                    retval = 0;
                }
                if (retval > 0) {
                    ptr += retval;
                    bLeft -= retval;
                }
            }
            if (retval > 0 && bLeft > 0) {
                va_list ap;
                va_start(ap, fmt);
                //add trace text
                retval = vsnprintf(ptr, bLeft, fmt, ap);
                if (retval >= bLeft) {
                    retval = 0;
                }
                if (retval > 0) {
                    ptr += retval;
                    bLeft -= retval;
                }
                va_end(ap);
            }

            if (retval > 0 && bLeft > 0  && self->suffix_f) {
                //add suffix string
                retval = snprintf(ptr, bLeft, "%s", self->suffix_f());
                if (retval >= bLeft) {
                    retval = 0;
                }
                if (retval > 0) {
                    ptr += retval;
                    bLeft -= retval;
                }
            }

            if (retval > 0 && bLeft > 0  && color) {
                //add zero color VT100 when color mode
                retval = snprintf(ptr, bLeft, "\x1b[0m");
                if (retval >= bLeft) {
                    retval = 0;
                }
                if (retval > 0) {
                    ptr += retval;
                    bLeft -= retval;
                }
            }
            //print out whole data
            self->printf(self->line);
        }
        //return tmp data pointer back to the beginning
        self->tmp_data_ptr = self->tmp_data;
    }
}
const char *mbed_trace_last(trace_t* self)
{
    return self->line;
}
/* Helping functions */
#define tmp_data_left()  self->tmp_data_length-(self->tmp_data_ptr-self->tmp_data)

#if MBED_CLIENT_TRACE_FEA_IPV6 == 1
char *mbed_trace_ipv6(trace_t* self, const void *addr_ptr)
{
    char *str = self->tmp_data_ptr;
    if (str == NULL) {
        return "";
    }
    if (tmp_data_left() < 41) {
        return "";
    }
    if (addr_ptr == NULL) {
        return "<null>";
    }
    str[0] = 0;
    mbed_client_trace_ip6tos(addr_ptr, str);
    self->tmp_data_ptr += strlen(str) + 1;
    return str;
}
char *mbed_trace_ipv6_prefix(trace_t* self, const uint8_t *prefix, uint8_t prefix_len)
{
    char *str = self->tmp_data_ptr;
    int retval, bLeft = tmp_data_left();
    char tmp[40];
    uint8_t addr[16] = {0};

    if (str == NULL) {
        return "";
    }
    if (bLeft < 43) {
        return "";
    }

    if (prefix_len != 0) {
        if (prefix == NULL || prefix_len > 128) {
            return "<err>";
        }
#ifdef COMMON_FUNCTIONS_FN        
        bitcopy(addr, prefix, prefix_len);
#else
        return "";
#endif    
    }

    mbed_client_trace_ip6tos(addr, tmp);
    retval = snprintf(str, bLeft, "%s/%u", tmp, prefix_len);
    if (retval <= 0 || retval > bLeft) {
        return "";
    }

    self->tmp_data_ptr += retval + 1;
    return str;
}
#endif
char *mbed_trace_array(trace_t* self, const uint8_t *buf, uint16_t len)
{
    int i, retval, bLeft = tmp_data_left();
    char *str, *wptr;
    str = self->tmp_data_ptr;
    if (str == NULL) {
        return "";
    }
    if (buf == NULL) {
        return "<null>";
    }
    wptr = str;
    wptr[0] = 0;
    const uint8_t *ptr = buf;
    for (i = 0; i < len; i++) {
        if (bLeft <= 0) {
            break;
        }
        retval = snprintf(wptr, bLeft, "%02x:", *ptr++);
        if (retval <= 0 || retval > bLeft) {
            break;
        }
        bLeft -= retval;
        wptr += retval;
    }
    if (wptr > str) {
        *(wptr - 1) = 0;    //null to replace last ':' character
    }
    self->tmp_data_ptr = wptr;
    return str;
}
