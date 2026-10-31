CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wno-unused-parameter -fsanitize=address
INCLUDES = -I./include/

SRC_DIR := src
BIN_DIR := bin

TARGET = $(BIN_DIR)/libthing 
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRCS))

all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^

# Compile .cpp files to .o files in the bin directory
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -rf $(BIN_DIR)

.PHONY: all clean
