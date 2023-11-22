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
        data = tokens[i].var;
        Node* newNode = new Node(data, type);
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

// start het reduction-proces
Node* Reduction::fullReduction(Node* root) {
    Node* prevRoot = nullptr;
    int count = 0;
    while (root != prevRoot) {
        prevRoot = root;
        root = alphaBetaRed(root);
        count++;
        if (count > 10) {
            exit(2);
        }
    }
    return root;
} // fullReduction

// help-functie voor uitvoeren alpha conversion en beta reduction
Node* Reduction::alphaBetaRed(Node* root) {
    if (root == nullptr) {
        return nullptr;
    }
    // alpha conversion
    alphaConvVar(root, false);
    alphaConvApp(root, false);
    cout << "Gevonden variabelen (captured): ";
    for (size_t i = 0; i < usedVar.size(); i++) {
        cout << usedVar[i]->data << " ";
    }
    cout << endl;
    cout << "Gevonden variabelen (free): ";
    for (size_t i = 0; i < freeVar.size(); i++) {
        cout << freeVar[i]->data << " ";
    }
    int N = 97;
    string change = "A";
    for (size_t i = 0; i < freeVar.size(); i++) {
        for (size_t j = 0; j < usedVar.size(); j++) {
            if (freeVar[i]->data == usedVar[i]->data) {
                freeVar[i]->data = change + char(N);
                N++;
                if (N > 122) {
                    N = 97;
                    change = change + char(N);
                }
            }
        }
    }
    int count = 0;
    Node* node = nullptr;
    while (node != root) {
        node = root;
        // beta reduction
        root = betaRed(root, "x", node);
        count++;
        // controle niet oneindig blijven reduction
        if (count > 10) {
            exit(2);
        }
    }
    return root;
} // alphaBetaRed

// voert alpha conversion uit
void Reduction::alphaConvVar(Node* root, bool lambdaParent) {
    if (root == nullptr) {
        return;
    }
    if (root->type == 1) { // lambda
        lambdaParent = true;
        alphaConvVar(root->left, lambdaParent);
        alphaConvVar(root->right, lambdaParent);
        lambdaParent = false;
    } else if (root->type == 0 && lambdaParent) { // variable
        usedVar.push_back(root);
    } else {
        alphaConvVar(root->left, lambdaParent);
        alphaConvVar(root->right, lambdaParent);
    }
} // alphaConvVar

// checkt vrije variabelen
void Reduction::alphaConvApp (Node* root, bool app) {
    if (root == nullptr) {
        return;
    }
    if (root->type == 4) { // space
        app = true;
        alphaConvApp(root->left, app);
        alphaConvApp(root->right, app);
        app = false;
    } else if (root->type == 1) { // lambda (captured)
        return;
    } else if (root->type == 0 && app) { // variable
        freeVar.push_back(root);
    } else {
        alphaConvApp(root->left, app);
        alphaConvApp(root->right, app);
    }
} // alphaConvApp

// voert beta reduction uit
Node* Reduction::betaRed(Node* root, string var, Node* replacement) {
    if (root == nullptr) {
        return nullptr;
    }
    // zoek application
    // zoek lambda
    return root;
} // betaRed
