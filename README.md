# mbed-client-trace

Simple trace library for mbed device

Usage example:
```c++
#define YOTTA_CFG_MBED_CLIENT_TRACE //this can be define also in yotta configuration file: config.yml
#include "mbed_client_trace.h"
#define TRACE_GROUP  "main"

int main(void){
    mbed_client_trace_init();   // initialize trace library
    tr_debug("this is debug msg");  //print debug message to stdout: "[DBG]
    tr_err("this is error msg");
    tr_warn("this is warning msg");
    tr_info("this is info msg");
    return 0;
}
```
