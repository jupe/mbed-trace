#include <stdio.h>

#define TRACE_GROUP "test"
#include "../mbed-trace/mbed_trace.h"

int main(int argc, char* argv[]) {
    trace_t* trace = mbed_trace_init(0);
    uint8_t cfg = TRACE_ACTIVE_LEVEL_ALL | TRACE_CARRIAGE_RETURN;
    mbed_trace_config_set(trace, cfg);
    FILE *fh = fopen("./debug.log", "a");
    mbed_trace_set_pipe(trace, fh);
    tr_debug("Hellou");
    tr_info("Hellou");
    tr_info("Hellou");
    tr_warn("Hellou");
    fclose(fh);
    return 0;
}
