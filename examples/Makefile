OCR_ROOT = $(HOME)/D-TEC/ocr-xstack
OCR_CONFIG = $(OCR_ROOT)/ocr/install/x86/config/default.cfg

CC = gcc
FC = gfortran

INCLUDES = -I$(OCR_ROOT)/ocr/install/x86/include/
OBJS = ocr-wrapper.o ocr-types.o ocr-interfaces.o wprintf.o printf.o
LIBS = -L$(OCR_ROOT)/ocr/install/x86/lib -locr
EXES = hello basic ex2 stencil_1d

CFLAGS = -g ${INCLUDES}
FFLAGS = ${INCLUDES}
LDFLAGS = -lpthread

.SUFFIXES: .f90 .F90 .o

.f90.o:
	$(FC) -c $(@:.o=.f90) ${FFLAGS}

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) ${CFLAGS}

all: hello

basic: $(OBJS) basic.F90 basic.o objs
	$(FC) -o basic basic.o $(FFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
	@echo '--Running--'
	@./basic

hello: $(OBJS) hello.F90 hello.o objs
	$(FC) -o hello hello.o $(FFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
	@echo '--Running--'
	@./hello

ex2: $(OBJS) ex2.F90 ex2.o objs
	$(FC) -o ex2 ex2.o $(FFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
	echo '--Running--'
	./ex2

stencil: $(OBJS) stencil_1d.F90 stencil_1d.o objs
	$(FC) -o stencil_1d stencil_1d.o $(FFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)
	echo '--Running--'
	./stencil_1d

objs: $(OBJS)
	-@echo "--Compiled--"

types: create-types type-mod

create-types: create-ocr-types.c
	$(CC) create-ocr-types.c -I$(INCLUDE) -o create-ocr-types
	cp ocr-types.F90 ocr-types.F90.orig
	./create-ocr-types

type-mod: ocr-types.F90
	$(FC) -c ocr-types.F90

check: hello
	@echo "---------------"
	@hello

clean:
	rm -f *~ *.o *.mod ${EXES} create-ocr-types
