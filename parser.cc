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
    string variabele = "";
    resultaat = "";
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\') {
            variabele = "\\";
            tokens.push_back(make_pair(lambda, variabele));
        } else if (input[i] == '(') {
            variabele = "(";
            tokens.push_back(make_pair(openb, variabele));
        } else if (input[i] == ')') {
            variabele = ")";
            tokens.push_back(make_pair(closedb, variabele));
        } else if ( (input[i] >= 48 && input[i] <= 57) || 
                    (input[i] >= 65 && input[i] <= 90) || 
                    (input[i] >= 97 && input[i] <= 122)) {
            if (input[i] >= 48 && input[i] <= 57) {
                cout << "Syntax error: variabele mag niet met getal beginnen" << endl;
            } else {
                variabele = input[i];
                i++;
                while ((input[i] >= 65 && input[i] <= 90) || 
                       (input[i] >= 97 && input[i] <= 122)) {
                    variabele = variabele + input[i];
                    i++;
                }
                i--;
            }
            tokens.push_back(make_pair(var, variabele));
    cout << "hoi";
        } else if (input[i] == 32) {
            variabele = " ";
            tokens.push_back(make_pair(space, variabele));
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
    if (index == tokens.size()) {
        return;
    }
    if ((index+1 < tokens.size() && tokens[index].second != ")") 
        || (index+1 == tokens.size() && tokens[index].first == Tokenizer::var)) {
        lexpr();
        exprap();
    } else { // empty
        return;
    }
} // exprap

// lexpr functie
void Parser::lexpr () {
    //cout << "lexpr" << endl;
    if (tokens[index].second == "\\") {
        cout << tokens[index].second; // lambda
        index++;
        if (tokens[index].first == Tokenizer::space) {
            index++;
        }
        if (tokens[index].first == Tokenizer::var) {
            cout << tokens[index].second; // var
            index++;
            if (tokens[index].first == Tokenizer::space) {
                index++;
            }
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
    if (tokens[index].second == "(") {
        cout << tokens[index].second;
        index++;
        if (tokens[index].first == Tokenizer::space) {
            index++;
        }
        expr();
        if (tokens[index].second == ")")  {
            cout << tokens[index].second;
            index++;
            if (tokens[index].first == Tokenizer::space) {
                index++;
            }
        } else {
            // onjuist
        }
    } else if (tokens[index].first == Tokenizer::var) {
        cout << tokens[index].second; // var
        index++;
        if (tokens[index].first == Tokenizer::space) {
            index++;
        }
        return;
    }
} // pexpr