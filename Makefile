# Basic Init Makefile.
# Change CPPFLAGS to define or undefine DEBUG.

CXX=g++-8
#CPPFLAGS=--static -Iinclude -DDEBUG -std=c++17 -Wall
CPPFLAGS=--static -Iinclude -std=c++17 -Wall
LINKERARGS=-lstdc++fs
SOURCEDIR=src/
SOURCE:=$(shell find $(SOURCEDIR) -name '*.cpp')
BINARY:=$(SOURCE:.cpp=.o)
NAME=init
.PHONY=all clean

all: $(NAME)
	@true
clean:
	rm $(BINARY) $(NAME) &> /dev/null; true

linking $(NAME): $(BINARY)
	$(CXX) $(CPPFLAGS) $(BINARY) $(LINKERARGS) -o $@

%.d: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ $<

cloc:
	cloc src include Makefile
