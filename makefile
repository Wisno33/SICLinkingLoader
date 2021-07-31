# Compiler
CC = gcc-10

# Compiler flags:
# -g debug
# -Wall warnings
# -O3 level 3 optimization
# Optimizations do not work on this version of MACOS
CFLAGS = -Wall -O3

# target executable
TARGET = SICLinkingLoader

# Included header directories 
INCLUDE = -Ilib -ISIC

# All object files needed for target.
OBJS =  main.o Functions.o Hash_Table.o Queue.o  SIC_components.o pass1.o pass2.o
OBJSPATH = $(addprefix obj/, $(OBJS))

default: $(TARGET)

all: $(TARGET)

$(TARGET): obj $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -O3 $(OBJSPATH) -o $(TARGET)

obj: obj
	mkdir obj

main.o: main.c lib/Functions.h lib/Hash_Table.h lib/Queue.h SIC/SIC_components.h SIC/pass1.h SIC/pass2.h
	$(CC) $(CFLAGS) $(INCLUDE) -c main.c -o obj/main.o

pass2.o: SIC/pass2.c SIC/pass2.h lib/Functions.h lib/Hash_Table.h lib/Queue.h SIC/SIC_components.h
	$(CC) $(CFLAGS) $(INCLUDE) -c SIC/pass2.c -o obj/pass2.o

pass1.o: SIC/pass1.c SIC/pass1.h lib/Functions.h lib/Hash_Table.h lib/Queue.h SIC/SIC_components.h
	$(CC) $(CFLAGS) $(INCLUDE) -c SIC/pass1.c -o obj/pass1.o

SIC_components.o: SIC/SIC_components.c SIC/SIC_components.h
	$(CC) $(CFLAGS) $(INCLUDE) -c SIC/SIC_components.c -o obj/SIC_components.o

Functions.o: lib/Functions.c lib/Functions.h
	$(CC) $(CFLAGS) $(INCLUDE) -c lib/Functions.c -o obj/Functions.o

Hash_Table.o: lib/Hash_Table.c lib/Hash_Table.h
	$(CC) $(CFLAGS) $(INCLUDE) -c lib/Hash_Table.c -o obj/Hash_Table.o

Queue.o: lib/Queue.c lib/Queue.h
	$(CC) $(CFLAGS) $(INCLUDE) -c lib/Queue.c -o obj/Queue.o

clean:
	rm SICLinkingLoader
	rm -r obj