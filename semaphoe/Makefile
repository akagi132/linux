# Makefile

convert_process: convert_process.c libshmem.o libsemaphoe.o
	gcc -g3 -o convert_process libshmem.o libsemaphoe.o convert_process.c

input_process: input_process.c libshmem.o libsemaphoe.o
	gcc -g3 -o input_process libshmem.o libsemaphoe.o input_process.c

libsemaphoe.o: libsemaphoe.c
	gcc -c libsemaphoe.c

libshmem.o: libshmem.c
	gcc -c libshmem.c

clean:
	rm input_process
	rm convert_process
	rm libshmem.o
	rm libsemaphoe.o
