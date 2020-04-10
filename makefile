main : buffer.o

buffer.o : buffer.cpp buffer.h

debug : CXXFLAGS += -D DEBUG
debug : main

clean :
	rm main *.o
