#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "tree_node.h"
#include <map>

class ParseTree {
public:
    ParseTree() : root(nullptr) {}

    void addNode(const std::string& name, int weight, const std::string& parentName) {
        TreeNode* node = new TreeNode(name, weight);
        if (parentName.empty()) {
            root = node;
        } else {
            TreeNode* parent = nodes[parentName];
            parent->addChild(node);
        }
        nodes[name] = node;
    }

    void printTree() const {
        printTree(root, 0);
    }

private:
    TreeNode* root;
    std::map<std::string, TreeNode*> nodes;

    void printTree(const TreeNode* node, int depth) const {
        for (int i = 0; i < depth; ++i) {
            std::cout << "  ";
        }
        std::cout << node->getName() << " (" << node->getWeight() << ")\n";
        for (const TreeNode* child : node->getChildren()) {
            printTree(child, depth + 1);
        }
    }
};

#endif // PARSE_TREE_H