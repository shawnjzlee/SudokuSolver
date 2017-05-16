#ifndef GLOBAL_H
#define GLOBAL_H

#include <map>
#include <memory>
#include <mutex>
#include <atomic>

extern std::mutex mutex_expanded_set;
extern std::mutex mutex_print_grid;
extern std::atomic_bool g_solution_found;

#endif /* global.h */