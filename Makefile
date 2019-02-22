PROGRAM    := hanoi
CSOURCES   :=
CXXSOURCES := hanoi.cpp main.cpp solve.cpp

COBJECTS   := $(CSOURCES:%.c=%.o)
CXXOBJECTS := $(CXXSOURCES:%.cpp=%.o)
CDEPENDS   := $(CSOURCES:%.c=%.d)
CXXDEPENDS := $(CXXSOURCES:%.cpp=%.d)

COMPFLAGS := -Wall -Wextra -Wpedantic -Werror -pedantic-errors -O0 -g3 -pipe
CPPFLAGS  := -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC
CFLAGS    := $(COMPFLAGS) -std=c18
CXXFLAGS  := $(COMPFLAGS) -std=c++17
LDFLAGS   :=

CC    := gcc
CXX   := g++
DEBUG := gdb

$(PROGRAM): $(COBJECTS) $(CXXOBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -MMD -MP $<
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -MMD -MP $<

-include $(CDEPENDS)
-include $(CXXDEPENDS)

.PHONY: run debug clean
run: $(PROGRAM)
	./$(PROGRAM)

debug: $(PROGRAM)
	$(DEBUG) $(PROGRAM)

clean:
	$(RM) $(PROGRAM) *.o *.d
