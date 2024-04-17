#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "tree_node.h"
#include <map>
#include <iostream>

using namespace std;

class ParseTree {
public:
    ParseTree() : root(nullptr) {}

    TreeNode* getRoot() const {return root;}
    
    void addNode(TreeNode* node) {
        if (root == nullptr) {
            root = node;
        } 
        if (node->getParentName() != "") {
            TreeNode* parent = findNode(node->getParentName());
            if (parent != nullptr) {
                parent->addChild(node);
            }
        }
        nodes.push_back(node);
    }

    TreeNode* findNode(const string& name) const {
        for (const auto& node : nodes) {
            if (node->getName() == name) {
                return node;
            }
        }
        return nullptr;
    }

    void printTree(const TreeNode* node = nullptr, bool isRoot = true) const {
        if (node == nullptr) {
            node = root;
        }
        cout << "(<" << node->getName() << "," << node->getWeight() << ">";
        const auto& children = node->getChildren();
        for (int i = children.size() - 1; i >= 0; i--) {
            cout << ",";
            printTree(children[i], false);
        }
        cout << ")";
        if (isRoot) {
            cout << endl;
        }
    }

    void printNodes() const {
        for (const auto& node : nodes) {
            cout << "Name: " << node->getName() << " "
            << "Parent: " << node->getParentName() << endl;
            
        }
        cout << endl;
    }

private:
    TreeNode* root;
    vector<TreeNode*> nodes;
};
#endif // PARSE_TREE_H