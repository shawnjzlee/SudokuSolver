#include <thread>
#include <mutex>

#include "cmdline.h"
#include "sudokugrid.h"
using namespace std;

int main(int argc, char * argv[]) {
    cmdline::parser flags;

    flags.add<int>("grid_size", 's', "size of sudoku grid (int)", false, 9);
    flags.parse_check(argc, argv);    
    
    SudokuGrid grid(flags.get<int>("grid_size"));
    
    return 0;
}