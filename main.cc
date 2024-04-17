#include "parse_tree.h"

int main() {
    ParseTree tree;
    TreeNode root("root", 5, "");
    TreeNode A("A", 2, "root");
    TreeNode B("B", 2, "root");
    // Add root node
    tree.addNode(&root);
    tree.addNode(&A);
    tree.addNode(&B);

    // Print the tree
    tree.printTree(
        tree.getRoot(), true
    );

    return 0;
}