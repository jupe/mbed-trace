#include <stdio.h>

#define TRACE_GROUP "test"
#include "mbed-trace/mbed_trace.hpp"

int main(int argc, char *argv[])
{
    MbedTrace tr;
    tr >> stdout;
    tr << "test: " << 10 << "\n";
    tr_debug("Hellou");
    tr_info("Hellou");
    tr.color_off();
    tr_error("Hellou");
    tr.color_on();
    tr_warn("Hellou");
    return 0;
}
