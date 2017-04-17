#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <queue>

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
            
            if (cell_value == 0) {
                unsolved.push_back(index(row, col));
            }
            else {
                bool rc = valid_reduction(index(row, col), cell_value);
                if (!rc) exit_from_error(4);
                grid.at(index(row, col)).reduce_all_except(cell_value);
            }
            col++;
        }
    }
    row = col = 0;
    
    cout << "Grid initialized:\n";
    print_grid();
    
    for(int index = 0; index < grid.size(); index++)
        if(grid.at(index).is_singleton())
            reduce(index, grid.at(index).possible_values().at(0));
    
    infile.close();
}

SudokuGrid::SudokuGrid(const vector<Point> grid) {
    this->grid = grid;
}

SudokuGrid::SudokuGrid(const int depth, const int path_cost, const int size, const vector<int> unsolved, const vector<Point> grid) :
        size(size) {
    // i'm a child!
    this->node_status.depth = depth;
    this->node_status.path_cost = path_cost;
    this->unsolved = unsolved;
    this->grid = grid;
}

SudokuGrid::~SudokuGrid() { } 

vector<Point> SudokuGrid::get_node_state() const {
    return grid;
}

void SudokuGrid::node_expansion() { }

int SudokuGrid::index(const int row, const int col) {
    return row * size + col;
}

void SudokuGrid::reduce(const int index, const int cell_value) {
    int traversed(0), row((index / size) * size), col(index % size);
    for(; traversed < size; row++, col+=size, traversed++) {
        grid.at(row).reduce(cell_value);
        grid.at(col).reduce(cell_value);
    }
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
            if(grid.at(j * size + i).is_singleton())
                if(grid.at(j * size + i).possible_values().at(0) == cell_value)
                    return false;
        }
    }
    return true;
}

int SudokuGrid::min_possible_values() {
    if(unsolved.size() == 0) exit_from_error(5);
    else if(unsolved.size() == 1) return unsolved.front();
    else {
        int curr_min_index(unsolved.front());
        for(auto i : unsolved) {
            // i is an index
            int count = grid.at(i).possible_values().size();
            if (count < grid.at(curr_min_index).possible_values().size()) {
                curr_min_index = i;
            }
        }
        return curr_min_index;
    }
}

void SudokuGrid::solve() {
    auto check = [](SudokuGrid child, queue<SudokuGrid> expanded) {
        SudokuGrid temp(child.grid);
        while(!expanded.empty()) {
            temp = expanded.front();
            expanded.pop();
            if(temp.grid == child.grid) {
                cout << "Not passed\n";
                return false;
            }
        }
        cout << "Passed check\n";
        return true;
    };
    
    auto expanded_node = [](SudokuGrid child, int unsolved_index) {
        child.unsolved.erase(find(child.unsolved.begin(), child.unsolved.end(), unsolved_index));
        return child;
    };
    
    queue<SudokuGrid> fringe;
    queue<SudokuGrid> expanded;
    
    SudokuGrid parent_node(0, 0, size, unsolved, grid);
    fringe.push(parent_node);
    
    int max_queued_nodes = 0;
    
    while(!fringe.empty()) {
        parent_node = fringe.front();
        fringe.pop();
        
        // cout << parent_node.unsolved.size() << "\n";
        
        if(parent_node.unsolved.empty()) {
            cout << "Solution found\n";
            parent_node.print_grid();
            return;
        }
        
        SudokuGrid child_node(parent_node.node_status.depth + 1,
                              parent_node.node_status.path_cost + 1,
                              parent_node.size,
                              parent_node.unsolved,
                              parent_node.grid);
                              
        int unsolved_index = child_node.min_possible_values();
        cout << "UI: " << unsolved_index << endl;
        
        // BRANCHING FACTOR
        for(auto cell_value : child_node.grid.at(unsolved_index).possible_values()) {
            cout << "P: " << child_node.grid.at(unsolved_index).possible_values().size() << endl;
            cout << "I: " << unsolved_index << endl;
            cout << "U: " << child_node.unsolved.size() << endl;
            child_node.grid = parent_node.grid;
            
            child_node.grid.at(unsolved_index).isolate(cell_value);
            child_node.reduce(unsolved_index, cell_value);
            
            cout << "\nBranching:";
            child_node.print_grid();
            
            if(check(child_node, expanded)) {
                cout << "F_B: " << fringe.size() << endl;
                fringe.push(expanded_node(child_node, unsolved_index));
                expanded.push(expanded_node(child_node, unsolved_index));
                cout << "F_A: " << fringe.size() << endl;
                cout << "U_M: " << child_node.unsolved.size() << endl;
            }
        }
        
        child_node.grid = parent_node.grid;
        if(fringe.size() > max_queued_nodes)
            max_queued_nodes = fringe.size();
    }
    
    cout << "Fringe empty. Exiting...\n" << endl;
}


void SudokuGrid::exit_from_error(const int ret) {
    switch(ret) {
        case 1: cout << "Failed to open file.\n"; break;
        case 2: cout << "Grid size cannot be less than 0.\n"; break;
        case 3: cout << "Grid size is not a perfect square.\n"; break;
        case 4: cout << "Incorrect value placement. Please check file.\n"; break;
        case 5: cout << "Returned no unsolved cells.\n"; print_grid(); break;
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