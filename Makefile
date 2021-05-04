CC=g++
CFLAGS=-Wall -s -Ofast -flto -lboost_program_options -pthread
TARGET=fprime

fprime: main.cpp
	$(CC) main.cpp $(CFLAGS) -o $(TARGET)

install:
	cp $(TARGET) /usr/local/bin

clean:
	$(RM) $(TARGET)
