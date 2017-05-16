#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <utility>
#include <queue>
#include <deque>
#include <set>
#include <thread>
#include <mutex>

#include "sudokugrid.h"
#include "global.h"

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
            
            grid.at(index(row, col)).value.set();
            
            if (cell_value == 0) {
                unsolved.emplace(index(row, col));
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

SudokuGrid::SudokuGrid(const int depth, const int path_cost, const int size, const set<int> unsolved, const vector<Point> grid) :
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

bool SudokuGrid::operator==(const SudokuGrid& rhs) const {
    return this->grid == rhs.grid;
}

int SudokuGrid::index(const int row, const int col) {
    return row * size + col;
}

void SudokuGrid::reduce(const int index, const int cell_value) {
    int traversed(0), row((index / size) * size), col(index % size);
    for(; traversed < size; row++, col+=size, traversed++) {
        grid.at(row).reduce(cell_value);
        grid.at(col).reduce(cell_value);
    }
    // check each subgrid
    int row_left_bound(0), row_right_bound(0), col_left_bound(0), col_right_bound(0);
    if((index / size) < sqrt(size)) row_right_bound = sqrt(size);
    else if((index / size) < (2* sqrt(size))) {
        row_left_bound = sqrt(size);
        row_right_bound = 2 * sqrt(size);
    }
    else {
        row_left_bound = 2 * sqrt(size);
        row_right_bound = size;
    }
    
    if((index % size) < sqrt(size)) col_right_bound = sqrt(size);
    else if((index % size) < (2* sqrt(size))) {
        col_left_bound = sqrt(size);
        col_right_bound = 2 * sqrt(size);
    }
    else {
        col_left_bound = 2 * sqrt(size);
        col_right_bound = size;
    }
    for(int i = row_left_bound; i < row_right_bound; i++) {
        for(int j = col_left_bound; j < col_right_bound; j++) {
            grid.at(i * size + j).reduce(cell_value);
        }
    }
}

bool SudokuGrid::valid_reduction(const int index, const int cell_value) {
    // check row & column
    int traversed(0), row((index / size) * size), col(index % size);
    for(; traversed < size; row++, col+=size, traversed++) {
        if(grid.at(row).is_singleton() && (row != index))
            if(grid.at(row).possible_values().at(0) == cell_value) {
                #ifdef VERBOSE
                cout << "\033[2;33mwarning (row check): \033[0mcell value " << cell_value 
                     << " invalid due to conflict with row " << row << endl;
                #endif
                return false;
            }
        if(grid.at(col).is_singleton() && (col != index))
            if(grid.at(col).possible_values().at(0) == cell_value) {
                #ifdef VERBOSE
                cout << "\033[2;33mwarning (col check): \033[0mcell value " << cell_value 
                     << " invalid due to conflict with col " << col << endl;
                #endif
                return false;
            }
    }
    // check each subgrid
    int row_left_bound(0), row_right_bound(0), col_left_bound(0), col_right_bound(0);
    if((index / size) < sqrt(size)) row_right_bound = sqrt(size);
    else if((index / size) < (2* sqrt(size))) {
        row_left_bound = sqrt(size);
        row_right_bound = 2 * sqrt(size);
    }
    else {
        row_left_bound = 2 * sqrt(size);
        row_right_bound = size;
    }
    
    if((index % size) < sqrt(size)) col_right_bound = sqrt(size);
    else if((index % size) < (2* sqrt(size))) {
        col_left_bound = sqrt(size);
        col_right_bound = 2 * sqrt(size);
    }
    else {
        col_left_bound = 2 * sqrt(size);
        col_right_bound = size;
    }
    for(int i = row_left_bound; i < row_right_bound; i++) {
        for(int j = col_left_bound; j < col_right_bound; j++) {
            if(grid.at(i * size + j).is_singleton() && ((i * size + j) != index))
                if(grid.at(i * size + j).possible_values().at(0) == cell_value) {
                    #ifdef VERBOSE
                    cout << "\033[2;33mwarning (subgrid check): \033[0mcell value " << cell_value 
                         << " invalid at index " << index << endl;
                    #endif
                    return false;
                }
        }
    }
    return true;
}

bool SudokuGrid::valid_grid() {
    // check row & column
    vector<int> row_values, col_values;
    for(int i = 0; i < grid.size(); i+=size) {
        for(int j = 0; j < size; j++) {
            #ifdef TEST
            cout << i + j << "  ";
            #endif
            row_values.push_back(i + j);
        }
        sort(row_values.begin(), row_values.end());
        if(!(unique(row_values.begin(), row_values.end()) == row_values.end())) return false;
        row_values.clear();
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < grid.size(); j+=size) {
            #ifdef TEST
            cout << i + j << "  ";
            #endif
            col_values.push_back(i + j);
        }
        sort(col_values.begin(), col_values.end());
        if(!(unique(col_values.begin(), col_values.end()) == col_values.end())) return false;
        col_values.clear();
    }
    // check each subgrid
    vector<int> subgrid1, subgrid2, subgrid3;
    for(int offset = 0; offset < size; offset+=3) {
        for(int i = 0; i < sqrt(size); i++) {
            for(int j = 0; j < sqrt(size); j++) {
                if(grid.at((i + offset) * size + j).is_singleton()) {
                    subgrid1.push_back(grid.at((i + offset) * size + j).possible_values().at(0));
                }
                if(grid.at((i + offset) * size + (j + 3)).is_singleton()) {
                    subgrid2.push_back(grid.at((i + offset) * size + (j + 3)).possible_values().at(0));
                }
                if(grid.at((i + offset) * size + (j + 6)).is_singleton()) {
                    subgrid3.push_back(grid.at((i + offset) * size + (j + 6)).possible_values().at(0));
                }
            }
        }
        sort(subgrid1.begin(), subgrid1.end());
        sort(subgrid2.begin(), subgrid2.end());
        sort(subgrid3.begin(), subgrid3.end());
        
        {
        #ifdef TEST
        if(!(unique(subgrid1.begin(), subgrid1.end()) == subgrid1.end())) {
            cout << "Subgrid 1 returned false:\n";
            for(auto i : subgrid1) cout << i << "\t";
            return false;
        }
        if(!(unique(subgrid2.begin(), subgrid2.end()) == subgrid2.end())) {
            cout << "Subgrid 2 returned false:\n";
            for(auto &i : subgrid2) cout << i << "\t";
            return false;
        }
        if(!(unique(subgrid3.begin(), subgrid3.end()) == subgrid3.end())) {
            cout << "Subgrid 3 returned false:\n";
            for(auto &i : subgrid3) cout << i << "\t";
            return false;
        }
        #endif
        }
        
        if(!(unique(subgrid1.begin(), subgrid1.end()) == subgrid1.end()) ||
           !(unique(subgrid2.begin(), subgrid2.end()) == subgrid2.end()) ||
           !(unique(subgrid3.begin(), subgrid3.end()) == subgrid3.end()))
                return false;
        
        subgrid1.clear(); subgrid2.clear(); subgrid3.clear();
    }
    return true;
}

