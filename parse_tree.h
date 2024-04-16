#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "tree_node.h"
#include <map>
#include <iostream>

using namespace std;

class ParseTree {
public:
    ParseTree() : root(nullptr) {}

    void addNode(TreeNode* node) {
        printf("Adding node %s\n", node->getName().c_str());
        if (node->getParentName().empty()) {
            root = node;
        } else {
            TreeNode* parent = nodes[node->getParentName()];
            parent->addChild(node);
        }
        nodes[node->getName()] = node;
        nodes2.push_back(node);
    }

    void printTree() const {
        printTree(root);
        cout << endl;
    }
    void printNodes() const {
        for (const auto& node : nodes2) {
            cout << node->getName() << " ";
        }
        cout << endl;
    }
private:
    TreeNode* root;
    map<string, TreeNode*> nodes;
    vector<TreeNode*> nodes2;
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