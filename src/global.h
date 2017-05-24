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
    std::vector<double> thread_execution_time; // 0
    // std::vector<double> expanded_emplace_time; // 1
    // std::vector<double> expanded_find_time; // 2
    // std::vector<double> mutex_emplace_lock_contention; // 3
    // std::vector<double> mutex_find_lock_contention; // 4
    std::vector<int> thread_expanded;
    std::vector<double> thread_emplace_lock;
    std::vector<double> thread_find_lock;
    std::vector<double> thread_expanded_emplace;
    std::vector<double> thread_expanded_find;
    // std::vector<double> sudokugrid_constructor; // 4
    unsigned int expanded, depth, max_queued_nodes, total_exec_time;
    
    // std::array<std::mutex, 5> mutex_benchmark;
    
    Benchmark() : expanded(0), depth(0), max_queued_nodes(0) { };
    
    void get_results(const int num_threads, const double total_exec_time) {
        std::string file = "results.csv";
        std::ofstream outfile;
        
        outfile.open(file, std::fstream::out | std::fstream::app);
        // std::cout << "Container sizes:\t" << thread_execution_time.size() << ", " << expanded_emplace_time.size() << ", " 
        //           << expanded_find_time.size() << ", " << mutex_emplace_lock_contention.size() << ", " 
        //           << mutex_find_lock_contention.size() << std::endl;
        
        std::cout << "Runtime per thread:\n";      
        for_each(thread_execution_time.begin(), thread_execution_time.end(), [](auto &i) {
            std::cout << i << "\t";
        });
        
        std::cout << "\nNumber of expanded nodes per thread:\n";      
        for_each(thread_expanded.begin(), thread_expanded.end(), [](auto &i) {
            std::cout << i << "\t";
        });
        
        std::cout << "\nSum of emplace lock time per thread:\n";      
        for_each(thread_emplace_lock.begin(), thread_emplace_lock.end(), [](auto &i) {
            std::cout << i << "\t";
        });
        
        std::cout << "\nSum of find lock time per thread:\n";      
        for_each(thread_find_lock.begin(), thread_find_lock.end(), [](auto &i) {
            std::cout << i << "\t";
        });
        
        std::cout << "\nSum of emplace call time per thread:\n";      
        for_each(thread_expanded_emplace.begin(), thread_expanded_emplace.end(), [](auto &i) {
            std::cout << i << "\t";
        });
        
        std::cout << "\nSum of find call time per thread:\n";      
        for_each(thread_expanded_find.begin(), thread_expanded_find.end(), [](auto &i) {
            std::cout << i << "\t";
        });
                  
        outfile << num_threads << "," << expanded << "," << depth << "," << max_queued_nodes << ",";
        // outfile << total_exec_time << ",";
        // outfile << AVG(thread_execution_time) << ",";
        // outfile << AVG(expanded_emplace_time) << ",";
        // outfile << AVG(expanded_find_time) << ",";
        // outfile << AVG(mutex_emplace_lock_contention) << ",";
        // outfile << std::accumulate(mutex_emplace_lock_contention.begin(), mutex_emplace_lock_contention.end(), 0.0) << "," << ",";
        // outfile << AVG(mutex_find_lock_contention) << ",";
        // outfile << std::accumulate(mutex_find_lock_contention.begin(), mutex_find_lock_contention.end(), 0.0);
        
        outfile << std::endl;
        outfile.close();
    }
};
extern struct Benchmark g_benchmark;

#endif /* global.h */