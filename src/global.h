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
        double emplace_lock_time;
        double find_lock_time;
        double emplace_time;
        double find_time;
        int valid_grid_checks;
        double valid_grid_check_time;
        int reduction_checks;
        double reduction_check_time;
        int reductions;
        double reduction_time;
        
        
        char buffer[100];
        
        per_thread()
            : execution_time(0.0), grids_expanded(0), emplace_lock_time(0.0),
              find_lock_time(0.0), emplace_time(0.0), find_time(0.0),
              valid_grid_checks(0), valid_grid_check_time(0.0), reduction_checks(0),
              reduction_check_time(0.0), reductions(0), reduction_time(0.0) { }
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
                    << i.second.grids_expanded << ","
                    << i.second.emplace_lock_time << ","
                    << i.second.find_lock_time << ","
                    << i.second.emplace_time << ","
                    << i.second.find_time << ","
                    << i.second.valid_grid_checks << ","
                    << i.second.valid_grid_check_time << ","
                    << i.second.reduction_checks << ","
                    << i.second.reduction_check_time << ","
                    << i.second.reductions << ","
                    << i.second.reduction_time
                    << std::endl;
        });

        outfile.close();
    }
};
extern struct Benchmark g_benchmark;

#endif /* global.h */