int SudokuGrid::min_possible_values() {
    if(unsolved.size() == 0) exit_from_error(5);
    else if(unsolved.size() == 1) {
        return *unsolved.begin();
    }
    else {
        vector<int> cardinality_possible_values, key;
        for(const auto &i : unsolved) {
            cardinality_possible_values.push_back(grid.at(i).possible_values().size());
            key.push_back(i);
        }
        auto result = min_element(cardinality_possible_values.begin(), cardinality_possible_values.end());
        int key_value = key.at(distance(cardinality_possible_values.begin(), result));
        
        return key_value;
    }
}

int SudokuGrid::max_possible_values() {
    if(unsolved.size() == 0) exit_from_error(5);
    else if(unsolved.size() == 1) {
        return *unsolved.begin();
    }
    else {
        vector<int> cardinality_possible_values, key;
        for(const auto &i : unsolved) {
            cardinality_possible_values.push_back(grid.at(i).possible_values().size());
            key.push_back(i);
        }
        auto result = max_element(cardinality_possible_values.begin(), cardinality_possible_values.end());
        int key_value = key.at(distance(cardinality_possible_values.begin(), result));
        
        return key_value;
    }
}

string SudokuGrid::get_unique_key() const {
    string bitstring;
    for(auto cell : grid) bitstring.append(cell.value.to_string());
    return bitstring;
}

