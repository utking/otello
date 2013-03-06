CC 			= gcc
CFLAGS 		= -std=c99 -ggdb `pkg-config --cflags --libs sdl` -lSDL_image -lSDL_ttf
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
