CC = gcc
CFLAGS = -g -w

all:: parallelCal vectorMultiply externalCal

parallelCal: 
	$(CC) $(CFLAGS) -pthread -o parallelCal parallelCal.c -L../ 

vectorMultiply: 
	$(CC) $(CFLAGS) -pthread -o vectorMultiply vectorMultiply.c -L../ 

externalCal: 
	$(CC) $(CFLAGS) -pthread -o externalCal externalCal.c -L../

clean:
	\rm -rf parallelCal vectorMultiply externalCal *.o ./record/
