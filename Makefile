CXX=clang++
CXXFLAGS:=-Wall -Wextra -g -llua5.4 -DVERSION=\"ALPHA\"
BUILD:=./build

OBJECTS := src/compiler.cpp

all:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJECTS) -o $(BUILD)/fltc

clean:
	rm -f fltc

install:
	cp ./fltc /usr/bin/fltc

uninstall:
	rm /usr/bin/fltc
