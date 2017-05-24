#ifndef SUDOKUGRID_H
#define SUDOKUGRID_H

#include <string>
#include <vector>
#include <set>
#include "point.h"
#include "treenode.h"

using namespace std;

class SudokuGrid : public TreeNode {
    public: 
        struct PossibleValueCmp {
            // bool operator< , bool operator=
            bool operator()(const SudokuGrid& lhs, const SudokuGrid& rhs) const {
                string left = lhs.get_unique_key();
                string right = rhs.get_unique_key();
                return left < right;
            }
        };
    public:
        SudokuGrid();
        SudokuGrid(const vector<Point>);
        SudokuGrid(const int, const string);
        SudokuGrid(const int, const int, const int, const set<int>, const vector<Point>);
        ~SudokuGrid();
        
        vector<Point> get_node_state() const;
        void node_expansion();
        
        bool operator==(const SudokuGrid&) const;
        
        int index(const int, const int);
        int min_possible_values();
        int max_possible_values();
        
        void reduce(const int, const int);
        bool valid_grid();
        bool valid_reduction(const int, const int);
        string get_unique_key() const;
        
        void solve(set<SudokuGrid, PossibleValueCmp>&);
        void thread_distribution(int, bool);
        void exit_from_error(const int);
        void diff_and_print_grid(SudokuGrid, SudokuGrid);
        void print_grid();
        
    private:
        int size;
        set<int> unsolved;
        vector<Point> grid;
};

#endif