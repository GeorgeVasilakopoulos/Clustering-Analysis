# Compiler
CC = g++

# Paths
COMMON       := ./src/common
COMMON_INCS  := $(COMMON)/include
COMMON_PROG  := $(wildcard $(COMMON)/*.cpp)
COMMON_SRCS  := $(wildcard $(COMMON)/modules/*.cpp)
COMMON_OBJS  := $(subst .cpp,.o,$(COMMON_PROG)) $(subst .cpp,.o,$(COMMON_SRCS))


LSH		  := ./src/approximators/lsh
LSH_INCS  := $(LSH)/include
LSH_PROG  := $(wildcard $(LSH)/*.cpp)
LSH_SRCS  := $(wildcard $(LSH)/modules/*.cpp) $(COMMON_SRCS)
LSH_OBJS  := $(subst .cpp,.o,$(LSH_PROG)) $(subst .cpp,.o,$(LSH_SRCS))


CUBE	   := ./src/approximators/cube
CUBE_INCS  := $(CUBE)/include
CUBE_PROG  := $(wildcard $(CUBE)/*.cpp)
CUBE_SRCS  := $(wildcard $(CUBE)/modules/*.cpp) $(COMMON_SRCS)
CUBE_OBJS  := $(subst .cpp,.o,$(CUBE_PROG)) $(subst .cpp,.o,$(CUBE_SRCS))


CLUSTER	   	  := ./src/cluster
CLUSTER_INCS  := $(CLUSTER)/include
CLUSTER_SRCS  := $(wildcard $(CLUSTER)/*.cpp) $(wildcard $(CLUSTER)/modules/*.cpp) $(COMMON_SRCS) $(LSH_SRCS) $(CUBE_SRCS)
CLUSTER_OBJS  := $(subst .cpp,.o,$(CLUSTER_SRCS))



TARGET   := $(word 1, $(MAKECMDGOALS))
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra 

# Compile options
ifeq ($(TARGET),lsh)
	CXXFLAGS += -I$(LSH_INCS)
else ifeq ($(TARGET),cube)
	CXXFLAGS += -I$(LSH_INCS) -I$(CUBE_INCS)
else ifeq ($(TARGET),cluster)
	CXXFLAGS += -I$(LSH_INCS) -I$(CUBE_INCS) -I$(CLUSTER_INCS)
endif

CXXFLAGS += -I$(COMMON_INCS)


%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $^ -o $@

clean:
	@rm -f $(COMMON_OBJS) $(LSH_OBJS) $(CUBE_OBJS) $(CLUSTER_OBJS) ./common ./lsh ./cube ./cluster

run: $(COMMON_OBJS)
	$(CC) $(COMMON_OBJS) -o ./common

lsh: $(LSH_OBJS)
	$(CC) $^ -o ./lsh

cube: $(CUBE_OBJS)
	$(CC) $^ -o ./cube

cluster: $(CLUSTER_OBJS)
	$(CC) $^ -o ./cluster

all: 
	make lsh; make cube; make cluster;