main : display/display.a display/screen.h display/view.h

display/display.a :
	$(MAKE) -C $(dir $(@))

debug : CXXFLAGS += -D DEBUG -ggdb
debug : main

clean :
	rm -f main *.o
