
# prompt> make
# builds everything and links in test program test_rb
#

OBJS = main.o hash.o zipf.o bmap.o tracer.o checker.o

CC = gcc

CFLAGS = -g -Wall 
#-pedantic

PROGRAM = main


all: $(PROGRAM)

$(PROGRAM): 	$(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(PROGRAM) $(DMALLOC_LIB)


main.o: main.c tracer.h instrumentor.h checker.h 

hash.o: hash.c hash.h list.h

zipf.o: zipf.c zipf.h

bmap.o: bmap.c bmap.h

tracer.o: tracer.c tracer.h

checker.o: checker.c checker.h instrumentor.h list.h


clean:			
	rm -f *.o *~ $(PROGRAM)






