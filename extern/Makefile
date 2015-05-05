CC = gcc
CFLAGS = -g
LIBS = -lpthread
OBJ = thread.o main.o
TARGET = thread

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -vf *.o $(TARGET)