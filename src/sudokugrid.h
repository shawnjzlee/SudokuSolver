#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <string>
#include <vector>
#include "point.h"

using namespace std;

class SudokuGrid {
    public:
        SudokuGrid(int, string);
        ~SudokuGrid();
        
        bool add_cell(const int, const int, const int);
        void print_grid();
        
    private:
        int blanks;
        int size;
        vector<Point> grid;
};

#endif