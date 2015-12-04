include ../../src/Include.mk
SRC=../../src

CC = gcc
FC = gfortran
LIBS = -L$(OCR_ROOT)/ocr/install/x86/lib -locr $(SRC)/ocrFortranLib.a
EXES = fork_join # task_chain loop

CFLAGS = -g $(OCR_INCLUDE)
FFLAGS = $(OCR_INCLUDE) -I$(SRC)
LDFLAGS = -lpthread

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(@:.o=.F90) $(FFLAGS)

.c.o:
	$(CC) -c $(@:.o=.c) $(CFLAGS)

all: clean $(EXES)

task_chain: task_chain.F90 task_chain.o $(OBJS)
	$(FC) -o task_chain task_chain.o $(FFLAGS) $(LDFLAGS) $(LIBS)

loop: loop.F90 loop.o
	$(FC) -o loop loop.o $(FFLAGS) $(LDFLAGS) $(LIBS)

fork_join: fork_join.F90 fork_join.o
	$(FC) -o fork_join fork_join.o $(FFLAGS) $(LDFLAGS) $(LIBS)

check: fork_join
	@echo "---------------"
	@fork_join

clean:
	rm -f *~ *.o *.mod $(EXES)
