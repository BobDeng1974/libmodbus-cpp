OJB:=$(patsubst %.c,   %.o, $(wildcard *.c))
OJB+=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
ARCH:=powerpc-e300c3-linux-gnu-
CC:=$(ARCH)g++
CFLAGS:= -I.. -I.

test.exe:$(OJB) test.c++
	$(CC) $+ -o $@
clean:
	@rm -rfv *.o *.exe
.c.o:
	$(CC) $(CFLAGS) -c $+ -o $@
.cpp.o:
	$(CC) $(CFLAGS) -c $+ -o $@
