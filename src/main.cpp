#include <chrono>
#include <iostream>
#include <thread>

#include "cmdline.h"
#include "sudokugrid.h"
#include "global.h"

using namespace std;
using namespace std::chrono;

#define PATH "../../dataset/"

int main(int argc, char * argv[]) {
    cmdline::parser flags;

    flags.add<int>("grid_size", 's', "size of sudoku grid (int)", true, 16);
    flags.add<int>("threads", 't', "number of threads to spawn (int)", false, 1);
    flags.add<string>("file", 'f', "pass in a file to initialize sudoku grid (string)", true, "");
    flags.parse_check(argc, argv);
    
    int num_threads(flags.get<int>("threads"));
    if(num_threads > thread::hardware_concurrency()) {
        num_threads = thread::hardware_concurrency();
        cout << "\033[2;33mwarning:\033[0m too many threads requested. Spawning "
             << thread::hardware_concurrency() << " threads.\n";
    }
    else cout << "Spawning " << num_threads << " threads.\n";
    
    #ifdef BENCH
    g_benchmark.thread_expanded.resize(num_threads, 0);
    #endif
    
    SudokuGrid grid(flags.get<int>("grid_size"), PATH + flags.get<string>("file"));
    high_resolution_clock::time_point start = high_resolution_clock::now();
    grid.thread_distribution(num_threads, true);
    high_resolution_clock::time_point end = high_resolution_clock::now();
    duration<double> runtime = duration_cast<duration<double>>(end - start);
    cout << "Time to solve (in secs): " << runtime.count() << endl;
    
    #ifdef BENCH
    g_benchmark.get_results(flags.get<int>("threads"), runtime.count());
    #endif
    
    return 0;
}