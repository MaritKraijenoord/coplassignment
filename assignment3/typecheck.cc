// implementation typecheck class

#include <fstream>
#include <iostream>
#include <string>
#include "typecheck.h"
using namespace std;

// constructor
Typecheck::Typecheck (string input) {
    exp = input;
    judgement();
} // constructor

// judgement
void Typecheck::judgement() {
    string expr;
    string type;
    bool correct = false;
    int location;
    for (size_t j = 0; j < exp.size(); j++) {
        if (exp[j] == ':') {
            correct = true;
            location = j;
        }
    }
    if (!correct) {
        cerr << "Syntax error: no ':' found" << endl;
        exit(1);
    }
    expr = exp.substr(0, location);
    type = exp.substr(location+1);
    cout << "expr: " << expr << endl;
    cout << "type: " << type << endl;
} // judgement
