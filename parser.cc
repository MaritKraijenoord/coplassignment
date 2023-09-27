// implementation parser class

#include <fstream>
#include <iostream>
#include <vector>
#include "parser.h"
using namespace std;

enum Tokenizer {
    var, lambda, openb, closedb, space
};

// constructor parser
Parser::Parser (string input) {
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\') {
            tokens.push_back(make_pair(lambda, input[i]));
        } else if (input[i] == '(') {
            tokens.push_back(make_pair(openb, input[i]));
        } else if (input[i] == ')') {
            tokens.push_back(make_pair(closedb, input[i]));
        } else if ( (input[i] >= 48 && input[i] <= 57) || 
                    (input[i] >= 65 && input[i] <= 90) || 
                    (input[i] >= 97 && input[i] <= 122)) {
            tokens.push_back(make_pair(var, input[i]));
        } else if (input[i] == 32) {
            tokens.push_back(make_pair(space, input[i]));
        } else {
            cout << "Syntax error: karakter niet herkend" << endl;
            //return 1;
        }
    }
    index = 0;
    cout << "Input:";
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << " " << tokens[i].second;
    }
    cout << endl;
    expr();
}

// expr functie
void Parser::expr () {
    //cout << "expr" << endl;
    lexpr();
    exprap();
} // expr

// expr' functie
void Parser::exprap () {
    //cout << "exprap" << endl;
    if (tokens[index].second != ')' && index+1 < tokens.size()) {
        lexpr();
        exprap();
    } else { // empty
        return;
    }
} // exprap

// lexpr functie
void Parser::lexpr () {
    //cout << "lexpr" << endl;
    if (tokens[index].second == '\\') {
        cout << tokens[index].second; // lambda
        index++;
        if (tokens[index].first == Tokenizer::var) {
            cout << tokens[index].second; // var
            index++;
        } else {
            // verkeerde input
        }
        lexpr();
    } else if (index < tokens.size()){
        pexpr();
    }
} // lexpr

// pexpr functie
void Parser::pexpr () {
    //cout << "pexpr" << endl;
    if (tokens[index].second == '(') {
        cout << tokens[index].second;
        index++;
        expr();
        if (tokens[index].second == ')') {
            cout << tokens[index].second;
        } else {
            // onjuist
        }
    } else if (tokens[index].first == Tokenizer::var) {
        cout << tokens[index].second; // var
        index++;
        return;
    }
} // pexpr
