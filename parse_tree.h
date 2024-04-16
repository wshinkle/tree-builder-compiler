#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "tree_node.h"
#include <map>
#include <iostream>

using namespace std;

class ParseTree {
public:
    ParseTree() : root(nullptr) {}

    void addNode(const string& name, int weight, const string& parentName) {
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
        printTree(root);
        cout << endl;
    }

private:
    TreeNode* root;
    map<string, TreeNode*> nodes;

    //this prints in the correct format and should be done
    void printTree(const TreeNode* node) const {
        if (node == nullptr) {
            return;
        }
        cout << "(<" << node->getName() << "," << node->getWeight() << ">";
        for (const TreeNode* child : node->getChildren()) {
            cout << ",";
            printTree(child);
        }
        cout << ")";
    }
};

#endif // PARSE_TREE_H