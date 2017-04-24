#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <string>
#include <vector>
#include "point.h"
#include "treenode.h"

using namespace std;

class SudokuGrid : public TreeNode {
    public:
        SudokuGrid();
        SudokuGrid(const vector<Point>);
        SudokuGrid(const int, const string);
        SudokuGrid(const int, const int, const int, const vector<int>, const vector<Point>);
        ~SudokuGrid();
        
        vector<Point> get_node_state() const;
        void node_expansion();
        
        bool operator==(const SudokuGrid&) const;
        
        int index(const int, const int);
        int min_possible_values();
        
        void reduce(const int, const int);
        bool valid_reduction(const int, const int);
        void compute_key_value();
        
        void solve();
        void exit_from_error(const int);
        void diff_and_print_grid(SudokuGrid, SudokuGrid);
        void print_grid();
        
    private:
        int size;
        vector<int> unsolved;
        vector<Point> grid;
};

#endif