CC=g++
CFLAGS=-c -ggdb -I.. -Wfatal-errors -DMBED_CONF_MBED_TRACE_FEA_IPV6=0 -DHAVE_NS_TYPES=0 -DMBED_CONF_MBED_TRACE_ENABLE=1
LDFLAGS=
SOURCES=log_stdout.c ../source/mbed_trace.c 
#SOURCES=log_file.c ../source/mbed_trace.c 
#SOURCES=log_class.cpp ../source/mbed_trace.c ../source/mbed_trace.cpp
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=test

.PHONY: all

#all: stdout file class
all: $(SOURCES) $(EXECUTABLE)

clean:
	rm $(OBJECTS)
    
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
