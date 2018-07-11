
#  	C Function Toolkit
#  	Makefile
#
#	Revisions:
#
#	V1.1
#	Ian Chiu  (ichiu@andrew.cmu.edu)
#	6/10/2018
#
#	Removed depriciated -m486 flag
#	Changed compiler to gcc
#	Changed installation destination from local directory rather than into /usr/ directory
#	remove line './configure: uname returned system type FreeBSD' from file
#
#  	V1.0
#	Matt White (mwhite+@cmu.edu)
#	10/2/94
#
#
#

INSTALL_DIR = ../resources
CC=gcc
MACHDEP_CFLAGS= -ansi -DFreeBSD -g

CFLAGS= $(MACHDEP_CFLAGS) -O0
OBJS= memory.o prompt.o string.o str_cvrt.o queue.o vector.o vectori.o

libtoolkit.a:	$(OBJS)
	        rm -f libtoolkit.a
		ar r libtoolkit.a $(OBJS)
		ranlib libtoolkit.a

memory.o:	memory.c toolkit.h
prompt.o:       prompt.c toolkit.h
string.o:	string.c memory.c toolkit.h
str_cvrt.o:     str_cvrt.c toolkit.h

queue.o:	queue.c queue.h toolkit.h
vector.o:	vector.c vector.h
vectori.o:	vectori.c vector.h

install:	libtoolkit.a toolkit.h vector.h queue.h
		'cp' libtoolkit.a $(INSTALL_DIR)/lib/libtoolkit.a
		'cp' toolkit.h $(INSTALL_DIR)/include/toolkit.h
		'cp' vector.h $(INSTALL_DIR)/include/vector.h
		'cp' queue.h $(INSTALL_DIR)/include/queue.h

clean:
		rm -f *.o *.u *~ core
