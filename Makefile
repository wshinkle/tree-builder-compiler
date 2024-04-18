# Variables
YACC = yacc
YFLAGS = -d
LEX = lex
CXX = g++
CXXFLAGS = -Wall -std=c++11 -g
YACC_SRC = tree_builder.y
LEX_SRC = tree_builder.l
CPP_SRC = $(wildcard *.cpp)
OBJ = $(CPP_SRC:.cpp=.o) y.tab.o lex.yy.o
EXE = tree_builder

# Default target
all: $(EXE)

# YACC
y.tab.c y.tab.h: $(YACC_SRC)
	$(YACC) $(YFLAGS) $<

# Lex
lex.yy.c: $(LEX_SRC) y.tab.h
	$(LEX) $<

# Compile
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $<

# Link
$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Run
run: $(EXE)
	./$(EXE)

# Clean
clean:
	rm -f $(OBJ) $(EXE) y.tab.c y.tab.h lex.yy.c Examples/*.output

test: $(EXE)
	./test.sh


.PHONY: all run clean