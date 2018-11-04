CC = gcc
AR = ar rcs
IDIR = ../include
SDIR = ../src
TDIR = ../test
CFLAGS := -I$(IDIR) -Wfatal-errors -g -Wall
SRCS := $(wildcard $(SDIR)/**.c)
TRCS := $(wildcard $(TDIR)/**.c)
OBJECTS := $(addprefix obj/, $(notdir $(SRCS:.c=.o)))
TESTS := $(addprefix test/, $(notdir $(TRCS:.c=.test)))

PROJECTS := libpthread.a

.PHONY: all

all: $(PROJECTS)

libpthread.a: $(OBJECTS)
	$(AR) $@ $^
	
obj/%.o: $(SDIR)/%.c obj_dir
	$(CC) -o $@ $(CFLAGS) $< -c
	
.PHONY: obj_dir
obj_dir:
	mkdir -p obj

.PHONY: test
test: $(TESTS) test_dir
	mkdir -p test

.PHONY: test_dir
test_dir:
	mkdir -p test

%.test: $(TDIR)/%.c
	$(CC) -o $@ $(CFLAGS) $<


.PHONY: clean
clean: 
	-rm -r obj test
