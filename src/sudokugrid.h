#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <vector>
#include <bitset>

using namespace std;

class SudokuGrid {
    public:
        SudokuGrid();
        ~SudokuGrid();
        
        void init(int, string);
        
        void find_potential_val();
        bool add_cell(bool, int, int, int);
        void reduce(int, int, int);
        void remove_cell(int, int);
        bool solve_grid();
        void print_grid();
    
    private:
        int blanks;
        int size;
        vector<vector<vector<bool>>> grid;
};

#endif