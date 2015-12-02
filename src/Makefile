include Include.mk
CC = gcc
FC = gfortran

OBJS = ocr-wrapper.o ocr-types.o ocr-interfaces.o wprintf.o printf.o
LIBS = -L$(OCR_ROOT)/ocr/install/x86/lib -locr
EXES = basic ex2 stencil_1d #hello

CFLAGS = -g ${OCR_INCLUDE}
FFLAGS = ${OCR_INCLUDE}
LDFLAGS = -lpthread

.SUFFIXES: .f90 .F90 .o

.f90.o:
	$(FC) -c $(@:.o=.f90) ${FFLAGS}

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) ${CFLAGS}

all: objs types

objs: $(OBJS)
	-@echo "--Compiled--"

types: create-types type-mod

create-types: create-ocr-types.c
	$(CC) create-ocr-types.c $(OCR_INCLUDE) -o create-ocr-types
	./create-ocr-types

type-mod: ocr-types.F90
	$(FC) -c ocr-types.F90 

clean:
	rm -f *~ *.o *.mod ${EXES} create-ocr-types
