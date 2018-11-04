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

.PHONY: test
test: $(TESTS) 

.PHONY: clean
clean: 
	-rm -r obj test
	
.PHONY: obj_dir
obj_dir:
	mkdir -p obj

.PHONY: test_dir
test_dir:
	mkdir -p test

test/%.test: $(TDIR)/%.c test_dir
	$(CC) -o $@ -I$(SDIR) $(CFLAGS) $<

libpthread.a: $(OBJECTS)
	$(AR) $@ $^
	
obj/%.o: $(SDIR)/%.c obj_dir
	$(CC) -o $@ $(CFLAGS) $< -c
