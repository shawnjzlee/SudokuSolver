#include <algorithm>
#include <iostream>
#include <numeric>
#include "point.h"

using namespace std;

Point::Point() : isolated(false) { }

bool Point::operator==(const Point& rhs) const {
    return this->value == rhs.value;
}

bool Point::is_singleton() {
    return value.count() == 1 ? true : false;
}

void Point::reduce(const int cell_value) {
    if(!is_singleton())
        value[cell_value - 1] = false;
}

void Point::reduce_all_except(const int cell_value) {
    value[cell_value - 1] = false;
    value.flip();
}

void Point::isolate(const int cell_value) {
    bset temp;
    temp[cell_value - 1] = true;
    isolated = true;
    value = temp;
}

vector<int> Point::possible_values() {
    vector<int> values;
    for(int i = 0; i < value.size(); i++) {
        if(value[i] == true) {
            values.push_back(i + 1);
        }
    }
    return values;
}