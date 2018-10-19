CC = gcc
CFLAGS = -g -w
ROOT = ../

all:: parallelCal vectorMultiply externalCal

parallelCal: 
	$(CC) $(CFLAGS) -pthread -o parallelCal parallelCal.c -L$(ROOT)

vectorMultiply: 
	$(CC) $(CFLAGS) -pthread -o vectorMultiply vectorMultiply.c -L$(ROOT)

externalCal: 
	$(CC) $(CFLAGS) -pthread -o externalCal externalCal.c -L$(ROOT)

clean:
	rm -rf parallelCal vectorMultiply externalCal *.o ./record/
