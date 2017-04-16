#ifndef POINT_H
#define POINT_H

#include <vector>

using namespace std;

struct Point {
    int row;
    int col;
    vector<bool> value;    
    
    Point();
    Point(int, int, int, int);
    
    bool is_singleton();
    vector<int> possible_values();
};

#endif