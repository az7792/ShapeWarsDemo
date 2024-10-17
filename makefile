# Makefile

CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall
SRC = main.cpp src/Time.cpp

OBJ = $(SRC:.cpp=.o)
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
