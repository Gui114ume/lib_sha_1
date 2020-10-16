CC=gcc
CFLAGS=-O0
LDFLAGS1=-lm
LDFLAGS2=-lm -L./ -lsha1
EXEC=

all: libsha1.so mysha1

libsha1.so: libsha.c
	$(CC) -shared -fPIC $^ -o $@ $(CFLAGS) $(LDFLAGS1)

mysha1.o: main.c
	$(CC) -c $^ -o $@ $(CFLAGS)

mysha1 : mysha1.o libsha1.so
	$(CC) $^ -o $@ $(LDFLAGS2)

clean:
	 rm -rf libsha1.so *.o mysha1
