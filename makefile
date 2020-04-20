main : display/display.a display/display.h

display/display.a :
	$(MAKE) -C $(dir $(@))

debug : CXXFLAGS += -D DEBUG -ggdb
debug : main

clean :
	rm -f main *.o
