#include <mutex>

#include "global.h"
#include "sudokugrid.h"

std::mutex mutex_expanded_set;
std::mutex mutex_print_grid;
// struct PossibleValueCmp cmp;