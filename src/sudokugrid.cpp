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

SudokuGrid::SudokuGrid() : size(0) { }

SudokuGrid::SudokuGrid(const int size, const string file) : 
        size(size) {
    // i'm a parent!
    node_status.depth = 0;
    node_status.path_cost = 0;
    
    ifstream infile(file);
    if(infile.fail()) exit_from_error(1);
    if(size < 0) exit_from_error(2);
    
    int square_root(round(sqrt(size)));
    if (size == square_root * square_root) grid.resize(size * size);
    else exit_from_error(3);
    
    char next;
    int row(0), col(0), cell_value(0);
    
    while(!infile.eof()) {
        infile.get(next);
        if(isspace(next)) {
            col = 0;
            row++;
        }
        else {
            if (row == size || col == size) break;
            cell_value = (int)next - (int)'0';
            
            grid.at(index(row, col)).value.resize(size, true);
            
            if (cell_value == 0) unsolved.push_back(index(row, col));
            else {
                bool rc = valid_reduction(index(row, col), cell_value);
                if (!rc) exit_from_error(4);
                grid.at(index(row, col)).reduce_all_except(cell_value);
            }
            col++;
        }
    }
    row = col = 0;
    
    infile.close();
    cout << "Grid initialized:\n";
    print_grid();
}

SudokuGrid::~SudokuGrid() { } 

vector<Point> SudokuGrid::get_node_state() const {
    return grid;
}

void SudokuGrid::node_expansion() { }

int SudokuGrid::index(const int row, const int col) {
    return row * size + col;
}

bool SudokuGrid::valid_reduction(const int index, const int cell_value) {
    // check row & column
    int traversed(0), row((index / size) * size), col(index % size);
    for(; traversed < size; row++, col+=size, traversed++) {
        if(grid.at(row).is_singleton())
            if(grid.at(row).possible_values().at(0) == cell_value) 
                return false;
        if(grid.at(col).is_singleton())
            if(grid.at(col).possible_values().at(0) == cell_value) 
                return false;
    }
    // check each subgrid
    int row_left_bound(0), row_right_bound(0), col_left_bound(0), col_right_bound(0);
    if((index % size) < sqrt(size)) row_right_bound = sqrt(size);
    else if((index % size) < (2* sqrt(size))) {
        row_left_bound = sqrt(size);
        row_right_bound = 2 * sqrt(size);
    }
    else {
        row_left_bound = 2 * sqrt(size);
        row_right_bound = size;
    }
    
    if((index / size) < sqrt(size)) col_right_bound = sqrt(size);
    else if((index / size) < (2* sqrt(size))) {
        col_left_bound = sqrt(size);
        col_right_bound = 2 * sqrt(size);
    }
    else {
        col_left_bound = 2 * sqrt(size);
        col_right_bound = size;
    }
    for(int i = row_left_bound; i < row_right_bound; i++) {
        for(int j = col_left_bound; j < col_right_bound; j++) {
            // cout << "I: " << index << " " << i * size + j << " [" << i << ", " << j << "]; V: " << cell_value << "\n";
            if(grid.at(j * size + i).is_singleton())
                if(grid.at(j * size + i).possible_values().at(0) == cell_value)
                    return false;
        }
    }
    return true;
}

void SudokuGrid::exit_from_error(const int ret) {
    switch(ret) {
        case 1: cout << "Failed to open file.\n"; break;
        case 2: cout << "Grid size cannot be less than 0.\n"; break;
        case 3: cout << "Grid size is not a perfect square.\n"; break;
        case 4: cout << "Incorrect value placement. Please check file.\n"; break;
        default: cout << "Undetermined error.\n"; break;
    }
    exit(-1);
}

void SudokuGrid::print_grid() {
    for(int i(0); i < grid.size(); i++) {
        if(i % size == 0) cout << endl;
        if(grid.at(i).is_singleton())
            cout << grid.at(i).possible_values().at(0) << " ";
        else cout << "0 ";
    }
    cout << endl;
}