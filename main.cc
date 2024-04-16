#include "parse_tree.h"

int main() {
    ParseTree tree;

    // Add root node
    tree.addNode("root", 1, "");

    // Add children to root
    tree.addNode("child1", 2, "root");
    tree.addNode("child2", 3, "root");

    // Add grandchildren
    tree.addNode("grandchild1", 4, "child1");
    tree.addNode("grandchild2", 5, "child2");

    // Print the tree
    tree.printTree();

    return 0;
}