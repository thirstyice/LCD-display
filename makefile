main : application.o screen.o buffer.o view.o

application.o : screen.o buffer.o

screen.o : buffer.o view.o

debug : CXXFLAGS += -D DEBUG
debug : main

clean :
	rm main *.o
