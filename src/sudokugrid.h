#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <vector>

using namespace std;

class SudokuGrid {
    public:
        SudokuGrid(int);
        ~SudokuGrid();
    
    private:
        vector<vector<int>> grid;
};

#endif