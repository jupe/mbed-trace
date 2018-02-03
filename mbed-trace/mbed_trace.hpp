#ifndef MBED_TRACE_HPP_
#define MBED_TRACE_HPP_

#include "../mbed-trace/mbed_trace.h"

class MbedTrace {
    trace_t* _trace;
    private:
        int8_t trace_filter(trace_t*, int8_t, const char*);

    public:
        MbedTrace();
        ~MbedTrace();
        bool color_state();
        MbedTrace&  color_on();
        MbedTrace&  color_off();
        bool  plain_state();
        MbedTrace&  plain_on();
        MbedTrace&  plain_off();
        MbedTrace&  level(int);
        MbedTrace& operator<<(const char*);
        MbedTrace& operator<<(const int);
        MbedTrace& operator<<(const unsigned int);
};

#endif //MBED_TRACE_HPP_
