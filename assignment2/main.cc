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
    string input;
    if (argc > 1) {
        ifstream file(argv[1]);
        if (file.is_open()) {
            while(getline(file, input)) {
                Parser* P1 = new Parser(input, true);
                delete P1;
            }
        } else {
            cerr << "Error opening file: " << argv[1] << endl;
        }
    } else {
        cout << "Geef de input string" << endl;
        getline(cin, input);
        Parser* P1 = new Parser(input, false);
        while (input != "n") {
            Reduction* R1 = new Reduction();
            if (R1->tokenizer(input)) {
                Node* root = R1->AST();
                R1->ASTtraversal(root);
                cout << endl;
                R1->fullReduction(root);
                R1->ASTtraversal(root);
                cout << endl;
            }
            cout << "Geef nieuwe input (of n voor stoppen)" << endl;
            getline(cin, input);
            delete R1;
        }
        delete P1;
    }
    return 0;
}//main