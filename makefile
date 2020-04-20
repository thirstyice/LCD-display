sources= main.cpp
CC=$(CXX)

main : display/display.a

display/display.a : display/*
	$(MAKE) -C $(dir $(@))

%.make: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

display/%.make:
	# Stay in your lane, make

include $(sources:.cpp=.make)



debug : CXXFLAGS += -D DEBUG -ggdb
debug : main

clean :
	rm -f main *.o *.make
	$(MAKE) -C display/ clean
