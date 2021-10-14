# Compiler
CXX = g++-11

# Warnings
WARN = -Wall -Wextra

# Link
LINK =

# Include Directory
IDIR = -Iinclude

# Optimizing Flags
OPT = -O3 -march=native
main: OPT += -flto
debug: OPT = -O0 -march=native

# Flags
CXXFLAGS = $(WARN) -std=c++20
debug: CXXFLAGS += -g

# Source
srcCPP = $(wildcard ./src/*.cpp) $(wildcard ./src/*/*.cpp)
_objCPP = $(srcCPP:.cpp=.o)
_asmCPP = $(srcCPP:.cpp=.s)
objCPP = $(subst src,bin/obj,$(_objCPP))
asmCPP = $(subst src,bin/asm,$(_asmCPP))

obj = $(objCPP)
asm = $(asmCPP)

out_name=chess_engine

# Compile
main: $(obj)
	$(CXX) $(CXXFLAGS) $(OPT) $(IDIR) -o $(out_name) $^ $(LINK)

debug: $(obj) 
	$(CXX) $(CXXFLAGS) $(OPT) $(IDIR) -o $(out_name) $^ $(LINK)

assembly: $(asm)

bin/obj/%.o: src/%.cpp | dir_make
	$(CXX) $(CXXFLAGS) $(OPT) $(IDIR) -c -o $@ $^ $(LINK)

bin/asm/%.s: src/%.cpp
	$(CXX) $(CXXFLAGS) $(OPT) $(IDIR) -S -o $@ $^ $(LINK)

dir_make:
	@mkdir -p bin/obj bin/asm

.PHONY: clean

# Clean
clean:
	rm -rf bin/obj/*.o bin/asm/*.s $(out_name)
