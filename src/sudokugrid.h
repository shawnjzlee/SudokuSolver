#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <vector>

using namespace std;

class SudokuGrid {
    public:
        SudokuGrid(int);
        SudokuGrid(int, string);
        ~SudokuGrid();
        
        bool add_cell(int, int, int);
        void populate_grid();
        void print_grid();
    
    private:
        int blanks;
        int size;
        vector<vector<int>> grid;
};

#endif