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
    // bij inlezen vanuit file
    // zet aan het eind een s op een aparte regel
    string input;
    bool file;
    if (argc > 0) {
        file = true;
    } else {
        file = false;
    }
    if (!file) {
        cout << "Geef de input string (typecheck)" << endl;
    }
    while (input != "s") {
        getline(cin, input);
        Typecheck* T1 = new Typecheck(input);
        T1->deleteAST(T1->treeroot);
        delete T1;
        if (!file) {
            cout << "Geef een nieuwe input string (of s voor stoppen)" << endl;
        }
        getline(cin, input);
        while (input != "s") {
            T1 = new Typecheck(input);
            T1->deleteAST(T1->treeroot);
            delete T1;
            if (!file) {
                cout << "Geef een nieuwe input string (of s voor stoppen)" << endl;
            }
            getline(cin, input);
        }
    }
    return 0;
}//main
