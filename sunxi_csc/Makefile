CC = gcc
CFLAGS = -g -lm
OBJ = csc.o
TARGET = csc

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -vf *.o $(TARGET)