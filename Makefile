.RECIPEPREFIX = >
CXX = g++
WINDRES = windres
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc
LDFLAGS = -lraylib -lopengl32 -lgdi32 -lwinmm

SRCS = DSA.cpp $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)
RC_FILE = DSA.rc
RES_OBJ = resource.o

DEBUG_TARGET = DSA_D.exe
RELEASE_TARGET = DSA.exe

all: CXXFLAGS += -g -O0
all: $(DEBUG_TARGET)

release: CXXFLAGS += -O2 -DNDEBUG
release: LDFLAGS += -mwindows
release: clean_objs $(RELEASE_TARGET)

build: all
> ./$(DEBUG_TARGET)

build-release: release
> ./$(RELEASE_TARGET)

$(DEBUG_TARGET): $(OBJS) $(RES_OBJ)
> $(CXX) $(OBJS) $(RES_OBJ) -o $(DEBUG_TARGET) $(LDFLAGS)

$(RELEASE_TARGET): $(OBJS) $(RES_OBJ)
> $(CXX) $(OBJS) $(RES_OBJ) -o $(RELEASE_TARGET) $(LDFLAGS)

$(RES_OBJ): $(RC_FILE)
> $(WINDRES) $(RC_FILE) -O coff -o $(RES_OBJ)

%.o: %.cpp
> $(CXX) $(CXXFLAGS) -c $< -o $@

clean_objs:
> rm -f $(OBJS) $(RES_OBJ)

clean:
> rm -f $(OBJS) $(RES_OBJ) $(DEBUG_TARGET) $(RELEASE_TARGET)

.PHONY: all release build build-release clean_objs clean
