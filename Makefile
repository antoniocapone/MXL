all: test.o mxl.o
	gcc -o test test.o mxl.o

test.o: test.c
	gcc -c test.c

mxl.o: mxl.c mxl.h
	gcc -c mxl.c

clean:
	del /s /q *.o *.exe 2>nul || exit 0