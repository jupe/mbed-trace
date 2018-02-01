#include <stdio.h>

#define TRACE_GROUP "test"
#include "../mbed-trace/mbed_trace.h"


int main(int argc, char* argv[]) {
    printf("test\n\n");
    mbed_trace_init(0);
    tr_debug("Hellou");
    printf("\ntest\n");
    return 0;
}
