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

SudokuGrid::SudokuGrid() : blanks(0), size(0) { }

void SudokuGrid::init(int size) {
    if (size < 0) {
        cout << "Grid size cannot be less than 0\n";
        exit(-1);
    }
    int square_root(round(sqrt(size)));
    if (size == square_root * square_root) {
        this->size = size;
        grid.resize(size);
        for(auto &col : grid) {
            col.resize(size);
            for(auto &cell : col) cell.resize(size);
        }
    }
    else {
        cout << "Grid size is not a perfect square\n";
        exit(-1);
    }
    cout << "Printing grid...\n";
    print_grid();
    // cout << "Populating grid...\n";
    // populate_grid();
}

void SudokuGrid::init(int size, string file) {
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
    if (size == square_root * square_root) {
        this->size = size;
        grid.resize(size);
        for(auto &col : grid) {
            col.resize(size);
            for(auto &cell : col) cell.resize(size);
        }
    }
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
            else grid.at(row).at(col).at(cell_value - 1) = 1;
            col++;
        }
    }
    row = col = 0;
    infile.close();
    cout << "Grid initialized:\n";
    print_grid();
}

void SudokuGrid::find_potential_val() {
    int sum (0);
    vector<vector<bool>> existing_values (size, vector<bool>(size, 0));
    vector<unsigned short> num_zeros(size, 0);
    for(int i = 0; i < size; i++) sum += i;
    
    for(int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            for(int cell = 0; cell < size; cell++) {
                if(grid[row][col].at(cell) == 1) {
                    existing_values.at(row).at(cell) = 1;
                    break;
                }
                else if((grid[row][col].at(cell) == 0) && (cell == size - 1)) {
                    num_zeros.at(row)++;
                }
            }
        }
    }

    for(int row = 0; row < size; row++) {
        // singleton
        if(num_zeros.at(row) == 1) {
            auto it = find(existing_values.at(row).begin(), existing_values.at(row).end(), 0);
            if (it != end(existing_values.at(row))) {
                for (int col = 0; col < size; col++) {
                    for(int cell = 0; cell < size; cell++) {
                        if(grid[row][col].at(cell) == 1) {
                            break;
                        }
                        else if((grid[row][col].at(cell) == 0) && (cell == size - 1)) {
                            grid[row][col].at(distance(begin(existing_values.at(row)), it)) = 1;
                        }
                    }
                }
            }
        }
    }

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            cout << existing_values[i][j] << " ";   
        }
        cout << " 0s: " << num_zeros[i] << endl;
    }
    
    
    cout << "\nModified grid:\n";
    print_grid();
}

// bool SudokuGrid::add_cell(bool test, int row, int col, int cell_value) {
//     // optimize
    
//     // 1) check if cell is already populated
//     if (find(grid.at(row).at(col).begin(), grid.at(row).at(col).end(), 1) != v.end()) 
//         return false;
        
//     // 2) check valid number within subgrid
//     int row_left_bound(0), row_right_bound(0), col_left_bound(0), col_right_bound(0);
//     if (row < sqrt(size))
//         row_right_bound = sqrt(size);
//     else if (row < (2* sqrt(size))) {
//         row_left_bound = sqrt(size);
//         row_right_bound = 2 * sqrt(size);
//     }
//     else {
//         row_left_bound = 2 * sqrt(size);
//         row_right_bound = size;
//     }
    
//     if (col < sqrt(size))
//         col_right_bound = sqrt(size);
//     else if (col < (2* sqrt(size))) {
//         col_left_bound = sqrt(size);
//         col_right_bound = 2 * sqrt(size);
//     }
//     else {
//         col_left_bound = 2 * sqrt(size);
//         col_right_bound = size;
//     }
    
//     for(int i = row_left_bound; i < row_right_bound; i++) {
//         for(int j = col_left_bound; j < col_right_bound; j++) {
//             if (grid.at(row).at(col).at(cell_value - 1) == 1) return false;
//             // cout << "Access: " << i << "\t" << j << endl;
//         }
//     }
    
//     // 3) check if number exists in its respective row or col already
//     for(int i = 0; i < size; i++) {
//         if (cell_value == grid.at(row).at(i).at(cell_value - 1) == 1) return false;
//         if (cell_value == grid.at(i).at(col).at(cell_value - 1) == 1) return false;
//     }  
//     if (!test) {
//         grid.at(row).at(col).at(cell_value - 1) = 1;
//         blanks--;
        
//         // print_grid();
//     }
//     return true;
// }

// void SudokuGrid::reduce(int row, int col, int cell_value) {
    
// }

// void SudokuGrid::remove_cell(int row, int col) { grid.at(row).at(col) = 0; }

// bool SudokuGrid::solve_grid() {
//     int row (0), col (0);
//     if (!blanks) return true;
    
//     find_next_cell(row, col);
//     for(int i = 1; i < size + 1; i++) {
//         // cout << "[" << row << ", " << col << "]: " << i << endl;
//         if (add_cell(0, row, col, i)) {
//             if (solve_grid()) {
//                 return true;
//             }
//             remove_cell(row, col);
//         }
//     }
//     return false;
// }

// void SudokuGrid::populate_grid() {
//     srand(time(0));
    
//     int cells_to_populate = (rand() % (size * size)) - 1;
    
//     int cell_value (0), row (0), col (0), subgrid (0);
//     bool return_val = false, do_once = true;
//     while(cells_to_populate > 0) {
//         row = rand() % size;
//         col = rand() % size;
//         cell_value = (rand() % size) + 1;
        
//         if(do_once) {
//             cout << cells_to_populate << endl;
//             do_once = false;
//         }
        
//         return_val = add_cell(0, row, col, cell_value);
        
//         // cout << cells_to_populate << ": " << row << " , " << col 
//             //  << " with value " << cell_value << " returned " << return_val << endl;
        
//         // print_grid();
//         // cout << endl << endl;
//         if(return_val) cells_to_populate--;
//     }
//     cout << "Grid initialized: \n";
//     print_grid();
// }

void SudokuGrid::print_grid() {
    for(const auto &col : grid) {
        for(const auto &cell : col) {
            for(int i = 0; i < cell.size(); i++) {
                if (cell.at(i) == 1) cout << i + 1 << " ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

SudokuGrid::~SudokuGrid() { }