CC = gcc
CFLAGS = -g
OBJ = tem.o
TARGET = tem
OBJ2 = tem2.o
TARGET2 = tem2

all: $(TARGET) $(TARGET2)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(TARGET2): $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -vf *.o $(TARGET)
	rm -vf *.o $(TARGET2)
