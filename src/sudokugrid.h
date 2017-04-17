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
        SudokuGrid(const int, const string);
        ~SudokuGrid();
        
        vector<Point> get_node_state() const;
        void node_expansion();
        
        int index(const int, const int);
        
        bool valid_reduction(const int, const int);
        
        void exit_from_error(const int);
        void print_grid();
        
    private:
        int size;
        vector<int> unsolved;
        vector<Point> grid;
};

#endif