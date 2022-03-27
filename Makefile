#Compiler definitions

CC = qcc -Vgcc_nto$(PLATFORM)
CXX = q++ -Vgcc_nto$(PLATFORM)_cxx
LD = $(CC)


#Macro to expand files recursively: parameters $1 -  directory, $2 - extension, i.e. cpp
rwildcard = $(wildcard $(addprefix $1/*.,$2)) $(foreach d,$(wildcard $1/*),$(call rwildcard,$d,$2))


all: build/client.o build/server.o
		$(LD) -o build/client build/client.o
		$(LD) -o build/server build/server.o
		

#Compiling rule
build/client.o: src/client.c src/server.h
	$(CC) -c src/client.c -o build/client.o -Wall -fmessage-length=0
	
build/server.o: src/server.c src/server.h
	$(CC) -c src/server.c -o build/server.o -Wall -fmessage-length=0


clean:
	rm -fr build

rebuild: clean all

#Inclusion of dependencies (object files to source and includes)
-include $(OBJS:%.o=%.d)