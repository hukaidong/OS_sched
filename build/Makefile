CC = gcc
AR = ar rcs
IDIR = ../include
SDIR = ../src
TDIR = ../test
CFLAGS := -I$(IDIR) -Wfatal-errors -g -Wall
SRCS := $(wildcard $(SDIR)/**.c)
TRCS := $(wildcard $(TDIR)/**.c)
OBJECTS := $(notdir $(SRCS:.c=.o))
TESTS := $(notdir $(TRCS:.c=.test))

PROJECTS := libpthread.a

.PHONY: all
all: $(PROJECTS)

libpthread.a: $(OBJECTS)
	$(AR) $@ $^
	
%.o: $(SDIR)/%.c
	$(CC) -o $@ $(CFLAGS) $< -c


.PHONY: test
test: $(TESTS)

%.test: $(TDIR)/%.c
	$(CC) -o $@ $(CFLAGS) $<

