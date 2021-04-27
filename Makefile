CC=g++
CFLAGS=-Wall -s -Ofast -march=native -mtune=native -fno-signed-zeros -fno-trapping-math -frename-registers -funroll-loops -flto -D_GLIBCXX_PARALLEL -lboost_program_options -pthread -fopenmp
TARGET=fprime

fprime: main.cpp
	$(CC) main.cpp $(CFLAGS) -o $(TARGET)

install:
	cp $(TARGET) /usr/local/bin

clean:
	$(RM) $(TARGET)
