#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>

using namespace std;

class TreeNode {
public:
    TreeNode(const string& name, int weight, const string& parentName = "")
        : name(name), weight(weight) {}

    void addChild(TreeNode* child) {
        children.push_back(child);
    }

    string getName() const {
        return name;
    }

    int getWeight() const {
        return weight;
    }

    string getParentName() const {
        return parentName;
    }
    const vector<TreeNode*>& getChildren() const {
        return children;
    }

private:
    string name;
    string parentName;
    int weight;
    vector<TreeNode*> children;
};

#endif // TREE_NODE_H