void SudokuGrid::solve(set<SudokuGrid, PossibleValueCmp>& expanded) {
    cout << "Thread " << this_thread::get_id() << " has spawned...\n";
    auto check = [](SudokuGrid child, set<SudokuGrid, PossibleValueCmp> expanded) {
        string key = child.get_unique_key();
        {
            lock_guard<mutex> lock(mutex_expanded_set);
            auto search = expanded.find(child);
            if (search == expanded.end()) return true;
            else {
                #ifdef VERBOSE
                cout << "\033[2;33mwarning: \033[0mduplicate grid found\n";
                #endif
                return false;
            }
        }
    };
    
    auto expanded_node = [](SudokuGrid child, int unsolved_index) {
        child.unsolved.erase(find(child.unsolved.begin(), child.unsolved.end(), unsolved_index));
        return child;
    };
    
    queue<SudokuGrid> fringe;
    
    SudokuGrid parent_node(0, 0, size, unsolved, grid);
    parent_node.get_unique_key();
    fringe.push(parent_node);
    
    int max_queued_nodes = 0;
    
    while(!fringe.empty()) {
        parent_node = fringe.front();
        fringe.pop();
        
        #ifdef VERBOSE
        cout << "Number of unsolved cells: " << parent_node.unsolved.size() << "\n";
        #endif
        
        if(parent_node.unsolved.empty()) {
            g_solution_found = true;
            cout << "\n\nSolution found:\n";
            {
                lock_guard<mutex> lock(mutex_print_grid);
                parent_node.print_grid();
            }
            cout << "\nStates expanded: " << expanded.size();
            cout << "\nDepth: " << parent_node.node_status.depth << endl;
            return;
        }
        
        SudokuGrid child_node(parent_node.node_status.depth + 1,
                              parent_node.node_status.path_cost + 1,
                              parent_node.size,
                              parent_node.unsolved,
                              parent_node.grid);

        bool invalid_singleton = false;
        int unsolved_index = child_node.min_possible_values();
        // REDUCE ALL SINGLETONS IN THIS INSTANCE
        while (child_node.grid.at(unsolved_index).is_singleton()) {
            vector<Point> prev = child_node.grid;
            int cell_value = child_node.grid.at(unsolved_index).possible_values().at(0);
            
            #ifdef VERBOSE
            cout << "\nIsolating " << cell_value << " at " << unsolved_index << endl;
            #endif
            
            child_node.grid.at(unsolved_index).isolate(cell_value);
            child_node.reduce(unsolved_index, cell_value);
            
            if(!child_node.valid_grid()) {
                #ifdef VERBOSE
                diff_and_print_grid(parent_node, child_node);
                cout << "\nGrid is invalid, emplacing previous grid \n";
                #endif
                
                child_node.grid = prev;
                
                #ifdef VERBOSE
                child_node.print_grid();
                #endif
                
                {
                    lock_guard<mutex> lock(mutex_expanded_set);
                    expanded.emplace(child_node);
                }
                invalid_singleton = true;
                break;
            }
            child_node = expanded_node(child_node, unsolved_index);
            
            if(child_node.unsolved.empty()) break;
            
            #ifdef VERBOSE
            cout << "Exited expanded_node: " << child_node.unsolved.size() << endl;
            diff_and_print_grid(parent_node, child_node);
            #endif
            
            unsolved_index = child_node.min_possible_values();
        }
        
        if(invalid_singleton) continue;
        if(child_node.unsolved.empty()) {
            g_solution_found = true;
            cout << "\n\nSolution found:\n";
            {
                lock_guard<mutex> lock(mutex_print_grid);
                child_node.print_grid();
            }
            cout << "\nStates expanded: " << expanded.size();
            cout << "\nDepth: " << parent_node.node_status.depth << endl;
            return;
        }
        
        // BRANCHING FACTOR
        vector<Point> prev = child_node.grid;
        for(auto cell_value : child_node.grid.at(unsolved_index).possible_values()) {
            if(!valid_reduction(unsolved_index, cell_value)) {
                #ifdef VERBOSE
                cout << "In branching, cell value " << cell_value << " is invalid at " << unsolved_index << endl;
                diff_and_print_grid(parent_node, child_node);
                #endif
                {
                    lock_guard<mutex> lock(mutex_expanded_set);
                    expanded.emplace(expanded_node(child_node, unsolved_index));
                }
                continue;
            }
            
            child_node.grid.at(unsolved_index).isolate(cell_value);
            child_node.reduce(unsolved_index, cell_value);
            
            #ifdef VERBOSE
            cout << "\nBranching...\n";
            // cout << "Expanded queue size: " << expanded.size() << endl;
            // cout << "Fringe queue size: " << fringe.size() << endl;
            // cout << "Unsolved vector size: " << child_node.unsolved.size() << endl;
            cout << "Modifying index " << unsolved_index << " (" << unsolved_index % (size - 1) << "," << unsolved_index / (size - 1) << ")\n";
            cout << "Appending value: " << cell_value << endl;
            diff_and_print_grid(parent_node, child_node);
            #endif
            
            if(check(child_node, expanded)) {
                fringe.push(expanded_node(child_node, unsolved_index));
                lock_guard<mutex> lock(mutex_expanded_set);
                expanded.emplace(expanded_node(child_node, unsolved_index));
            }
            child_node.grid = prev;
        }
        
        child_node.grid = parent_node.grid;
        if(fringe.size() > max_queued_nodes)
            max_queued_nodes = fringe.size();
    }
    // exit_from_error(6);
}

