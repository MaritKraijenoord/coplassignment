// implementation reduction class

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include "reduction.h"
using namespace std;

// constructor
Reduction::Reduction() {
    index = 0;
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
                cout << "Syntax error: variabele mag niet met getal beginnen" << endl;
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
            cout << "Syntax error: karakter niet herkend" << endl;
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
        /* if (type == 4) {
            data = "apply";
        } else {
            data = tokens[i].var;
        } */ // delete
        data = tokens[i].var;
        Node* newNode = new Node(data, type);
        //cout << "node: " << newNode->data << " " << newNode->type << endl;
        if (newNode->type == 0) { // variabele
            nodeStack.push(newNode);
        } else if (newNode->type == 2) { // open bracket (
            operatorStack.push(newNode);
        } else if (newNode->type == 3) { // closed bracket )
            while (!operatorStack.empty() && operatorStack.top()->type != 2) {
                Node* operatorNode = new Node(operatorStack.top()->data, operatorStack.top()->type);
                operatorStack.pop(); // pop )
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
        operatorStack.pop(); // pop )
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
        cout << root->data << " ";
        ASTtraversal(root->right);
    }
} // ASTtraversal
