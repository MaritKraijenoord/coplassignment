// Main program for Concepts of Programming Languages
// Assignment 3
// Marit Kraijenoord, s3180131
// Jasmijn Makop, s3688321
// 31/12/2023

#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
#include "typecheck.h"
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
        cout << "Geef input string (typecheck)" << endl;
        getline(cin, input);
        Typecheck* T1 = new Typecheck(input);
        T1->ASTtraversal(T1->treeroot);
        T1->convert(T1->treeroot->left);
        cout << endl;
        T1->deleteAST(T1->treeroot);
        delete T1;
        cout << "Geef een nieuwe input string (of s voor stoppen)" << endl;
        getline(cin, input);
        while (input != "s") {
            Typecheck* T2 = new Typecheck(input);
            T2->ASTtraversal(T2->treeroot);
            cout << endl;
            T2->deleteAST(T2->treeroot);
            delete T2;
            cout << "Geef een nieuwe input string (of s voor stoppen)" << endl;
            getline(cin, input);
        }
    }
    return 0;
}//main
