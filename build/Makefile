CC = gcc
AR = ar rcs
IDIR = ../include
SDIR = ../src
TDIR = ../test
CFLAGS := -I$(IDIR) 
SRCS := $(wildcard $(SDIR)/**.c)
TRCS := $(wildcard $(TDIR)/**.c)
OBJECTS := $(SRCS:.c=.o)
TESTS := $(notdir $(TRCS:.c=.out))

PROJECTS := libpthread.a

.PHONY: all
all: $(PROJECTS)

libpthread.a: $(OBJECTS)
	$(AR) $@ $^
	
%.o: %.c
	$(CC) -o $@ $(CFLAGS) $<


.PHONY: test
test: $(TESTS)

%.out: $(TDIR)/%.c
	$(CC) -o $@ $(CFLAGS) $<

