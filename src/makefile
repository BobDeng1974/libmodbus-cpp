OJB:=$(patsubst %.c,   %.o, $(wildcard *.c))
OJB+=$(patsubst %.cpp, %.o, $(wildcard *.cpp))
ARCH:=powerpc-e300c3-linux-gnu-
CC:=$(ARCH)g++
STRIP:=$(ARCH)strip
CFLAGS:= -I.. -I.

all: modbus_server.exe modbus_client.exe
	@$(STRIP) $+
	@ls -sh1 $+
modbus_client.exe:$(OJB) test-client.c++
	@$(CC) $+ -o $@
modbus_server.exe:$(OJB) test-server.c++
	@$(CC) $+ -o $@
clean:
	@rm -rfv *.o *.exe
.c.o:
	$(CC) $(CFLAGS) -c $+ -o $@
.cpp.o:
	$(CC) $(CFLAGS) -c $+ -o $@
