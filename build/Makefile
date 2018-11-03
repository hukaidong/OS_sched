CC = gcc
AR = ar rcs
IDIR = ../include
SDIR = ../src
CFLAGS := -I$(IDIR) 
SRCS := $(wildcard $(SDIR)/**.c)
OBJECTS := $(SRCS:.c=.o)

PROJECTS := libpthread.a

.PHONY: all
all: $(PROJECTS)

libpthread.a: $(OBJECTS)
	$(AR) $@ $^
	
%.o: %.c
	$(CC) -o $@ $(CFLAGS) $<




