IDIR =../mbed-trace
CC=gcc
CFLAGS=-I$(IDIR) -DYOTTA_CFG_MBED_TRACE_FEA_IPV6=0 -DHAVE_NS_TYPES=0 -DMBED_CONF_MBED_TRACE_ENABLE=1

ODIR=obj
LDIR =../mbed-trace

LIBS=-lm

_DEPS = mbed_trace.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
