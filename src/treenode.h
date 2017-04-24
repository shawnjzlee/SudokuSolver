#ifndef TREENODE_H
#define TREENODE_H

#include <vector>

using namespace std;

class TreeNode {
    public:
        struct node_status {
            int depth;
            int path_cost;
        }; node_status node_status;
        
        virtual vector<Point> get_node_state() const = 0;
        virtual void node_expansion() = 0;
};

#endif