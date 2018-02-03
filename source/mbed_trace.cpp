#include <stdio.h>

#define TRACE_GROUP "TRAC"
#include "../mbed-trace/mbed_trace.h"
#include "../mbed-trace/mbed_trace.hpp"

MbedTrace::MbedTrace() 
{
    this->_trace = mbed_trace_init(NULL);
    // mbed_trace_filter_set(this->_trace, this->trace_filter);
}

int8_t MbedTrace::trace_filter(trace_t *self, int8_t dlevel, const char* group)
{
    return 1;
}

MbedTrace::~MbedTrace() 
{
    mbed_trace_free(this->_trace);
}

bool MbedTrace::color_state()
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    return cfg & TRACE_MODE_COLOR;
}
MbedTrace& MbedTrace::color_on()
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    mbed_trace_config_set(this->_trace, cfg | TRACE_MODE_COLOR);
    return *this;
}
MbedTrace& MbedTrace::color_off()
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    mbed_trace_config_set(this->_trace, cfg & ~TRACE_MODE_COLOR);
    return *this;
}

bool MbedTrace::plain_state()
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    return cfg & TRACE_MODE_PLAIN;
}
MbedTrace& MbedTrace::plain_on()
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    mbed_trace_config_set(this->_trace, cfg | TRACE_MODE_PLAIN);
    return *this;
}
MbedTrace& MbedTrace::plain_off()
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    mbed_trace_config_set(this->_trace, cfg & ~TRACE_MODE_PLAIN);
    return *this;
}

MbedTrace& MbedTrace::level(int level)
{
    uint8_t cfg = mbed_trace_config_get(this->_trace);
    cfg &=~TRACE_MASK_LEVEL;
    cfg |= level == 0 ? TRACE_ACTIVE_LEVEL_NONE :
            level == 1 ? TRACE_ACTIVE_LEVEL_CMD : 
            level == 2 ? TRACE_ACTIVE_LEVEL_ERROR : 
            level == 3 ? TRACE_ACTIVE_LEVEL_WARN : 
            level == 4 ? TRACE_ACTIVE_LEVEL_INFO : 
            level == 5 ? TRACE_ACTIVE_LEVEL_DEBUG : TRACE_ACTIVE_LEVEL_ALL;
    mbed_trace_config_set(this->_trace, cfg);
    return *this;
}

MbedTrace& MbedTrace::operator<<(char const* str)
{
    mbed_tracef(this->_trace, TRACE_LEVEL_DEBUG, TRACE_GROUP, "%s", str);
    return *this;
}
MbedTrace& MbedTrace::operator<<(int value)
{
    mbed_tracef(this->_trace, TRACE_LEVEL_DEBUG, TRACE_GROUP, "%d", value);
    return *this;
}