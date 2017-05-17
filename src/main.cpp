#include <chrono>
#include <iostream>

#include "cmdline.h"
#include "sudokugrid.h"
using namespace std; 
using namespace std::chrono;

#define PATH "../../dataset/"

int main(int argc, char * argv[]) {
    cmdline::parser flags;

    flags.add<int>("grid_size", 's', "size of sudoku grid (int)", false, 9);
    flags.add<int>("threads", 't', "number of threads to spawn (int)", false, 1);
    flags.add<string>("file", 'f', "pass in a file to initialize sudoku grid (string)", true, "");
    flags.parse_check(argc, argv);    
    
    SudokuGrid grid(flags.get<int>("grid_size"), PATH + flags.get<string>("file"));
    
    high_resolution_clock::time_point start = high_resolution_clock::now();
    // grid.solve();
    grid.thread_distribution(flags.get<int>("threads"));
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> runtime = duration_cast<duration<double>>(end - start);
    cout << "Time to solve (in secs): " << runtime.count() << endl;
    
    return 0;
}