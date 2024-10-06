# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -MMD -MP

# Target executable
TARGET = grammar_program

# Source files
SRCS = main.cpp grammar.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Dependency files
DEPS = $(OBJS:.o=.d)

# Rule to compile the project
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files to .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include the generated dependency files
-include $(DEPS)

# Clean up the build
clean:
	rm -f $(OBJS) $(TARGET) $(DEPS)

# Run the program
run: $(TARGET)
	./$(TARGET)

