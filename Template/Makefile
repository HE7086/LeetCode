CXXFLAGS ?= -std=c++23 -march=x86-64-v3 -O2 -fno-plt -pipe \
			-fPIC -fstack-clash-protection -fcf-protection \
			-Wall -Wextra -Wpedantic -Werror \
			-flto=auto -ffat-lto-objects 
CPPFLAGS ?= -Wp,-D_FORTIFY_SOURCE=3,-D_GLIBCXX_ASSERTIONS
LDFLAGS ?= -Wl,-O1,--sort-common,--as-needed,-z,relro,-z,now,-z,pack-relative-relocs
LDLIBS ?= -lstdc++ -lm -lfmt

PREFIX ?= /usr/local

SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
EXE := main

all: $(EXE)

$(EXE): $(OBJECTS)
$(OBJECTS): %o: %cpp

.PHONY: clean
clean:
	$(RM) $(EXE) *.o
