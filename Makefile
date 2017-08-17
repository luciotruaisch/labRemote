# Simple Makefile for multiple executeables + X other files/classes

MAKEFLAGS=--warn-undefined-variables

INCLUDE = -Iinclude/.#$(shell root-config --cflags)
LIBS = #$(shell root-config --libs)

# Define compiler and flags
CXX = g++
CFLAGS = -g -O0 -Wall -std=c++11 $(INCLUDE)
LDFLAGS = $(LIBS)

SRC = $(wildcard src/*.cpp) 
TARGET = $(wildcard *.cpp)
OBJ = $(patsubst src/%.cpp,build/%.o,$(SRC))
OOBJ = $(patsubst %.cpp,build/%.o,$(TARGET))
EXE = $(patsubst %.cpp,bin/%,$(TARGET))

all: $(OBJ) $(OOBJ) $(EXE)

build/%.o: %.cpp
	@echo "[Compiling] $@"
	@$(CXX) -c $(CFLAGS) $< -o $@

build/%.o: src/%.cpp
	@echo "[Compiling] $@"
	@$(CXX) -c $(CFLAGS) $< -o $@

bin/%: build/%.o $(OBJ)
	@echo "[Linking] $@"
	@$(CXX) $(CFLAGS) $(LDFLAGS) $< $(OBJ) -o $@

.PHONY: clean
clean:
	rm -f $(OBJ) $(OOBJ)
	rm -f $(EXE)
