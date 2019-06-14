CFLAGS = -Wall -g
CC = gcc $(CFLAGS)

all : bl_server bl_client

bl_server : bl_server.o server_funcs.o simpio.o util.o blather.h
	$(CC) -o bl_server bl_server.o server_funcs.o simpio.o util.o

bl_server.o : bl_server.c blather.h
	$(CC) -c bl_server.c

server_funcs.o : server_funcs.c blather.h
	$(CC) -c server_funcs.c

util.o : util.c blather.h
	$(CC) -c util.c

simpio.o : simpio.c blather.h
	$(CC) -c simpio.c

bl_client : bl_client.o simpio.o util.o blather.h
	$(CC) -o bl_client bl_client.o simpio.o util.o -lpthread

bl_client.o : bl_client.c blather.h
	$(CC) -c bl_client.c

test : test_blather.sh test_blather_data.sh cat_sig.sh clean-tests
	chmod u+rx test_blather.sh test_blather_data.sh cat_sig.sh normalize.awk filter-semopen-bug.awk
	./test_blather.sh

clean :
	rm -f *.o

clean-tests :
	rm -f test-*.{log,out,expect,diff,valgrindout}

real-clean :
	rm -f *.o *.fifo bl_server bl_client
