include ../../src/Include.mk

CC = gcc
FC = gfortran
OBJS = ../ocr-wrapper.o ../ocr-types.o ../ocr-interfaces.o ../wprintf.o ../printf.o
LIBS = -L$(OCR_ROOT)/ocr/install/x86/lib -locr
EXES = task_chain loop fork_join

CFLAGS = -g ${OCR_INCLUDE}
FFLAGS = ${OCR_INCLUDE}
LDFLAGS = -lpthread

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) ${CFLAGS}

all: ${EXES}

task_chain: task_chain.F90 task_chain.o
	$(FC) -o task_chain task_chain.o ${FFLAGS} ${LDFLAGS} ${OBJS} ${LIBS}

loop: loop.F90 loop.o
	$(FC) -o loop loop.o ${FFLAGS} ${LDFLAGS} ${OBJS} ${LIBS}

fork_join: fork_join.F90 fork_join.o
	$(FC) -o fork_join fork_join.o ${FFLAGS} ${LDFLAGS} ${OBJS} ${LIBS}

check: fork_join
	@echo "---------------"
	@fork_join

clean:
	rm -f *~ *.o *.mod ${EXES}
