CC 			= gcc
CFLAGS 		= -std=c99 -ggdb
OBJS 		= *.o
TARGET 		= otello
CLEAN 		= rm -rf
OBJS 		= main.o otello.o
SOURCES 	= main.c otello.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

clean:
	$(CLEAN) $(TARGET) $(OBJS)
