#Makefile
CC     = gcc
CFLAGS = -g
LFLAGS =
OBJS   = newGame.o
EXEC   = program
LIBS  =
INCS  =

$(EXEC)   :   $(OBJS)
	$(CC) $(LFLAGS) -o $(EXEC) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $(INCS) $*.c

clean :
	rm -f $(EXEC) $(OBJS)
