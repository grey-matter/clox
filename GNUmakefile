CC = gcc
OBJECTS = main.o chunk.o memory.o debug.o value.o vm.o scanner.o compiler.o

main: $(OBJECTS)
	$(CC) -o main $(OBJECTS)


vm.o: vm.c vm.h compiler.h
	$(CC) -c vm.c
value.o: value.c value.h memory.h
	$(CC) -c value.c
debug.o: debug.c debug.h
	$(CC) -c debug.c
main.o: main.c chunk.h common.h debug.h
	$(CC) -c main.c
chunk.o: chunk.c chunk.h memory.h value.h
	$(CC) -c chunk.c
memory.o: memory.c common.h memory.h
	$(CC) -c memory.c
compiler.o: compiler.c compiler.h scanner.h
	$(CC) -c compiler.c
scanner.o: scanner.c scanner.h
	$(CC) -c scanner.c


clean:
	rm $(OBJECTS)
