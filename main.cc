// Main program for Concepts of Programming Languages
// Assignment 1
// Marit Kraijenoord, s3180131
// Jasmijn Makop, s3688321
// DATE

#include <iostream>
#include <vector>
using namespace std;

enum Tokenizer {
    var, lambda, openb, closedb, space
};

int main () {
    string input;
    vector<pair<int,char>> tokens;
    cout << "Geef de input string" << endl;
    getline(cin, input);
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
            return 1;
        }
    }
    for (size_t i = 0; i < input.size(); i++) {
        cout << tokens[i].first << "," << tokens[i].second << endl;
    }

    return 0;
}//main