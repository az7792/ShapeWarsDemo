# Makefile

CXX = g++
CXXFLAGS = -Iinclude -std=c++17 -Wall
SRC = $(wildcard src/*.cpp)

OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))
MAIN_OBJ = build/main.o
CLIENT_OBJ = build/client.o

ALL_SRC = main.cpp client.cpp $(SRC)

TARGET = main client

all: $(TARGET)

# $(TARGET): $(OBJ)
# 	$(CXX) -o $@ $^

main: build/main.o $(OBJ)
	$(CXX) -o $@ $^

client: build/client.o $(OBJ)
	$(CXX) -o $@ $^


build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) build/*
