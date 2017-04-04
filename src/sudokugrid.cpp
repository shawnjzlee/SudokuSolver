#include <iostream>
#include <cmath>

#include "sudokugrid.h"

using namespace std;

SudokuGrid::SudokuGrid(int size) {
    if (size < 0) {
        cout << "Grid size cannot be less than 0\n";
        return;
    }
    int square_root(round(sqrt(size)));
    if (size == square_root * square_root) {
        grid.resize(size);
        for(auto &i : grid) i.resize(size);
    }
    else {
        cout << "Grid size is not a perfect square\n";
        return;
    }
}

SudokuGrid::~SudokuGrid() { }