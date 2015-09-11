# Require C unity -library 
# NOT YET CONFIGURE FOR THIS NEW REPOSITORY...
# 
C_COMPILER=gcc
TARGET_BASE=test_libTrace
TARGET_EXTENSION=.out
TARGET = $(TARGET_BASE)$(TARGET_EXTENSION)
UNIT_TEST_BASE = ../../../../unittest/src

SRC_FILES = $(UNIT_TEST_BASE)/unity.c Test.c Test_Runner.c
SRC_FILES += ../../mbed-client-trace/ns_trace.c ../../libip6string/ip6tos.c ../../libBits/common_functions.c

INC_DIRS = -I$(UNIT_TEST_BASE) -I../../../libService -I../

SYMBOLS=-DTEST -DHAVE_DEBUG -std=gnu99 -pedantic -DUNITY_EXCLUDE_STDINT_H

CLEANUP = rm -f build/*.o ; rm -f $(TARGET)

all: clean default

default:
	ruby $(UNIT_TEST_BASE)/../auto/generate_test_runner.rb Test.c  Test_Runner.c
	$(C_COMPILER) $(INC_DIRS) $(SYMBOLS) $(SRC_FILES) -o $(TARGET)
	./$(TARGET)

clean:
	$(CLEANUP)
