#include <iostream>
#include <fstream>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "sudokugrid.h"

using namespace std;

SudokuGrid::SudokuGrid(int size) {
    if (size < 0) {
        cout << "Grid size cannot be less than 0\n";
        exit(-1);
    }
    int square_root(round(sqrt(size)));
    if (size == square_root * square_root) {
        this->size = size;
        grid.resize(size);
        for(auto &col : grid) col.resize(size);
    }
    else {
        cout << "Grid size is not a perfect square\n";
        exit(-1);
    }
}

SudokuGrid::SudokuGrid(int size, string file) {
    ifstream infile(file);
    if (infile.fail()) {
        cout << "Failed to open file.\n";
        exit(-1);
    }
    char next;
    int row (0), col (0), cell_value (0);
    
    while(!infile.eof()) {
        infile.get(next);
        if (isspace(next)) {
            col = 0;
            row++;
        }
        else {
            if (row > size || col > size) continue;
            
            cell_value = next;
            grid[row][col] = cell_value;
            col++;
        }
    }
    row = col = 0;
    infile.close();
}

/* @param: 
        [1] takes subgrid label (starting from 0),
        [2] row to access, [3] col to access, 
        [4] cell_value to place into the cell
*/
bool SudokuGrid::add_cell(int subgrid, int row, int col, int cell_value) {
    // optimize
    for(int row = subgrid * sqrt(size); row < sqrt(size); row++) {
        for(int col = subgrid * sqrt(size); col < sqrt(size); col++) {
            // 1) check valid number within subgrid
            // 2) check if number exists in its respective row or col already
        }
    }
    return true;
}

void SudokuGrid::populate_grid() {
    srand(time(0));
    
    int cells_to_populate = rand() % (size - 1);
    int cell_value (0), row (0), col (0), subgrid (0);
    bool return_val = false;
    while(cells_to_populate > 0) {
        subgrid = (rand() % size) + 1;
        row = (rand() % ((int)sqrt(size))) + 1;
        col = (rand() % ((int)sqrt(size))) + 1;
        cell_value = (rand() % size) + 1;
        return_val = add_cell(subgrid, row, col, cell_value);
        
        if(return_val) cells_to_populate--;
    }
}

SudokuGrid::~SudokuGrid() { }