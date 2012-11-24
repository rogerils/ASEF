CC = gcc
CFLAGS = -Wall -pedantic -g -std=c99 `pkg-config opencv --cflags`
LDFLAGS = `pkg-config opencv --libs`
EXEC = camera_main
OBJS = camera_main.o asef.o fd.o utils.o list.o
SRCS = camera_main.c asef.c asef.h fd.c fd.h utils.c utils.h list.c list.h

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXEC)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c camera_main.c asef.c fd.c utils.c list.c

clean:
	rm -f *~
	rm -f *#
	rm -f ./$(EXEC)
	rm -f *.o
