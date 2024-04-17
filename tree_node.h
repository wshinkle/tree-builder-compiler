#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <string>
#include <vector>
#include <map>

using namespace std;

class TreeNode {
public:
    TreeNode(const string& name, int weight, const string& parentName = "")
        : name(name), weight(weight) {}

    void addChild(TreeNode* child) {children.push_back(child);}

    string getName() const {return name;}
    int getWeight() const {return weight;}
    string getParentName() const {return parentName;}
    const vector<TreeNode*>& getChildren() const {return children;}

    virtual void execute() {}
private:
    string name;
    string parentName;
    int weight;
    vector<TreeNode*> children;
};

class ForLoopNode : public TreeNode {
public:
    ForLoopNode(const std::string& var, int start, int end, const std::vector<TreeNode*>& statements)
        : TreeNode("", 0), var(var), start(start), end(end), statements(statements) {}

    virtual void execute() {
        for (int i = start; i <= end; i++) {
            // Set the value of the loop variable
            variables[var] = i;

            // Execute each statement in the loop
            for (TreeNode* statement : statements) {
                // Assuming each TreeNode has an execute method
                statement->execute();
            }
        }
    }

private:
    std::string var;
    int start;
    int end;
    std::map<std::string, int> variables;
    std::vector<TreeNode*> statements;
};


class Expr {
public:
    virtual ~Expr() {}
    virtual std::string eval() = 0;
};

class StringExpr : public Expr {
    std::string value;
public:
    StringExpr(const std::string& value) : value(value) {}
    std::string eval() override {
        return value;
    }
};

class ConcatExpr : public Expr {
    Expr* left;
    Expr* right;
public:
    ConcatExpr(Expr* left, Expr* right) : left(left), right(right) {}
    ~ConcatExpr() override {
        delete left;
        delete right;
    }
    std::string eval() override {
        return left->eval() + right->eval();
    }
};

class IntExpr : public Expr {
    int value;
public:
    IntExpr(int value) : value(value) {}
    std::string eval() override {
        return std::to_string(value);
    }
};

class AddExpr : public Expr {
    Expr* left;
    Expr* right;
public:
    AddExpr(Expr* left, Expr* right) : left(left), right(right) {}
    ~AddExpr() override {
        delete left;
        delete right;
    }
    std::string eval() override {
        int leftVal = std::stoi(left->eval());
        int rightVal = std::stoi(right->eval());
        return std::to_string(leftVal + rightVal);
    }
};

#endif // TREE_NODE_H