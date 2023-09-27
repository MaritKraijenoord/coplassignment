// Main program for Concepts of Programming Languages
// Assignment 1
// Marit Kraijenoord, s3180131
// Jasmijn Makop, s3688321
// DATE

#include <iostream>
#include <vector>
#include "parser.h"
using namespace std;

int main () {
    string input;
    cout << "Geef de input string" << endl;
    getline(cin, input);
    Parser* P1 = new Parser(input);
    return 0;
}//main