OBJECTS = memory.o stack.o keyboard.o screen.c

all: ${OBJECTS}
	gcc main.c ${OBJECTS} -o chip8 -lSDL2

memory.o:memory.c
	gcc ${INCLUDES} memory.c -c -o memory.o

stack.o:stack.c
	gcc ${INCLUDES} stack.c -c -o stack.o

keyboard.o:keyboard.c
	gcc ${INCLUDES} keyboard.c -c -o keyboard.o

screen.o:screen.c
	gcc ${INCLUDES} screen.c -c -o screen.o