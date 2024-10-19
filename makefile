# Makefile

CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall
SRC = $(wildcard src/*.cpp)

OBJ = $(SRC:.cpp=.o)

ALL_SRC = main.cpp client.cpp $(SRC)

TARGET = main client

all: $(TARGET)

# $(TARGET): $(OBJ)
# 	$(CXX) -o $@ $^

main: main.o $(OBJ)
	$(CXX) -o $@ $^

client: client.o $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET) main.o client.o
