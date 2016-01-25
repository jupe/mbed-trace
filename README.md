# mbed-client-trace

General purpose tracing abstraction library for mbed devices.

## Description and Purpose

This library purpose is to provide light, simple and general tracing solution for mbed devices. By default it print traces to stdout (normally it means serial-port), but outputs can be redirect to anywhere. Library is developed with ansi C -language, but could be used with C++ as well. Currently there is no C++ wrapper available, but it could be created easily top of this library.

## Philosophy

* Library should be as light, fast, simple and as abstract than possible
* Minimal dependencies
* Initialization allocate required memory space once in application life-time, so no new malloc/free's needed when traces is used
* trace -methods should be as fast as possible
* after trace -method call it release print required resources
* trace method call produce single line, which contains `<level>`, `<group>` and `<message>`
* allow to filter messages on the fly, and in compile time (not fully supported)

## Compromises

* Traces are stored as a ascii arrays in flash memory (which take quite much memory space) because of it doesn't require..
 * to encode/decode trace messages on the fly which could take cpu time too much, or 
 * require external dev-env dependencies to encode traces on compile time and external application to decode traces afterward. (this could be future direct to go) 
* Group name length is limited to 4 characters, because of lines looks much cleaner and it is enough for most of use cases to separate module names. Maybe sometimes group names length is not suitable to create clean human readable format, but still 4 character is enought for unique module names.
* By default it uses stdout as output because it goes directly to serial port when it is initialized.
* By default it produce traces like: `[<levl>][grp ]: msg`, because that allows to easy way to detect trace prints and separate traces from normal prints (e.g. with regex)
* Require sprintf -implementation (`stdio.h`). This take quite much memory, but allows efficient way to format traces.
* is not thread safe (sorry, but current implementation not have mutexes, but PR's are more than wellcome)

## How traces looks like

```
[DBG ][abc ]: this is debug message from abc module<cr><lf>
[ERR ][abc ]: something goes wrong in abc module<cr><lf>
[WARN][br  ]: Oh nou, br warning occurs!<cr><lf>
[INFO][br  ]: Hi there<cr><lf>
```

## Usage

### Pre-requirements

* Initialize serial port so that stdout works. You can verify with `printf()` -function that serial port works.
 * if you want to redirect traces to somewhere else, see [trace api](https://github.com/ARMmbed/mbed-client-trace/blob/master/mbed-client-trace/mbed_client_trace.h#L170).
* Trace initialization (`mbed_client_trace_init`) need to be called once before using any other api's. It allocate trace buffer and initialize internal variables.
* define `TRACE_GROUP` in your source code (not in header!), where you want to use traces. `TRACE_GROUP` 1-4 characters long char-array (e.g. `#define TRACE_GROUP "APPL"`), which will be printed in every trace lines.

#### Traces

Any time when you want to print some traces, use tr_<level> -macros. macro's behaviour ~exactly same than `printf()`, so propably you are familiar with that already, e.g. by adding: `tr_debug("hello %s", "trace")` will produce trace line: `[DBG ][APPL] hello trace<cr><lf>` 

Available levels:
* debug
* warning
* error
* info
* cmdline (special behavior, normally shouldn't be used)

Initialization (once in application life-time)
```c
int mbed_client_trace_init(void);
```

Set output function, by default it uses `printf` 
```c
mbed_client_trace_print_function_set(printf)
```

#### Helping functions

Helping functions purpose is to provide simple conversions from e.g. array to C string, so that you can print everything to single trace line.

Available conversion functions:
```
char *mbed_trace_ipv6(const void *addr_ptr)
char *mbed_trace_ipv6_prefix(const uint8_t *prefix, uint8_t prefix_len)
char *mbed_trace_array(const uint8_t *buf, uint16_t len)
```

[see more mbed_client_trace.h](https://github.com/ARMmbed/mbed-client-trace/blob/master/mbed-client-trace/mbed_client_trace.h)


## Usage example:
```c++
#define YOTTA_CFG_MBED_CLIENT_TRACE //this can be define also in yotta configuration file: config.yml
#include "mbed_client_trace.h"
#define TRACE_GROUP  "main"

int main(void){
    mbed_client_trace_init();       // initialize trace library
    tr_debug("this is debug msg");  //-> "[DBG ][main]: this is debug msg"
    tr_err("this is error msg");    //-> "[ERR ][main]: this is error msg"
    tr_warn("this is warning msg"); //-> "[WARN][main]: this is warning msg"
    tr_info("this is info msg");    //-> "[INFO][main]: this is info msg"
    char arr[] = {30, 31, 32};
    tr_debug("printing array: %s", mbed_trace_array(arr, 3)); //-> "[DBG ][main]: printing array: 01:02:03"
    return 0;
}
```

## Unit tests

How to run unit tests

* In linux:
```
yotta target x86-linux-native
yotta test mbed_client_trace_test
```

* In linux:
```
yotta target x86-windows-native
yotta test mbed_client_trace_test
```
