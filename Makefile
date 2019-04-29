CPP=g++
CPPFLAGS=--static -Iinclude -DDEBUG
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
	$(CPP) $(CPPFLAGS) $(BINARY) -o $@

%.d: %.cpp
	$(CPP) $(CPPFLAGS) -o $@ $<
