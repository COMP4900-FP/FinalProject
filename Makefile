#Compiler definitions
#MAKEFLAGS := --jobs=$(shell nproc)
CC = qcc -Vgcc_nto$(PLATFORM)
CXX = q++ -Vgcc_nto$(PLATFORM)_cxx
LD = $(CC)
CFLAGS = -Wall -fmessage-length=0
.PHONY: clean upload rebuild client server
#Macro to expand files recursively: parameters $1 -  directory, $2 - extension, i.e. cpp
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))

INCLUDES = \
	-Isrc \
	-Isrc/clients/include\

all: client server build/generatorFunctions.o
		
		
#Linking
client: build/client

server: build/server

build/client: build/client.o build/humidity.o build/light.o build/soil.o build/temp.o build/generatorFunctions.o
	$(LD) -o build/client build/client.o build/humidity.o build/light.o build/soil.o build/temp.o build/generatorFunctions.o

build/server: build/server.o
	$(LD) -o build/server build/server.o


#Compiling rule
build/client.o: src/client.c src/server.h
	$(CC) -c src/client.c -o build/client.o $(CFLAGS)
	
build/server.o: src/server.c src/server.h
	$(CC) -c src/server.c -o build/server.o $(CFLAGS) $(INCLUDES)

build/humidity.o: src/clients/humidity.c src/server.h src/clients.h
	$(CC) -c src/clients/humidity.c -o build/humidity.o $(CFLAGS) $(INCLUDES)

build/soil.o: src/clients/soil.c src/server.h src/clients.h
	$(CC) -c src/clients/soil.c -o build/soil.o $(CFLAGS) $(INCLUDES)

build/temp.o: src/clients/temp.c src/server.h src/clients.h
	$(CC) -c src/clients/temp.c -o build/temp.o $(CFLAGS) $(INCLUDES)

build/light.o: src/clients/light.c src/server.h src/clients.h
	$(CC) -c src/clients/light.c -o build/light.o $(CFLAGS) $(INCLUDES)

build/generatorFunctions.o: src/generatorFunctions.c src/generatorFunctions.h
	$(CC) -c src/generatorFunctions.c -o build/generatorFunctions.o $(CFLAGS) $(INCLUDES)


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