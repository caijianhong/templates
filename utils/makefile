main: $(shell basename $$PWD).cpp
	g++ $< -o $@ -g -DNF -O0 -DLOCAL -fsanitize=undefined,address -D_GLIBCXX_DEBUG
%: %.cpp
	g++ $< -o $@ -g -DNF -O2 -Wall -Wconversion -Wextra -Wshadow -std=c++14 -pg

