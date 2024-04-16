#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>

class TreeNode {
public:
    TreeNode(const std::string& name, int weight)
        : name(name), weight(weight) {}

    void addChild(TreeNode* child) {
        children.push_back(child);
    }

    std::string getName() const {
        return name;
    }

    int getWeight() const {
        return weight;
    }

    const std::vector<TreeNode*>& getChildren() const {
        return children;
    }

private:
    std::string name;
    int weight;
    std::vector<TreeNode*> children;
};

#endif // TREE_NODE_H