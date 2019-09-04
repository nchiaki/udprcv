CC=gcc
LIB=
#LIBDIR=-lpthread -Isypcieif -L/usr/local/lib
LIBDIR=-lpthread -L/usr/local/lib
INCDIR=-I/usr/local/include
CFLAGS=-ggdb -Wall  $(DBGFLG)
#CFLAGS=-O3 -Wall  $(DBGFLG)
#LDFLAGS=-Xlinker --cref
TARGETS=udprcv

SRCS=main.c gloval.c initproc.c cmdproc.c recvproc.c

OBJS=$(SRCS:.c=.o)

all:	$(TARGETS) $(OBJS)

$(TARGETS):	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(INCDIR) $(LIBDIR) $(LIB)

.c.o:
	$(CC) $(CFLAGS) -c $< $(INCDIR) $(LIBDIR) $(LIB)

.PHONY:	clean
clean:
	$(RM) *~ $(TARGETS) $(OBJS)

DBGFLG=
