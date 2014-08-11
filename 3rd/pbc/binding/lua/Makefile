.PHONY : all clean

all :
	gcc -g -O2 -Wall -fPIC -c -I../.. -I/usr/local/include pbc-lua.c
	gcc -g -shared -o protobuf.so -L/usr/local/lib -L../../build pbc-lua.o -lpbc -llua

clean :
	rm protobuf.so

