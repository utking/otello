CC 			= gcc
CFLAGS 		= -std=c99 -ggdb `pkg-config --cflags --libs sdl` -DNO_STDIO_REDIRECT -lSDL_image -Wall -Werror
OBJS 		= *.o
TARGET 		= otello
CLEAN 		= rm -rf
OBJS 		= main.o otello.o
SOURCES 	= main.c otello.c

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
	strip $(TARGET)

clean:
	$(CLEAN) $(OBJS)
