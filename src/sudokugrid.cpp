#include <algorithm>
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
    cout << "Populating grid...\n";
    populate_grid();
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
bool SudokuGrid::add_cell(int row, int col, int cell_value) {
    // optimize
    // 1) check valid number within subgrid
    int row_left_bound(0), row_right_bound(0), col_left_bound(0), col_right_bound(0);
    if (row < sqrt(size))
        row_right_bound = sqrt(size);
    else if (row < (2* sqrt(size))) {
        row_left_bound = sqrt(size);
        row_right_bound = 2 * sqrt(size);
    }
    else {
        row_left_bound = 2 * sqrt(size);
        row_right_bound = size;
    }
    
    if (col < sqrt(size))
        col_right_bound = sqrt(size);
    else if (col < (2* sqrt(size))) {
        col_left_bound = sqrt(size);
        col_right_bound = 2 * sqrt(size);
    }
    else {
        col_left_bound = 2 * sqrt(size);
        col_right_bound = size;
    }
    // cout << "Row bound: " << row_left_bound << " " << row_right_bound << "\n";
    // cout << "Col bound:" << col_left_bound << " " << col_right_bound << "\n";
    for(int i = row_left_bound; i < row_right_bound; i++) {
        for(int j = col_left_bound; j < col_right_bound; j++) {
            if (cell_value == grid.at(i).at(j)) return false;
            // cout << "Access: " << i << "\t" << j << endl;
        }
    }
    // 2) check if number exists in its respective row or col already
    for(int i = 0; i < size; i++) {
        if (cell_value == grid.at(row).at(i)) return false;
        if (cell_value == grid.at(i).at(col)) return false;
    }    
    grid.at(row).at(col) = cell_value;
    return true;
}

void SudokuGrid::populate_grid() {
    srand(time(0));
    
    int cells_to_populate = rand() % ((size*size) - 1);
    int cell_value (0), row (0), col (0), subgrid (0);
    bool return_val = false;
    while(cells_to_populate > 0) {
        row = rand() % size;
        col = rand() % size;
        cell_value = (rand() % size) + 1;
        return_val = add_cell(row, col, cell_value);
        
        // cout << cells_to_populate << ": " << row << " , " << col 
            //  << " with value " << cell_value << " returned " << return_val << endl;
        
        // print_grid();
        // cout << endl << endl;
        if(return_val) cells_to_populate--;
    }
    print_grid();
}

void SudokuGrid::print_grid() {
    for(auto &i : grid) {
        for_each(i.begin(), i.end(), [&](int &j) { cout << j << " "; });
        cout << endl;
    }
}
SudokuGrid::~SudokuGrid() { }