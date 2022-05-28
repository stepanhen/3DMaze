# the compiler: gcc for C program, define as g++ for C++
CC = g++
 
  # compiler flags:
  #  -g     - this flag adds debugging information to the executable file
  #  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -Wall
 
  # The build target 
TARGET = maze
 
all: $(TARGET)
 
$(TARGET): $(TARGET).cpp game.cpp game.hpp levels.cpp levels.hpp visuals.cpp visuals.hpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp game.cpp visuals.cpp levels.cpp -l ncurses

clean:
	$(RM) $(TARGET)
