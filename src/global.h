#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>

extern std::mutex mutex_expanded_set;
extern std::mutex mutex_print_grid;
extern std::atomic_bool g_solution_found;

#define AVG(X) std::accumulate(X.begin(), X.end(), 0.0) / X.size()

struct Benchmark {
    struct per_thread {
        double execution_time;
        int grids_expanded;
        double emplace_lock_time;
        double find_lock_time;
        double emplace_time;
        double find_time;
        
        char buffer[100];
    };
    std::vector<per_thread> results;
    
    int expanded, depth, max_queued_nodes;
    
    Benchmark() : expanded(0), depth(0), max_queued_nodes(0) { };
    
    void get_results(const int num_threads, const double total_exec_time) {
        std::string file = "results.csv";
        std::ofstream outfile;
        
        outfile.open(file, std::fstream::out | std::fstream::app);
        
        for(int i = 0; i < results.size(); i++) {
            outfile << i << ","
                    << results.at(i).execution_time << ","
                    << results.at(i).grids_expanded << ","
                    << results.at(i).emplace_lock_time << ","
                    << results.at(i).find_lock_time << ","
                    << results.at(i).emplace_time << ","
                    << results.at(i).find_time << ","
                    << std::endl;
        }
        
        outfile << std::endl;
        outfile.close();
    }
};
extern struct Benchmark g_benchmark;

#endif /* global.h */