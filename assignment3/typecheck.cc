// implementation typecheck class

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "typecheck.h"
using namespace std;

// constructor
Typecheck::Typecheck (string input) {
    exp = input;
    index = 0;
    arrow = false;
    judgement();
    // typecheck
} // constructor

// judgement
void Typecheck::judgement () {
    string expression;
    string types;
    bool correct = false;
    int location;
    for (size_t j = 0; j < exp.size(); j++) {
        if (exp[j] == ':') {
            correct = true;
            location = j;
        }
    }
    if (!correct) {
        cerr << "Syntax error: missing types" << endl;
        exit(1);
    }
    expression = exp.substr(0, location);
    types = exp.substr(location+1);
    // cout << "expr: " << expression << endl;
    // cout << "type: " << types << endl;
    // start boom
    treeroot = new Node(":");
    index = 0;
    treeroot->left = expr(expression);
    index = 0;
    treeroot->right = type(types);
    lambda = false;
    treeroot->left = convert(treeroot->left);
    if (compare(treeroot->left, treeroot->right)) {
        cout << exp << endl;
        ASTtraversal(treeroot);
        cout << endl;
        cout << "Correct expression" << endl;
    } else {
        cout << "Incorrect expression" << endl;
    }
} // judgement

// expr
Node* Typecheck::expr (string p1) {
    string s;
    Node* n4 = new Node("\0");
    if (index < p1.size() && p1[index] == '(') {
        index++;
        n4 = expr(p1);
        if (p1[index] != ')') {
            cerr << "Syntax error: ')' expected, not found" << endl;
            exit(1);
        }
        return n4;
    } else if (index < p1.size() && p1[index] == '\\') {
        s = p1[index];
        Node* n1 = new Node(s);
        index++;
        if (index < p1.size() && !lvar(p1, n4)) {
            cerr << "Syntax error: no lowercase letter found" << endl;
            exit(1);
        }
        if (index < p1.size() && p1[index] != '^') {
            cerr << "Syntax error: no '^' found" << endl;
            exit(1);
        }
        s = p1[index];
        Node* n3 = new Node(s);
        index++;
        n3->left = n4;
        n3->right = type(p1);
        index++;
        n1->left = n3;
        n1->right = expr(p1);
        return n1;
    } else if (lvar(p1, n4)) {
        return n4;
    } else if (index < p1.size() && !lvar(p1, n4)) {
        Node* node = new Node("@"); // application
        node->left = expr(p1);
        if (index < p1.size() && p1[index] != ' ') {
            cerr << "Syntax error: space expected for application, not found" << endl;
            exit(1);
        }
        node->right = expr(p1);
        return node;
    } else {
        cerr << "Syntax error: not enough characters" << endl;
        exit(1);
    }
} // expr

// type
Node* Typecheck::type (string p1) {
    string s;
    Node* n2 = new Node("\0");
    if (uvar(p1, n2)) {
        if (arrow && index < p1.size()) {
            arrow = false;
            Node* n1 = n2;
            if (p1[index] != '-') {
                cerr << "Syntax error: no '-' found" << endl;
                exit(1);
            }
            index++;
            if (p1[index] != '>') {
                cerr << "Syntax error: no '>' found" << endl;
            }
            index++;
            Node* n3 = new Node("->");
            n3->left = n1;
            n3->right = type(p1);
            return n3;
        } else {
            return n2;
        }
    } else if (index < p1.size() && p1[index] == '(') {
        index++;
        Node* node = type(p1);
        if (p1[index] != ')') {
            cerr << "Syntax error: ')' expected, not found" << endl;
            exit(1);
        } 
        index++;
        return node;
    } else {
        cerr << "Syntax error: not enough characters" << endl;
        exit(1);
    }
} // type

// lvar
bool Typecheck::lvar (string p1, Node* &node) {
    string s;
    if (index < p1.size() && p1[index] >= 'a' && p1[index] <= 'z') {
        s = p1[index];
        index++;
        while (index < p1.size() && ((p1[index] >= 'a' && p1[index] <= 'z') || 
                (p1[index] >= 'A' && p1[index] <= 'Z') || 
                (p1[index] >= '0' && p1[index] <= '9'))) {
            s = s + p1[index];
            index++;
        }
        node->cat = 2;
        node->data = s;
        return true;
    } else if (index < p1.size()) {
        return false;
    } else {
        cerr << "Syntax error: not enough characters" << endl;
        exit(1);
    }
} // lvar

// uvar
bool Typecheck::uvar (string p1, Node* &node) {
    string s;
    if (index < p1.size() && p1[index] >= 'A' && p1[index] <= 'Z') {
        s = p1[index];
        index++;
        while (index < p1.size() && ((p1[index] >= 'a' && p1[index] <= 'z') || 
                (p1[index] >= 'A' && p1[index] <= 'Z') || 
                (p1[index] >= '0' && p1[index] <= '9'))) {
            s = s + p1[index];
            index++;
        }
        if (index < p1.size() && p1[index] != ')' && p1[index] != '-' 
            && p1[index] != '(' && p1[index] != '\\' && p1[index] != ' ') {
            cout << "index:" << p1[index] << endl;
            cerr << "Syntax error: wrong character" << endl;
            exit(1);
        }
        if (index < p1.size()-1 && p1[index] == '-' && p1[index+1] == '>') {
            arrow = true;
        }
        node->cat = 1;
        node->data = s;
        return true;
    } else if (index < p1.size()) {
        return false;
    } else {
        cerr << "Syntax error: not enough characters" << endl;
        exit(1);
    }
} // uvar

// ASTtraversal
void Typecheck::ASTtraversal (Node* root) {
    if (root == nullptr) {
        return;
    }
    if (root->data == "\\") {
        cout << root->data << " ";
        ASTtraversal(root->left);
    } else {
        ASTtraversal(root->left);
        cout << root->data << " ";
    }
    ASTtraversal(root->right);
} // ASTtraversal

// deleteAST
void Typecheck::deleteAST (Node* root) {
    if (root == nullptr) {
        return;
    }
    deleteAST(root->left);
    deleteAST(root->right);
    delete root;
} // deleteAST

// kopieert een boom vanaf de meegegeven node
Node* Typecheck::copyTree(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    Node* newNode = new Node(node->data);
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);
    return newNode;
} // copyTree

// convert
Node* Typecheck::convert (Node* root) {
    if (root->data == "\\") { // lambda
        lambda = true;
        root->data = "->"; 
        root->left = convert(root->left);
        lambda = false;
        root->right = convert(root->right);
        if (varleft == varright) {
            if (root->right->data == varright) {
                root->right->data = root->left->data;
            }
            return root;
        } else {
            cerr << "Syntax error: unknown type" << endl;
            exit(1);
        }
    } else if (root->data == "^") { // roof
        root->left = convert(root->left);
        root = convert(root->right);
    } else if (root->cat == 1) { // uvar
        return root;
    } else if (root->cat == 2) { // lvar
        if (lambda) {
            varleft = root->data;
        } else {
            varright = root->data;
        }
    } else if (root->data == "@") { // application
        root->left = convert(root->left);
        root->right = convert(root->right);
    }
    return root;
} // convert

// compare
bool Typecheck::compare (Node* left, Node* right) {
    if (left == nullptr && right == nullptr) {
        return true;
    } else if ((left == nullptr && right != nullptr) || (left != nullptr && right == nullptr)) {
        return false;
    }
    if (left->data == right->data) {
        if (compare(left->left, right->left) && compare(left->right, right->right)) {
            return true;
        }
    }
    return false;
}