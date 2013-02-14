CC = gcc 
CFLAGS = -g -std=gnu99 `pkg-config opencv --cflags`
LDFLAGS = `pkg-config opencv --libs` 
EXEC = camera_main
OBJS = camera_main.o asef.o fd.o utils.o 
SRCS = camera_main.c asef.c asef.h fd.c fd.h utils.c utils.h

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) $(LDFLAGS) 

$(OBJS): $(SRCS)
	$(CC) -c camera_main.c asef.c fd.c utils.c $(CFLAGS)

clean:
	rm -f *~
	rm -f *#
	rm -f ./$(EXEC)
	rm -f *.o
