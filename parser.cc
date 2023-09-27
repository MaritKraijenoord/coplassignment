// implementation parser class

#include <fstream>
#include <iostream>
#include <vector>
#include "parser.h"
using namespace std;

enum Tokenizer {
    variabele, lambda, openb, closedb, space
};

// constructor parser
Parser::Parser (string input) {
    string var;
    for (size_t i = 0; i < input.size(); i++) {
        if (input[i] == '\\') {
            var = '\\';
            tokens.push_back(make_pair(lambda, var));
        } else if (input[i] == '(') {
            var = '(';
            tokens.push_back(make_pair(openb, var));
        } else if (input[i] == ')') {
            var = ')';
            tokens.push_back(make_pair(closedb, var));
        } else if ( (input[i] >= 48 && input[i] <= 57) || 
                    (input[i] >= 65 && input[i] <= 90) || 
                    (input[i] >= 97 && input[i] <= 122)) {
            if (input[i] >= 48 && input[i] <= 57) {
                cout << "Syntax error: variabele mag niet beginnen met cijfer" << endl;
                //return 1;
            } else {
                var = input[i];
                i++;
                while (input[i] != ' ' && i < input.size()) {
                    var = var + input[i];
                    i++;
                }
            }
            cout << "voor" << endl;
            tokens.push_back(make_pair(variabele, var)); // probleem
            cout << "na" << endl;
        } else if (input[i] == 32) {
            var = " ";
            tokens.push_back(make_pair(space, var));
        } else {
            cout << "Syntax error: karakter niet herkend" << endl;
            //return 1;
        }
    }
    index = 0;
    result = "";
    cout << "Input:";
    for (size_t i = 0; i < tokens.size(); i++) {
        cout << " " << tokens[i].second;
    }
    cout << endl;
    expr();
    cout << result << endl;
}

// expr functie
void Parser::expr () {
    //cout << "expr" << endl;
    if (result == "Input not acceptable, syntax error") {
        return;
    }
    lexpr();
    exprap();
} // expr

// expr' functie
void Parser::exprap () {
    //cout << "exprap" << endl;
    if (result == "Input not acceptable, syntax error") {
        return;
    }
    if (tokens[index].second != ")" && index+1 < tokens.size()) {
        lexpr();
        exprap();
    } else { // empty
        return;
    }
} // exprap

// lexpr functie
void Parser::lexpr () {
    //cout << "lexpr" << endl;
    if (result == "Input not acceptable, syntax error") {
        return;
    }
    if (tokens[index].second == "\\") {
        //cout << tokens[index].second; // lambda
        result = result + tokens[index].second;
        index++;
        if (tokens[index].first == Tokenizer::variabele) {
            //cout << tokens[index].second; // var
            result = result + tokens[index].second;
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
    if (result == "Input not acceptable, syntax error") {
        return;
    }
    if (tokens[index].second == "(") {
        //cout << tokens[index].second;
        result = result + tokens[index].second;
        index++;
        expr();
        if (tokens[index].second == ")") {
            //cout << tokens[index].second;
            result = result + tokens[index].second;
        } else {
            result = "Input not acceptable, syntax error";
        }
    } else if (tokens[index].first == Tokenizer::variabele) {
        //cout << tokens[index].second; // var
        result = result + tokens[index].second;
        index++;
        return;
    }
} // pexpr
