CC = gcc
CFLAGS = -g
OBJ = blit.o
TARGET = blit
LIBS = -lpixman-1

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -vf *.o $(TARGET)