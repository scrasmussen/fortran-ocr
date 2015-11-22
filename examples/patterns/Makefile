OCR_ROOT = $(HOME)/D-TEC/ocr-xstack
OCR_CONFIG = $(OCR_ROOT)/ocr/install/x86/config/default.cfg

CC = gcc
FC = gfortran

INCLUDES = -I$(OCR_ROOT)/ocr/install/x86/include -I..
OBJS = ../ocr-wrapper.o ../ocr-types.o ../ocr-interfaces.o ../wprintf.o ../printf.o
LIBS = -L$(OCR_ROOT)/ocr/install/x86/lib -locr
EXES = task_chain

CFLAGS = -g ${INCLUDES}
FFLAGS = ${INCLUDES}
LDFLAGS = -lpthread

.SUFFIXES: .F90 .o

.F90.o:
	$(FC) -c $(@:.o=.F90) ${FFLAGS}

.c.o:
	$(CC) -c $(@:.o=.c) ${CFLAGS}

all: ${EXES}

task_chain: task_chain.F90 task_chain.o
	$(FC) -o task_chain task_chain.o ${FFLAGS} ${LDFLAGS} ${OBJS} ${LIBS}

check: task_chain
	@echo "---------------"
	@task_chain

clean:
	rm -f *~ *.o *.mod ${EXES}
