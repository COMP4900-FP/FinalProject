#Compiler definitions
MAKEFLAGS := --jobs=$(shell nproc)
CC = qcc -Vgcc_nto$(PLATFORM)
CXX = q++ -Vgcc_nto$(PLATFORM)_cxx
LD = $(CC)
CFLAGS = -Wall -fmessage-length=0
.PHONY: clean upload rebuild client server
#Macro to expand files recursively: parameters $1 -  directory, $2 - extension, i.e. cpp
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))


all: client server
		
		
#Linking
client: build/client

server: build/server

build/client: build/client.o build/humidity.o build/light.o build/soil.o build/temp.o
	$(LD) -o build/client build/client.o build/humidity.o build/light.o build/soil.o build/temp.o

build/server: build/server.o
	$(LD) -o build/server build/server.o

#Compiling rule
build/client.o: src/client.c src/server.h
	$(CC) -c src/client.c -o build/client.o $(CFLAGS)
	
build/server.o: src/server.c src/server.h
	$(CC) -c src/server.c -o build/server.o $(CFLAGS)

build/humidity.o: src/clients/humidity.c
	$(CC) -c src/clients/humidity.c -o build/humidity.o $(CFLAGS)

build/soil.o: src/clients/soil.c
	$(CC) -c src/clients/soil.c -o build/soil.o $(CFLAGS)

build/temp.o: src/clients/temp.c
	$(CC) -c src/clients/temp.c -o build/temp.o $(CFLAGS)

build/light.o: src/clients/light.c
	$(CC) -c src/clients/light.c -o build/light.o $(CFLAGS)


upload: server client 
	scp build/{client,server} root@$(VMIP):/tmp
clean:
	rm -fr build/*.o

rebuild: clean all

#Inclusion of dependencies (object files to source and includes)
-include $(OBJS:%.o=%.d)

#Testing stuff, ignore
build/watch.o: src/watch.c 
	$(CC) -c src/watch.c -o build/watch.o $(CFLAGS)

uploadw:  watch
	scp build/watch root@$(VMIP):/tmp
	
watch: build/watch

build/watch: build/watch.o
	$(LD) -o build/watch build/watch.o