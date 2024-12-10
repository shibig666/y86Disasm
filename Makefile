CC = gcc

.PHONY: all clean

all: y86disassembler hex2bin
	rm -f *.o *.a 

y86disasm.o: src/y86disasm.c
	$(CC) -c src/y86disasm.c -o y86disasm.o -I ./include

y86registor.o: src/y86registor.c
	$(CC) -c src/y86registor.c -o y86registor.o -I ./include

liby86asm: y86disasm.o y86registor.o
	ar rcs liby86asm.a y86disasm.o y86registor.o

y86disassembler.o: src/main.c
	$(CC) -c src/main.c -o y86disassembler.o -I ./include

y86disassembler: liby86asm y86disassembler.o
	$(CC) y86disassembler.o -o y86disassembler -L. -ly86asm

hex2bin: src/hex2bin.c
	$(CC) src/hex2bin.c -o hex2bin

clean:
	rm -f *.o *.a y86disassembler hex2bin
