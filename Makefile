CC=g++
CXXFLAGS=-g -std=c++17 -pedantic
ODIR=.
BUILD=build

_OBJ=$(BUILD)/G2CNF.o $(BUILD)/Gramatica.o
OBJ=$(patsubst %,%,$(_OBJ))

all:G2CNF

$(BUILD)/G2CNF.o: $(ODIR)/G2CNF.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS)

$(BUILD)/Gramatica.o: $(ODIR)/Gramatica.cpp
	$(CC) -c -o $@ $< $(CXXFLAGS)


G2CNF: $(OBJ)
	$(CC) -o $@ $^ $(CXXFLAGS) -lm

.PHONY:clean

clean:
	rm -rf build/* G2CNF

