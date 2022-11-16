
CXXFLAGS += -std=c++20 -MD

CXXFLAGS += -Wall -Wextra -Wwrite-strings -Wno-parentheses
CXXFLAGS += -Wpedantic -Warray-bounds
CXXFLAGS += -Wconversion
CXXFLAGS += -Weffc++
CXXFLAGS += -Wuseless-cast

CXXFLAGS += -fPIC -gdwarf-4 -O3

wc: $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	$(LINK.cc) $^ $(LDLIBS) -o $@

-include $(wildcard *.d)

clean::
	$(RM) wc *.o *.d
.PHONY: clean
