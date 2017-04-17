#ifndef POINT_H
#define POINT_H

#include <vector>

using namespace std;

struct Point {
    vector<bool> value;    
    
    Point();
    Point(const int, const int);
    
    bool is_singleton();
    void reduce(const int);
    void reduce_all_except(const int);
    vector<int> possible_values();
};

#endif