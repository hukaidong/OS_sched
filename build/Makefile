CC = gcc
AR = ar rcs
IDIR = ../include
SDIR = ../src
TDIR = ../test
CFLAGS := -I$(IDIR) -Wfatal-errors -g -Wall
CSRCS := $(wildcard $(SDIR)/**.c)
CTRCS := $(wildcard $(TDIR)/**.c)
OBJECTS := $(addprefix obj/, $(notdir $(CSRCS:.c=.o)))
TESTS := $(addprefix test/, $(notdir $(CTRCS:.c=.test)))

PROJECTS := libmy_pthread.a

.PHONY: all
all: $(PROJECTS)

.PHONY: test
test: $(TESTS) 

.PHONY: clean
clean: 
	-rm -r obj test *.a
	
.PHONY: obj_dir
obj_dir:
	mkdir -p obj

.PHONY: test_dir
test_dir:
	mkdir -p test

test/%.test: $(TDIR)/%.c test_dir $(PROJECTS)
	$(CC) -o $@ -I$(SDIR) $(CFLAGS) $(OBJECTS) $< -ggdb

libmy_pthread.a: $(OBJECTS)
	$(AR) $@ $^
	
obj/%.o: $(SDIR)/%.c obj_dir
	$(CC) -o $@ $(CFLAGS) $< -c
