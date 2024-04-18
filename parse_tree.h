#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include <map>
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <vector>
using namespace std;

class TreeNode
{
public:
  TreeNode(const string name, int weight, const string parentName = "")
  {
    this->name = name;
    this->weight = weight;
    this->parentName = parentName;
  }

  void addChild(TreeNode *child) { children.push_back(child); }

  string getName() const { return name; }
  int getWeight() const { return weight; }
  string getParentName() const { return parentName; }
  void setParentName(string str) { parentName = str; }
  const vector<TreeNode *> &getChildren() const { return children; }

  virtual void execute() {}

private:
  string name;
  string parentName;
  int weight;
  vector<TreeNode *> children;
};

class ParseTree
{
public:
  ParseTree() : root(nullptr) {}

  TreeNode *getRoot() const { return root; }

  void addNode(TreeNode *node)
  {
    if (root == nullptr)
    {
      root = node;
    }
    if (node->getParentName() != "")
    {
      TreeNode *parent = findNode(node->getParentName());
      if (parent != nullptr)
      {
        parent->addChild(node);
      }
    }
    nodes.push_back(node);
  }

  TreeNode *findNode(const string &name) const
  {
    for (const auto &node : nodes)
    {
      if (node->getName() == name)
      {
        return node;
      }
    }
    return nullptr;
  }

  void printTree(const TreeNode *node = nullptr, bool isRoot = true) const
  {
    if (node == nullptr)
    {
      node = root;
    }
    cout << "(<" << node->getName() << "," << node->getWeight() << ">";
    const auto &children = node->getChildren();
    for (int i = children.size() - 1; i >= 0; i--)
    {
      cout << ",";
      printTree(children[i], false);
    }
    cout << ")";
    if (isRoot)
    {
      cout << endl;
    }
  }

  void printNodes() const
  {
    for (const auto &node : nodes)
    {
      cout << "Name: " << node->getName() << " "
           << "Parent: " << node->getParentName() << endl;
    }
    cout << endl;
  }

private:
  TreeNode *root;
  vector<TreeNode *> nodes;
};


struct string_list {
  char * string;
  string_list * next;
};








class expression
{
public:
  virtual string evaluate_expression(map<string, string> &sym_tab) = 0;
};

class variable : public expression
{
public:
  variable(char *in_val)
  { // cout << "Found variable = " << in_val << endl;
    saved_val = in_val;
  }

  virtual string evaluate_expression(map<string, string> &sym_tab)
  {

    map<string, string>::iterator p;
    p = sym_tab.find(saved_val);
    // cout << "Looking up variable " << saved_val << endl;
    if (p != sym_tab.end())
    {
      // cout << "Returning value of variable " << saved_val << endl;
      // cout << "= " << p->second << endl;

      return p->second;
    }
    else
    {
      // ERROR .... for now return -1;
      // Should throw error!
      exit(-1);
      return "-1";
    }
  }

private:
  string saved_val;
};
class string_constant : public expression
{
public:
  string_constant(string val) { saved_val = val; }

  virtual string evaluate_expression(map<string, string> &sym_tab)
  {
    return saved_val;
  }

private:
  string saved_val;
};

class concat_op : public expression
{
public:
  concat_op(expression *l, expression *r)
  {
    left = l;
    right = r;
  }

  virtual string evaluate_expression(map<string, string> &sym_tab)
  {
    string l_val = left->evaluate_expression(sym_tab);
    string r_val = right->evaluate_expression(sym_tab);
    return l_val + r_val;
  }

private:
  expression *left;
  expression *right;
};

class int_expression
{
public:
  virtual int evaluate_expression(map<string, string> &sym_tab) = 0;
};


class int_variable : public int_expression
{
public:
  int_variable(string in_val)
  { // cout << "Found variable = " << in_val << endl;
    saved_val = in_val;
  }

  virtual int evaluate_expression(map<string, string> &sym_tab)
  {

    map<string, string>::iterator p;
    p = sym_tab.find(saved_val);
    // cout << "Looking up variable " << saved_val << endl;
    if (p != sym_tab.end())
    {
      // cout << "Returning value of variable " << saved_val << endl;
      // cout << "= " << p->second << endl;

      return stoi(p->second);
    }
    else
    {
      // ERROR .... for now return -1;
      // Should throw error!
      exit(-1);
      return -1;
    }
  }
private:
  string saved_val;
};

class int_constant : public int_expression
{
public:
  int_constant(int val) { saved_val = val; }

