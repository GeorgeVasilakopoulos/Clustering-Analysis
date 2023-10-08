# Compiler
CC = g++

# Paths
COMMON = ./src/common
INCLUDE = .
SRC = ./src/common
MAIN = .


TARGET      := $(word 1, $(MAKECMDGOALS))

# Compile options
ifeq ($(TARGET),lsh)
	CXXFLAGS = -std=c++11 -g3 -Wall -I$(ILSH)
else ifeq ($(TARGET),cube)
	CXXFLAGS = -std=c++11 -g3 -Wall -I$(ICUBE)
else ifeq ($(TARGET),cluster)
	CXXFLAGS = -std=c++11 -g3 -Wall -I$(ICLUSTER)
else
	CXXFLAGS = -std=c++11 -g3 -Wall -I$(INCLUDE)
endif

CXXFLAGS += -I$(COMMON)

# .o files needed
SRCS    := $(wildcard $(SRC)/*.cpp)
OBJS    := $(subst .cpp,.o,$(SRCS))

# Executable
EXEC = main

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	@rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)
	
valgrind: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s ./$(EXEC) $(ARGS)