#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>

#include "sudokugrid.h"

using namespace std;

SudokuGrid::SudokuGrid(int size, string file) :
        blanks(0), size(size) { 
    ifstream infile(file);
    if (infile.fail()) {
        cout << "Failed to open file.\n";
        exit(-1);
    }
    if (size < 0) {
        cout << "Grid size cannot be less than 0\n";
        exit(-1);
    }
    
    int square_root(round(sqrt(size)));
    if (size == square_root * square_root)
        grid.resize(size * size);
    else {
        cout << "Grid size is not a perfect square\n";
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
            if (row == size || col == size) break;
            cell_value = (int)next - (int)'0';
            if(cell_value == 0) blanks++;
            auto rc = add_cell(row, col, cell_value);
            if (!rc) cout << "Incorrect value placement. Please check file" << endl;
            col++;
        }
    }
    row = col = 0;
    infile.close();
    cout << "Grid initialized:\n";
    print_grid();
}



bool SudokuGrid::add_cell(const int row, const int col, const int cell_value) {
    // optimize
    Point point(row, col, size, cell_value);
    
    if (cell_value == 0) {
        grid.at(row*size + col) = point;
        return true;
    }
    // 1) check if cell is already populated
    // if (grid.at(index).possible_values().size() > 0) 
    //     return false;
        
    // 2) check valid number within subgrid
    int row_left_bound(0), row_right_bound(0), col_left_bound(0), col_right_bound(0);
    if (row < sqrt(size)) row_right_bound = sqrt(size);
    else if (row < (2* sqrt(size))) {
        row_left_bound = sqrt(size);
        row_right_bound = 2 * sqrt(size);
    }
    else {
        row_left_bound = 2 * sqrt(size);
        row_right_bound = size;
    }
    
    if (col < sqrt(size)) col_right_bound = sqrt(size);
    else if (col < (2* sqrt(size))) {
        col_left_bound = sqrt(size);
        col_right_bound = 2 * sqrt(size);
    }
    else {
        col_left_bound = 2 * sqrt(size);
        col_right_bound = size;
    }
    
    for(int i = row_left_bound; i < row_right_bound; i++) {
        for(int j = col_left_bound; j < col_right_bound; j++) {
            if (grid.at(i*size + j).value.at(cell_value - 1) == 1) return false;
        }
    }
    
    // 3) check if number exists in its respective row or col already
    for(int i = 0; i < size; i++) {
        if (cell_value == grid.at(row*size + i).value.at(cell_value - 1) == 1) return false;
        if (cell_value == grid.at(i*size + col).value.at(cell_value - 1) == 1) return false;
    }
    
    grid.at(row*size + col) = point;
    blanks--;
    return true;
}

void SudokuGrid::print_grid() {
    for(int index = 0; index < grid.size(); index++) {
        if(index % (size) == 0) cout << "\n";
        if(grid.at(index).is_singleton()) {
            cout << grid.at(index).possible_values().at(0) << " ";
        }
        else cout << "0 ";
    }
    cout << endl;
}

SudokuGrid::~SudokuGrid() { }