void SudokuGrid::thread_distribution(int num_threads) {
    auto fetch = [](SudokuGrid& node) {
        int unsolved_index = node.max_possible_values();
        vector<int> temp = node.grid.at(unsolved_index).possible_values();
        deque<int> possible_values(temp.begin(), temp.end());
        node.unsolved.erase(find(node.unsolved.begin(), node.unsolved.end(), unsolved_index));
        
        return make_pair(unsolved_index, possible_values);
    };
    
    set<SudokuGrid, PossibleValueCmp> expanded;
    
    SudokuGrid parent_node(0, 0, size, unsolved, grid);
    parent_node.get_unique_key();
    
    pair<int, deque<int>> dist = fetch(parent_node);
    int unsolved_index = dist.first;
    deque<int> possible_values = dist.second;
    
    if(num_threads > thread::hardware_concurrency()) {
        num_threads = thread::hardware_concurrency();
        cout << "\033[2;33mwarning:\033[0m too many threads requested. Spawning "
             << thread::hardware_concurrency() << " threads.\n";
    }
    else cout << "Spawning " << num_threads << " threads.\n";
    
    vector<thread> threads;
    while(!g_solution_found) {
        if(possible_values.empty()) {
            dist = fetch(parent_node);
            unsolved_index = dist.first;
            possible_values = dist.second;
        }        
        while(threads.size() > num_threads) {
            // join threads that finished work, but solution is not found
            for(auto i = threads.begin(); i < threads.end(); i++) {
                if(i->joinable()) {
                    cout << "Thread " << i->get_id() << " has returned and joined...\n";
                    i->join();
                    threads.erase(i);
                }
            }
        }
        
        int cell_value = possible_values.front();
        possible_values.pop_front();
        
        parent_node.grid.at(unsolved_index).isolate(cell_value);
        parent_node.reduce(unsolved_index, cell_value);
        
        {
            lock_guard<mutex> lock(mutex_expanded_set);
            expanded.emplace(parent_node);
        }
        
        // #ifdef VERBOSE
        cout << "Initializing with max_value index " << unsolved_index << endl;
        cout << "Possible values: ";
        for(const auto i : possible_values) cout << i << " ";
        cout << "\n";
        // #endif
        
        threads.push_back(thread(&SudokuGrid::solve, this, ref(expanded)));
    }
    
    for_each(threads.begin(), threads.end(), mem_fn(&thread::join));
}


void SudokuGrid::exit_from_error(const int ret) {
    switch(ret) {
        case 1: cout << "Failed to open file.\n"; break;
        case 2: cout << "Grid size cannot be less than 0.\n"; break;
        case 3: cout << "Grid size is not a perfect square.\n"; break;
        case 4: cout << "Incorrect value placement. Please check file.\n"; break;
        case 5: cout << "Did not return any unsolved cells.\n"; print_grid(); break;
        case 6: cout << "Fringe is empty.\n"; break;
        default: cout << "Undetermined error.\n"; break;
    }
    exit(-1);
}

void SudokuGrid::diff_and_print_grid(SudokuGrid parent_node, SudokuGrid child_node) {
    for(int i(0); i < parent_node.grid.size(); i++) {
        int parent_node_cv(0), child_node_cv(0);
        if(i % size == 0) cout << endl;
        if(parent_node.grid.at(i).is_singleton()) parent_node_cv = parent_node.grid.at(i).possible_values().at(0);
        if(child_node.grid.at(i).is_singleton()) child_node_cv = child_node.grid.at(i).possible_values().at(0);
        
        // cout << parent_node_cv << "\t" << child_node_cv << endl << endl;
        
        if(parent_node_cv != child_node_cv) cout << "\033[2;34m" << child_node_cv << "\033[0m ";
        else if(child_node.grid.at(i).isolated == true) cout << "\033[2;31m" << child_node_cv << "\033[0m ";
        else if(parent_node_cv == child_node_cv) cout << child_node_cv << " ";
        else cout << "0 ";
    }
    cout << endl;
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