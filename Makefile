# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
  # compiler flags:
  #  -g     - this flag adds debugging information to the executable file
  #  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -Wall -std=c++17
 
  # The build target

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	TARGET = maze
else
	TARGET = maze.exe
endif
 
all: $(TARGET)
 
$(TARGET): maze.cpp game.cpp game.hpp levels.cpp levels.hpp visuals.cpp visuals.hpp
	$(CC) $(CFLAGS) -o $(TARGET) maze.cpp game.cpp visuals.cpp levels.cpp -l ncurses

clean:
	$(RM) $(TARGET)
