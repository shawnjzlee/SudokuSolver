#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>

extern std::mutex mutex_expanded_set;
extern std::mutex mutex_print_grid;
extern std::atomic_bool g_solution_found;

struct Benchmark {
    std::vector<double> thread_execution_time; // 0
    std::vector<double> mutex_expanded_lock_time; // 1
    std::vector<double> mutex_expanded_lock_contention; // 2
    // std::vector<double> sudokugrid_constructor; // 3
    unsigned int threads_spawned, expanded, depth, max_queued_nodes, total_exec_time;
    
    std::array<std::mutex, 4> mutex_benchmark;
    
    Benchmark() : threads_spawned(0), expanded(0), depth(0), max_queued_nodes(0) { };
    
    void get_results(const int num_threads, const double total_exec_time) {
        std::string file = "results.csv";
        std::ofstream outfile;
        
        outfile.open(file, std::fstream::out | std::fstream::app);
        std::cout << "Container sizes:\t" << thread_execution_time.size() << ", " << mutex_expanded_lock_time.size() << ", " 
                  << mutex_expanded_lock_contention.size() << std::endl;
        outfile << num_threads << "," << threads_spawned << "," << expanded << "," << depth << "," << max_queued_nodes << ",";
        outfile << total_exec_time << ",";
        outfile << std::accumulate(thread_execution_time.begin(), thread_execution_time.end(), 0.0) / thread_execution_time.size() << ",";
        outfile << std::accumulate(mutex_expanded_lock_time.begin(), mutex_expanded_lock_time.end(), 0.0) / mutex_expanded_lock_time.size() << ",";
        outfile << std::accumulate(mutex_expanded_lock_contention.begin(), mutex_expanded_lock_contention.end(), 0.0) / mutex_expanded_lock_contention.size();
        // outfile << std::accumulate(sudokugrid_constructor.begin(), sudokugrid_constructor.end(), 0.0) / sudokugrid_constructor.size() << ",";
        
        outfile << std::endl;
        outfile.close();
    }
};
extern struct Benchmark g_benchmark;

#endif /* global.h */