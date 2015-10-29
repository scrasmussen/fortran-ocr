CC=gcc
FC=gfortran
INCLUDE=$(HOME)/xstack/ocr/install/x86/include/
OBJS = ocr-wrapper.o ocr-types.o ocr-interfaces.o printf.o test-ocr-types.o 
CFLAGS = -g -std=c99
FFlags = -g

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) -I$(INCLUDE)  -g ${CFLAGS}

all: ex2

basic: basic.F90 basic.o objs # $(OBJS)
	$(CC) -o basic basic.o printf.o ../../build/x86/libocr.a -lpthread ${FFLAGS}

ex1: ex1.F90 ex1.o objs # $(OBJS)
	$(CC) -o ex1 ex1.o ocr-wrapper.o printf.o ../../build/x86/libocr.a -lpthread -v ${CFLAGS}

ex2: ex2.F90 ex2.o objs # $(OBJS)
	$(FC) -o ex2 ex2.o ocr-wrapper.o printf.o ../../build/x86/libocr.a -lpthread -v ${FFLAGS} -g

objs: $(OBJS)
	-@echo "--Compiled--"

type: create-type type-mod

type-mod: ocr-types.F90
	$(FC) -c ocr-types.F90

create-type: create-ocr-type-size.c
	$(CC) create-ocr-type-size.c -I$(INCLUDE) -o create-ocr-type-size
	./create-ocr-type-size

test-type: create-ocr-types.c
	$(CC) create-ocr-types.c -I$(INCLUDE) -o create-ocr-types
	./create-ocr-types


clean:
	rm -f *~ *.o basic *.mod ex1 ex2 create-ocr-type-size
