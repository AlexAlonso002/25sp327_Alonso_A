CC=gcc
LINKER=gcc
CFLAGS=-Wall -Wextra -O0
LDFLAGS=-g
LIBS=-lpthread
OBJS=worker_thread.o  main.o queue.o tasks.o
TARGETS=  test_queue request_server 
#worker_thread_pool.o 
all: $(TARGETS)

worker_thread_pool: $(OBJS) 
	$(LINKER) $(LINKER_FLAGS) $(OBJS) $(LIBS) -o worker_thread_pool

request_server: $(OBJS)
	$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o request_server


# TODO: Add remaining dependencies
main.o: main.c common.h queue.h tasks.h thread_pool.h worker_thread.h
	$(CC) $(CFLAGS) -c main.c -o main.o

thread_pool.o: thread_pool.c thread_pool.h queue.h worker_thread.h common.h
	$(CC) $(CFLAGS) -c thread_pool.c -o thread_pool.o

tasks.o: tasks.c tasks.h
	$(CC) $(CFLAGS) -c tasks.c -o tasks.o

worker_thread.o: worker_thread.c worker_thread.h queue.h common.h
	$(CC) $(CFLAGS) -c worker_thread.c -o worker_thread.o

queue.o: queue.c queue_internal.h queue.h common.h
	$(CC) $(CFLAGS) -c queue.c -o queue.o

test_queue.o: test_queue.c queue_internal.h queue.h
	$(CC) $(CFLAGS) -c test_queue.c -o test_queue.o

test_queue: test_queue.o queue.o
	$(LINKER) $(LDFLAGS) test_queue.o queue.o -o test_queue -lcmocka -Wl,--wrap=pthread_mutex_lock,--wrap=pthread_mutex_unlock

clean:
	rm -fr *.o $(OBJS) $(TARGETS)