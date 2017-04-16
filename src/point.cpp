#include "point.h"

using namespace std;

Point::Point() : row(0), col(0) { 
    value.resize(9, 0);
};

Point::Point(int row, int col, int size, int cell_value) : 
        row(row), col(col) { 
    value.resize(size, 0);
    if(cell_value > 0) value.at(cell_value - 1) = 1;
}

bool Point::is_singleton() {
    int count = 0;
    for(auto i : value) {
        if(i == 1) count++;
        if(count > 1) return false;
    }
    if(count == 0) return false;
    return true; 
}

vector<int> Point::possible_values() {
    vector<int> values;
    for(int i = 0; i < value.size(); i++) {
        if(value.at(i) == 1) values.push_back(i + 1);
    }
    return values;
}