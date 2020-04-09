main : buffer.o

buffer.o : buffer.cpp buffer.h

clean :
	rm main *.o
