// implementation reduction class

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>
#include "reduction.h"
using namespace std;

// constructor
Reduction::Reduction() {
    index = 0;
    counter = 0;
} // constructor

// destructor
Reduction::~Reduction() {
} // destructor

// plaatst alle tokens in een vector met enum waardes
bool Reduction::tokenizer (string input) {
    Token t;
    string variabele;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\') {
            t.var = "\\";
            t.type = Token::lambda;
            tokens.push_back(t);
        } else if (input[i] == '(') {
            if (input[i-1] == ')') {
                t.var = " ";
                t.type = Token::space;
                tokens.push_back(t);
            }
            t.var = "(";
            t.type = Token::openb;
            tokens.push_back(t);
        } else if (input[i] == ')') {
            t.var = ")";
            t.type = Token::closedb;
            tokens.push_back(t);
        } else if ( (input[i] >= 48 && input[i] <= 57) || 
                    (input[i] >= 65 && input[i] <= 90) || 
                    (input[i] >= 97 && input[i] <= 122)) {
            if (input[i] >= 48 && input[i] <= 57 && 
                (input[i-1] == ' ' || input[i-1] == ')' ||input[i-1] == '(')) {
                cerr << "Syntax error: variabele mag niet met getal beginnen" << endl;
                exit(1);
            } else {
                t.var = input[i];
                i++;
                while ((input[i] >= 65 && input[i] <= 90) || 
                       (input[i] >= 97 && input[i] <= 122)) {
                    t.var = t.var + input[i];
                    i++;
                }
                i--;
            }
            t.type = Token::variabele;
            tokens.push_back(t);
        } else if (input[i] == ' ') {
            t.var = " ";
            t.type = Token::space;
            tokens.push_back(t);
        } else {
            cerr << "Syntax error: karakter niet herkend" << endl;
            exit(1);
        }
    }
    return true;
} // tokenizer

// zet tokens in AST
Node* Reduction::AST () {
    stack<Node*> nodeStack;
    stack<Node*> operatorStack;
    string data;
    for (size_t i = 0; i < tokens.size(); i++) {
        int type = tokens[i].type;
        data = tokens[i].var;
        Node* newNode = new Node(data, type);
        if (newNode->type == 0) { // variabele
            nodeStack.push(newNode);
        } else if (newNode->type == 2) { // open bracket (
            operatorStack.push(newNode);
        } else if (newNode->type == 3) { // closed bracket )
            while (!operatorStack.empty() && operatorStack.top()->type != 2) {
                Node* operatorNode = new Node(operatorStack.top()->data, operatorStack.top()->type);
                operatorStack.pop(); 
                operatorNode->right = nodeStack.top();
                nodeStack.pop();
                if (operatorNode->type != 1) {
                    operatorNode->left = nodeStack.top();
                    nodeStack.pop();
                }
                nodeStack.push(operatorNode);
            }
            operatorStack.pop(); // pop (
        } else if (newNode->type == 1 || newNode->type == 4) { // lambda en space
            operatorStack.push(newNode);
        }
    }
    while (!operatorStack.empty()) {
        Node* operatorNode = new Node(operatorStack.top()->data, operatorStack.top()->type);
        operatorStack.pop(); // pop
        operatorNode->right = nodeStack.top();
        nodeStack.pop();
        if (operatorNode->type != 1) {
            operatorNode->left = nodeStack.top();
            nodeStack.pop();
        }
        nodeStack.push(operatorNode);
    }
    // haakjes zijn uit expressie gehaald,
    // precedence is uitgewerkt in ast
    return nodeStack.top();
} // AST

// laat inorder traversal van abstract syntax tree  zien
void Reduction::ASTtraversal(Node* root) {
    if (root) {
        ASTtraversal(root->left);
        if (root->data == " ") {
            cout << "@ "; // voor leesbaarheid AST (application)
        } else {
            cout << root->data << " ";
        }
        ASTtraversal(root->right);
    }
} // ASTtraversal

// kopieert een boom vanaf de meegegeven node
Node* Reduction::copyTree(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    Node* newNode = new Node(node->data, node->type);
    newNode->left = copyTree(node->left);
    if (node->type != 1) {
        newNode->right = copyTree(node->right);
    }
    return newNode;
} // copyTree

void Reduction::deleteTree(Node* &node) {
    if (node != nullptr) {
        // verwijder linker- en rechtersubbomen
        deleteTree(node->left);
        deleteTree(node->right);
        // verwijder huidige node
        delete node;
        node = nullptr; // zet als nullptr na delete
    }
} // verwijderBoomDot

// start het reduction-proces
Node* Reduction::fullReduction(Node* root) {
    root = alphaBetaRed(root);
    return root;
} // fullReduction

// help-functie voor uitvoeren alpha conversion en beta reduction
Node* Reduction::alphaBetaRed(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    // alpha conversion
    unordered_set<string> vars;
    alphaConv(root, vars, false);
    // }
    cout << endl;
    cout << "After alpha conversion: " << endl;
    ASTtraversal(root);
    cout << endl;
    int count = 0;
    diff = true;
    cout << "After beta reduction: " << endl;
    while (diff) {
        diff = false;
        // beta reduction
        root = betaRed(root);
        count++;
        // controle niet oneindig reduction blijven proberen
        if (count > 100) {
            cerr << "Oneindige bèta reductie" << endl;
            exit(2);
        }
    }
    ASTtraversal(root);
    cout << endl;
    return root;
} // alphaBetaRed

// voert alpha conversion uit
void Reduction::alphaConv(Node* root, unordered_set<string>& vars, bool lambda) {
    if (root == nullptr) {
        return;
    }
    if (root->type == 1) { // lambda
        if (root->right != nullptr) {
            alphaConv(root->right ,vars, true);
        }
    } else if (root->type == 0) {
        if (!lambda && vars.count(root->data) > 0) {
            // conflict, rename
            string rename = "a" + to_string(counter++);
            root->data = rename;
            vars.insert(rename);
        }
        vars.insert(root->data);
    } else if (root->type == 4) {
        if (lambda) {
            if (root->left != nullptr) {
                alphaConv(root->left, vars, true);
            }
            if (root->right != nullptr) {
                alphaConv(root->right, vars, false);
            }
        } else {
            if (root->left != nullptr) {
                alphaConv(root->left, vars, false);
            }
            if (root->right != nullptr) {
                alphaConv(root->right, vars, false);
            }
        }
    } else {
        cerr << "Syntax error: karakter niet herkend" << endl;
        exit(1);
    }
    
} // alphaConv

// voert beta reduction uit
Node* Reduction::betaRed(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->type == 4) { // application
        if (root->left != nullptr && root->left->type == 1 && root->right != nullptr) {
            if (root->left->right != nullptr && root->left->right->left != nullptr) {
                Node* newnode = copyTree(root->right);
                deleteTree(root->left->right->left);
                root->left->right->left = copyTree(newnode);
                deleteTree(newnode);
            }
        }
    }
    if (root->left != nullptr) {
        root->left = betaRed(root->left);
    }
    if (root->right != nullptr) {
        root->right = betaRed(root->right);
    } 
    return root;
} // betaRed
