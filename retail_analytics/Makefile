CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
INCLUDES = -I.

# Target executable
TARGET = retail_analytics.exe

# Source files
SRC = main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(INCLUDES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del $(OBJ) $(TARGET)

.PHONY: all clean