sources = screen.cpp view.cpp touch.cpp

display.a : $(sources:.cpp=.o)
	ar ru $@ $^
	ranlib $@

%.make: %.cpp
	@set -e; rm -f $@; \
	$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include $(sources:.cpp=.make)

debug : CXXFLAGS += -D DEBUG -ggdb
debug : display.a

clean:
	$(RM) *.a *.o *.make
