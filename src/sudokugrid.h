#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <vector>

using namespace std;

class SudokuGrid {
    public:
        SudokuGrid();
        ~SudokuGrid();
        
        void init(int);
        void init(int, string);
        
        void find_next_cell(int&, int&);
        bool add_cell(bool, int, int, int);
        void remove_cell(int, int);
        bool solve_grid();
        void populate_grid();
        void print_grid();
    
    private:
        int blanks;
        int size;
        vector<vector<int>> grid;
};

#endif