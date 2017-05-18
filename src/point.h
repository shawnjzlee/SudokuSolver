#ifndef POINT_H
#define POINT_H

#include <bitset>
#include <vector>

using namespace std;

typedef bitset<16> bset;

struct Point {
    bset value;
    bool isolated;
    
    Point();
    Point(const int, const int);
    
    bool operator==(const Point&) const;
    
    bool is_singleton();
    int bin_to_int();
    void reduce(const int);
    void reduce_all_except(const int);
    void isolate(const int);
    vector<int> possible_values();
    void print_possible_values();
};

#endif