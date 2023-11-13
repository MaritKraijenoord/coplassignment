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
    stack<Token> operatorStack;

    for (const Token& token : tokens) {
        if (token.type == Token::variabele) {
            nodeStack.push(new Node(token.var, token.type));
        } else if (token.type == Token::space) {
            while (nodeStack.size() > 1) {
                Node* right = nodeStack.top();
                nodeStack.pop();

                Node* left = nodeStack.top();
                nodeStack.pop();

                Node* applyNode = new Node("apply", 5); // type 5 = apply
                applyNode->left = left;
                applyNode->right = right;

                nodeStack.push(applyNode);
            }
        } else if (token.type == Token::lambda) {
            while (nodeStack.size() > 1) {
                Node* right = nodeStack.top();
                nodeStack.pop();

                Node* left = nodeStack.top();
                nodeStack.pop();

                Node* applyNode = new Node("lambda", 1);
                applyNode->left = left;
                applyNode->right = right;

                nodeStack.push(applyNode);
            }
        }
    }
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
