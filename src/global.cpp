#include "global.h"
#include "sudokugrid.h"

std::mutex mutex_expanded_set;
std::mutex mutex_print_grid;
std::atomic_bool g_solution_found(false);