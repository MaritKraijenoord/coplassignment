// Main program for Concepts of Programming Languages
// Assignment 2
// Marit Kraijenoord, s3180131
// Jasmijn Makop, s3688321
// 09/11/2023

#include <iostream>
#include <vector>
#include "parser.h"
#include "reduction.h"
using namespace std;

int main () {
    string input;
    cout << "Geef de input string" << endl;
    getline(cin, input);
    Parser* P1 = new Parser(input);
    Reduction* R1 = new Reduction();
    if (R1->tokenizer(input)) {
        Node* root = R1->AST();
        R1->ASTtraversal(root);
    }
    delete P1;
    delete R1;
    return 0;
}//main