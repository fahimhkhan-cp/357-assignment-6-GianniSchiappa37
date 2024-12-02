#set the compiler
CC = gcc

#set the output file name
OUTPUT = main.out

#compile main.c to create main.out
all: $(OUTPUT)

$(OUTPUT): main.c
	$(CC) main.c -o $(OUTPUT)


#clean 
clean:
	rm -f $(OUTPUT)	