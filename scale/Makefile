CC = gcc
CFLAGS = -g
OBJ = scale.o
TARGET = scale

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -vf *.o $(TARGET)