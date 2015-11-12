CC=gcc
FC=gfortran
INCLUDE=$(HOME)/xstack/ocr/install/x86/include/
OBJS = ocr-wrapper.o ocr-types.o ocr-interfaces.o printf.o
CFLAGS = 
FFlags = 

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) -I$(INCLUDE)  -g ${CFLAGS}

all: stencil

basic: $(OBJS) basic.F90 basic.o objs
	$(CC) -o basic basic.o printf.o ../../build/x86/libocr.a -lpthread ${FFLAGS}

ex1: $(OBJS) ex1.F90 ex1.o objs
	$(CC) -o ex1 ex1.o ocr-wrapper.o printf.o ../../build/x86/libocr.a -lpthread -v ${CFLAGS}

ex2: $(OBJS) ex2.F90 ex2.o objs
	$(CC) -o ex2 ex2.o ocr-wrapper.o printf.o ../../build/x86/libocr.a -lpthread -v ${FFLAGS} -g
	echo '--Running--'
	./ex2

stencil: $(OBJS) stencil_1d.F90 stencil_1d.o objs
	$(CC) -o stencil_1d stencil_1d.o ocr-wrapper.o printf.o ../../build/x86/libocr.a -lpthread -v ${FFLAGS} -g
	echo '--Running--'
	./stencil_1d

objs: $(OBJS)
	-@echo "--Compiled--"

types: create-types type-mod

create-types: create-ocr-types.c
	$(CC) create-ocr-types.c -I$(INCLUDE) -o create-ocr-types
	./create-ocr-types

type-mod: ocr-types.F90
	$(FC) -c ocr-types.F90

clean:
	rm -f *~ *.o basic *.mod ex1 ex2 create-ocr-type-size stencil_1d