  virtual int evaluate_expression(map<string, string> &sym_tab)
  {
    return saved_val;
  }
private:
  int saved_val;
};

class add_op : public int_expression
{
public:
  add_op(int_expression *l, int_expression *r)
  {
    left = l;
    right = r;
  }

  virtual int evaluate_expression(map<string, string> &sym_tab)
  {
    int l_val = left->evaluate_expression(sym_tab);
    int r_val = right->evaluate_expression(sym_tab);
    return l_val + r_val;
  }
private:
  int_expression *left;
  int_expression *right;
};

class statement
{
public:
  virtual void evaluate_statement(map<string, string> &sym_tab, ParseTree &tree) = 0;
  // virtual void evaluate_statement_for(map<string, int> &sym_tab, ParseTree &tree, string var) =0;
};

class compound_statement : public statement
{
public:
  compound_statement(statement *first, compound_statement *rest)
  {
    f = first;
    r = rest;
  }

  virtual void evaluate_statement(map<string, string> &sym_tab, ParseTree &tree)
  {
    if (f != NULL)
    {
      f->evaluate_statement(sym_tab, tree);
    }
    if (r != NULL)
    {
      r->evaluate_statement(sym_tab, tree);
    }
  }
  // virtual void evaluate_statement_for(map<string, int> &sym_tab, ParseTree &tree, string var) {
  //   if (f!=NULL) {
  //     f->evaluate_statement_for(sym_tab, tree, var);
  //   }
  //   if (r!=NULL) {
  //     r->evaluate_statement_for(sym_tab, tree, var);
  //   }
  // }
private:
  compound_statement *r;
  statement *f;
};

class for_statement : public statement
{
public:
  for_statement(string var, int start, int end, compound_statement *body)
  {
    this->var = var;
    this->start = start;
    this->end = end;
    b = body;
  }

  virtual void evaluate_statement(map<string, string> &sym_tab, ParseTree &tree)
  {
    for (int i = start; i <= end; i++)
    {
      sym_tab[var] = to_string(i);
      b->evaluate_statement(sym_tab, tree);
    }
    sym_tab.erase(var);
  }
  // virtual void evaluate_statement_for(map<string, int> &sym_tab, ParseTree &tree, int var) {
  //   return;
  // }
private:
  string var;
  int start;
  int end;
  compound_statement *b;
};

class for_in_statement : public statement {
public:
  for_in_statement(string var, string_list *vars, compound_statement *body)
  {
    this->var = var;
    this->vars = vars;
    b = body;
  }

  virtual void evaluate_statement(map<string, string> &sym_tab, ParseTree &tree)
  {
    while(vars != NULL) {
      sym_tab[var] = vars->string;
      b->evaluate_statement(sym_tab, tree);
      vars = vars->next;
    }
  }
private:
  string var;
  string_list * vars;
  compound_statement *b;
};



class build_statement : public statement
{
public:
  build_statement(expression *nameExpr, int_expression *weightExpr, expression *parentNameExpr = NULL)
  {
    name = nameExpr;
    weight = weightExpr;
    parentName = parentNameExpr;
    // printf("Building statement");
  }

  virtual void evaluate_statement(map<string, string> &sym_tab, ParseTree &tree)
  {
    // printf("Evaluating statement\n");
    if (parentName == NULL)
    {
      // printf("Parent name is null\n");
      TreeNode *node = new TreeNode(name->evaluate_expression(sym_tab), weight->evaluate_expression(sym_tab));
      // printf("Adding node %s with weight %s\n", name->evaluate_expression(sym_tab).c_str(), weight->evaluate_expression(sym_tab).c_str());
      tree.addNode(node);
    }
    else
    {
      // printf("Parent name is not null\n");
      TreeNode *node = new TreeNode(name->evaluate_expression(sym_tab), weight->evaluate_expression(sym_tab), parentName->evaluate_expression(sym_tab));
      // printf("Adding node %s with weight %s\n", name->evaluate_expression(sym_tab).c_str(), weight->evaluate_expression(sym_tab).c_str());
      tree.addNode(node);
    }
  }
  // virtual void evaluate_statement_for(map<string, int> &sym_tab, ParseTree &tree, int var) {
  //   TreeNode *node = new TreeNode(name->eval(), weight->eval(), parentName->eval());
  //   tree.addNode(node);
  // }
private:
  expression *name;
  int_expression *weight;
  expression *parentName;
};

#endif // PARSE_TREE_H