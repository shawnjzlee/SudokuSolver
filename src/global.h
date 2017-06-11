#ifndef GLOBAL_H
#define GLOBAL_H

#include <mutex>
#include <atomic>
#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <thread>
#include <map>
#include <cmath>

extern std::mutex mutex_expanded_set;
extern std::mutex mutex_print_grid;
extern std::atomic_bool g_solution_found;

struct Benchmark {
    struct per_thread {
        double execution_time;
        int grids_expanded;
        
        char buffer[100];
        
        per_thread()
            : execution_time(0.0), grids_expanded(0) { }
    };
    std::map<std::thread::id, per_thread> results;

    int expanded, depth, max_queued_nodes;
    
    Benchmark() : expanded(0), depth(0), max_queued_nodes(0) { };
    
    double calculate_mean(std::vector<double> values) {
        return std::accumulate(values.begin(), values.end(), 0.0) / values.size();
    }
    
    double calculate_stdev(std::vector<double> values) {
        double mean = calculate_mean(values);
        double stdev;
        for(auto &i : values) {
            stdev += pow(i - mean, 2);
        }
        return sqrt(stdev / values.size());
    }
    
    void get_results(const int num_threads, const double total_exec_time) {
        std::string file = "results.csv";
        std::ofstream outfile;
        
        outfile.open(file, std::fstream::out | std::fstream::app);
        
        for_each(results.begin(), results.end(), [&](auto &i){
            outfile << i.first << ","
                    << total_exec_time << ","
                    << expanded << ","
                    << depth << ","
                    << max_queued_nodes << ","
                    << i.second.execution_time << ","
                    << i.second.grids_expanded
                    << std::endl;
        });

        outfile.close();
    }
};
extern struct Benchmark g_benchmark;

#endif /* global.h */