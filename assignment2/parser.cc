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
Parser::Parser (string input, bool file) {
    string variabele = "";
    string opnieuwParsen;
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
            if (input[i] >= 48 && input[i] <= 57 && 
                (input[i-1] == ' ' || input[i-1] == ')' ||input[i-1] == '(')) {
                cerr << "Syntax error: variabele mag niet met getal beginnen" << endl;
                exit(1);
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
        } else if (input[i] == 32) {
            variabele = " ";
            tokens.push_back(make_pair(space, variabele));
        } else {
            cerr << "Syntax error: karakter niet herkend" << endl;
            exit(1);
        }
    }
    index = 0;
    expr();
    std::cout << resultaat << std::endl;
    if (!file) {
        std::cout << "Wil u het resultaat opnieuw parsen? (j/n)" << std::endl;
        getline(cin, opnieuwParsen);
        if (opnieuwParsen == "J" || opnieuwParsen == "j") {
            Parser* P2 = new Parser(resultaat, file);
            delete P2;
        } else {
            std::cout << "Er wordt niet opnieuw geparsed." << std::endl;
        }
    }
}

// expr functie
void Parser::expr () {
    lexpr();
    exprap();
} // expr

// expr' functie
void Parser::exprap () {
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
    if (tokens[index].second == "\\") {
        resultaat = resultaat + tokens[index].second;
        index++;
        if (tokens[index].first == Tokenizer::space) {
            resultaat = resultaat + tokens[index].second;
            index++;
        }
        if (tokens[index].first == Tokenizer::var) {
            resultaat = resultaat + tokens[index].second;
            index++;
            if (tokens[index].first == Tokenizer::space) {
                resultaat = resultaat + tokens[index].second;
                index++;
            }
        } else {
            cerr << "Syntax error: missing variable" << endl;
            exit(1);
        }
        lexpr();
    } else if (index < tokens.size()){
        pexpr();
    }
} // lexpr

// pexpr functie
void Parser::pexpr () {
    if (tokens[index].second == "(" && index+1 < tokens.size() && tokens[index+1].second != ")") {
        resultaat = resultaat + tokens[index].second;
        index++;
        if (tokens[index].first == Tokenizer::space) {
            resultaat = resultaat + tokens[index].second;
            index++;
        }
        expr();
        if (tokens[index].second == ")")  {
            resultaat = resultaat + tokens[index].second;
            index++;
            if (tokens[index].first == Tokenizer::space) {
                resultaat = resultaat + tokens[index].second;
                index++;
            }
        } else {
            cerr << "Syntax error: missing closing parenthesis" << endl;
            exit(1);
        }
    } else if (tokens[index].second == "(" && index+1 < tokens.size() && tokens[index+1].second == ")" ) {
        cerr << "Syntax error: missing variable" << endl;
        exit(1);
    } else if (tokens[index].first == Tokenizer::var) {
        resultaat = resultaat + tokens[index].second;
        index++;
        if (tokens[index].first == Tokenizer::space) {
            resultaat = resultaat + tokens[index].second;
            index++;
        }
        return;
    }
} // pexpr