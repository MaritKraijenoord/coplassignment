// Main program for Concepts of Programming Languages
// Assignment 2
// Marit Kraijenoord, s3180131
// Jasmijn Makop, s3688321
// 09/11/2023

#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include "reduction.h"
using namespace std;

int main (int argc, char* argv[]) {
    // bij inlezen vanuit file
    // zet aan het eind een n op een aparte regel
    string input;
    bool file;
    if (argc > 0) {
        file = true;
    } else {
        file = false;
    }
    if (!file) {
        cout << "Geef de input string" << endl;
    }
    getline(cin, input);
    Parser* P1 = new Parser(input, file);
    while (input != "n") {
        Reduction* R1 = new Reduction();
        if (R1->tokenizer(input)) {
            Node* root = R1->AST();
            R1->ASTtraversal(root);
            cout << endl;
            R1->fullReduction(root);
        }
        if (!file) {
            cout << "Geef nieuwe input (of n voor stoppen)" << endl;
        }
        getline(cin, input);
        delete R1;
    }
    delete P1;
    return 0;
}//main