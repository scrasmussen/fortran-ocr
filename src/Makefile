include Include.mk
CC = gcc
FC = gfortran

OBJS = ocr-interfaces.o ocr-wrapper.o ocr-types.o  wprintf.o printf.o ocr-fortran.o printc.o
LIBS = -L$(OCR_ROOT)/ocr/install/x86/lib -locr
EXES = basic ex2 stencil_1d #hello

CFLAGS = -g $(OCR_INCLUDE)
FFLAGS = $(OCR_INCLUDE)
LDFLAGS = -lpthread

.SUFFIXES: .f90 .F90 .o

.f90.o:
	$(FC) -c $(@:.o=.f90) $(FFLAGS)

.F90.o:
	$(FC) -c $(@:.o=.F90) $(FFLAGS)

.c.o:
	$(CC) -c $(@:.o=.c) $(CFLAGS)

all: types objs ocr-fortran

objs: $(OBJS)
	-@echo "--Compiled--"

types: create-types ocr-types

create-types: create-ocr-types.c
	$(CC) create-ocr-types.c $(OCR_INCLUDE) -o create-ocr-types
	./create-ocr-types

ocr-types: ocr-types.F90
	$(FC) -c ocr-types.F90 

ocr-fortran: ocr-fortran.F90
	$(FC) -o ocr-fortran  $(OBJS) $(LIBS)

clean:
	rm -f *~ *.o *.mod $(EXES) create-ocr-types
