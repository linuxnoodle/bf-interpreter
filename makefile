PROJECT:=bf-interpreter
TESTER:=generate-print-program

CC:=g++
LDFLAGS=-O2 -std=c++17 -lstdc++fs
UNAME:=$(shell uname)

OBJDIR:=obj
OBJ:=$(addprefix $(OBJDIR)/,main.o interpreter.o)

.PHONY: all
all: $(PROJECT) $(TESTER)

$(PROJECT): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

$(TESTER):
	$(CC) ./src/tester.cpp $(LDFLAGS) -o $@

$(OBJDIR)/%.o: src/%.cpp | $(OBJDIR)
	$(CC) $(LDFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(PROJECT) $(TESTER)
