CC=g++-5
CFLAGS=-c -std=c++14
LDFLAGS=-pthread -lpthread
SOURCES=main.cpp sudokugrid.cpp point.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sudoku_solver
BUILD_DIR=./bin

all: $(SOURCES) $(EXECUTABLE)
	
debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) copy
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$@
	rm *.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -rf $(BUILD_DIR)
	
copy:
	mkdir -p $(BUILD_DIR)
	cp -f ./Makefile $(BUILD_DIR)/Makefile
	
restart:
	cd .. && make && cd bin/ && ./sudoku_solver -s 9 -f p0.txt