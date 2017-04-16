#include <thread>
#include <mutex>

#include "cmdline.h"
#include "sudokugrid.h"
using namespace std; 

#define PATH "../../dataset/"

int main(int argc, char * argv[]) {
    cmdline::parser flags;

    flags.add<int>("grid_size", 's', "size of sudoku grid (int)", true, 9);
    flags.add<int>("threads", 't', "number of threads to spawn (int)", false, 1);
    flags.add<string>("file", 'f', "pass in a file to initialize sudoku grid (string)", true, "");
    flags.parse_check(argc, argv);    
    
    SudokuGrid grid(flags.get<int>("grid_size"), PATH + flags.get<string>("file"));
    
    // grid.find_potential_val();
    
    // if(grid.solve_grid()) {
    //     cout << "Solution found: \n";
    //     grid.print_grid();
    // }
    // else cout << "No solution could be found for the generated grid.\n";
    
    return 0;
}