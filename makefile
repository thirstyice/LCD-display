main : screen.o buffer.o

screen.o : buffer.o

buffer.o :

debug : CXXFLAGS += -D DEBUG
debug : main

clean :
	rm main *.o
