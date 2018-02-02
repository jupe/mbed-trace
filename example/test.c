#include <stdio.h>

#define TRACE_GROUP "test"
#include "../mbed-trace/mbed_trace.h"


int main(int argc, char* argv[]) {
    mbed_trace_init(NULL);
    tr_debug("Hellou");
    tr_info("Hellou");
    tr_info("Hellou");
    tr_warn("Hellou");
    return 0;
}
