CC=gcc
FC=gfortran
INCLUDE=$(HOME)/xstack/ocr/install/x86/include/
LIBOCR=$(HOME)/xstack/ocr/build/x86/libocr.a
OBJS = cocr.o stencil1d.o daveStencil1d.o # serialStencil.o
CFLAGS = -lpthread -v -g
FFlags = 

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) -I$(INCLUDE) ${CFLAGS}

all: stencil

stencil: $(OBJS) stencil1d.c cocr.c objs
	$(CC) -o stencil1d stencil1d.o cocr.o ${LIBOCR} ${CFLAGS}
	echo '--Running--'
	./stencil1d

daveStencil: $(OBJS) daveStencil1d.c objs
	$(CC) -o daveStencil1d daveStencil1d.o ${LIBOCR} ${CFLAGS}
	echo '--Running--'
	./daveStencil1d

objs: $(OBJS)
	-@echo "--Compiled--"

clean:
	rm -f *~ *.o stencil1d daveStencil1d test
