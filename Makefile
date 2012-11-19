CC = gcc
CFLAGS = -Wall -g -pedantic -std=c99 `pkg-config opencv --cflags`
LDFLAGS = `pkg-config opencv --libs`
EXEC = asef_camera
OBJS = asef_camera.o asef.o fd.o utils.o 
SRCS = asef_camera.c asef.c asef.h fd.c fd.h utils.c utils.h

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(EXEC)

$(OBJS): $(SRCS)
	$(CC) $(CFLAGS) -c asef_camera.c asef.c fd.c utils.c

clean:
	rm -f *~
	rm -f *#
	rm -f ./$(EXEC)
	rm -f *.o
