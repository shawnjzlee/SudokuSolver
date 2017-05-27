CC=g++-5
CFLAGS=-c -std=c++14
LDFLAGS=-pthread -lpthread
SOURCES=main.cpp sudokugrid.cpp point.cpp global.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sudoku_solver
BUILD_DIR=./bin

all: $(SOURCES) $(EXECUTABLE)
	
debug: CFLAGS += -DDEBUG -g
debug: $(EXECUTABLE)

verbose: CFLAGS += -DVERBOSE
verbose: $(EXECUTABLE)

bench: CFLAGS += -DBENCH
bench: $(EXECUTABLE)
    
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
	cp -f ./benchmark.sh $(BUILD_DIR)/benchmark.sh
	echo "TID,Total Exec Time, Total Grids Expanded, Furthest Depth, Largest Fringe Size, Grids Expanded,Emplace Lock Time, Find Lock Time, Emplace Time, Find Time" >> $(BUILD_DIR)/results.csv
	
restart:
	cd .. && make && cd bin/ && ./sudoku_solver -s 16 -f ${ARG}

restart_v:
	cd .. && make verbose && cd bin/ && ./sudoku_solver -s 16 -f ${ARG}
	
restart_d:
	cd .. && make debug && cd bin/ && gdb --args ./sudoku_solver -s 16 -f ${ARG}
	
restart_b:
	cd .. && make bench && cd bin/ && ./sudoku_solver -s 16 -f ${ARG}
	
restart_t:
	cd .. && make && cd bin/ && ./sudoku_solver -s 16 -f ${ARG1} -t ${ARG2}
	
restart_tv:
	cd .. && make verbose && cd bin/ && ./sudoku_solver -s 16 -f ${ARG1} -t ${ARG2}
	
restart_td:
	cd .. && make debug && cd bin/ && gdb --args ./sudoku_solver -s 16 -f ${ARG1} -t ${ARG2}
		
restart_tb:
	cd .. && make bench && cd bin/ && ./sudoku_solver -s 16 -f ${ARG1} -t ${ARG2}