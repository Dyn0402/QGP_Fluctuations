# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11 $(shell /afs/rhic.bnl.gov/star/ROOT/38/6.20.08/build/bin/root-config --cflags)
LDFLAGS = $(shell /afs/rhic.bnl.gov/star/ROOT/38/6.20.08/build/bin/root-config --libs)

# Directories
SRC_DIR = src
REFMULT_DIR = StRefMultCorr

# Source files
SRC_CPP = $(wildcard $(SRC_DIR)/*.cpp)
REFMULT_CXX = $(wildcard $(REFMULT_DIR)/*.cxx)

# Object files
OBJ = $(SRC_CPP:.cpp=.o) $(REFMULT_CXX:.cxx=.o)

# Target executable
TARGET = tree_reader

# Default rule
all: $(TARGET)

# Path to Fluctuation_Lib
FLUCT_LIB_DIR = ../../Research_Libs/Fluctuation_Lib
FLUCT_LIB = $(FLUCT_LIB_DIR)/libfluctuation.a
FLUCT_INC = -I$(FLUCT_LIB_DIR)

# Add to compile and link flags
CXXFLAGS += $(FLUCT_INC)
LDFLAGS += $(FLUCT_LIB)

# Modify target rule to depend on the library
$(TARGET): $(OBJ) $(FLUCT_LIB)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Add a rule to build the library
$(FLUCT_LIB):
	$(MAKE) -C $(FLUCT_LIB_DIR)


# Link
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Compile .cpp files in src/
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cxx files in StRefMultCorr/
$(REFMULT_DIR)/%.o: $(REFMULT_DIR)/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(SRC_DIR)/*.o $(REFMULT_DIR)/*.o
