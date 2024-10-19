# Makefile for the fan control project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11

# Linker flags
LDFLAGS = -lpigpio -lpthread

# Executable name
TARGET = cpu_fan_control

# Source files
SRCS = cpu_fan_control.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	sudo ./$(TARGET)

.PHONY: all clean run

