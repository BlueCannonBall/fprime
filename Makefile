CC=g++
CFLAGS=-Wall -s -Ofast -lboost_program_options -pthread -fopenmp-simd -march=native -ftree-vectorize -fvect-cost-model=dynamic
TARGET=fprime

$(TARGET): main.cpp
	$(CC) main.cpp $(CFLAGS) -o $@

.PHONY: install clean

install:
	cp $(TARGET) /usr/local/bin

clean:
	$(RM) $(TARGET)
