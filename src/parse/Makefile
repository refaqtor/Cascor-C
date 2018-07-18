INSTALL_DIR = ../resources
CC=gcc
MACHDEP_CFLAGS= -ansi -DFreeBSD -g
#	CMU Learning Benchmark Archive
#	Parse Library
#
#	Revisions:
#
#	V2.1
#	Ian Chiu	(ichiu@andrew.cmu.edu)
#	6/10/2018
#
#	Removed depriciated -m486 flag
#	Changed compiler to gcc
#	Changed installation destination from local directory rather than into /usr/ directory
#	remove line './configure: uname returned system type FreeBSD' from file
#
#	v2.0
#	Matt White  (mwhite+@cmu.edu)
#	3/5/95
#
#	QUESTIONS/COMMENTS:  neural-bench@cs.cmu.edu
#
#  This is the makefile for compiling the parse library.  Replace the compiler
#  named in 'CC' with whatever ANSI compatible compiler you are most
#  comfortable with and then type 'make' on the command line'.
#

INC=-I



CFLAGS = $(MACHDEP_CFLAGS) -O0 -I$(INSTALL_DIR)/include

libparse.a:	parse.o
	'rm' -f libparse.a
	ar r libparse.a parse.o
	ranlib libparse.a

parse.o:	parse.c parse.h
	$(CC) $(CFLAGS) -c parse.c

install:	libparse.a parse.h
		'rm' -f $(INSTALL_DIR)/lib/libparse.a
		'rm' -f $(INSTALL_DIR)/include/parse.h
		'cp' libparse.a $(INSTALL_DIR)/lib/libparse.a
		'cp' parse.h $(INSTALL_DIR)/include/parse.h

clean:
	-'rm' -f *.o *.u *~ *.core core
