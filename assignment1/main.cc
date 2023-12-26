// Main program for Concepts of Programming Languages
// Assignment 1
// Marit Kraijenoord, s3180131
// Jasmijn Makop, s3688321
// 27/09/2023

#include <iostream>
#include <fstream>
#include <vector>
#include "parser.h"
using namespace std;

int main (int argc, char* argv[]) {
    string input = "j";
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
        Parser* P1 = nullptr;
        cout << "Geef de input string (of n voor stoppen)" << endl;
        getline(cin, input);
        while (input != "n") {
            P1 = new Parser(input, false);
            delete P1;
            cout << "Geef de input string (of n voor stoppen)" << endl;
            getline(cin, input);
        }
        delete P1;
    }
    return 0;
}//main