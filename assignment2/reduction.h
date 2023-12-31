// definitie reduction class

#ifndef ReductionHVAR // voorkom dat dit bestand meerdere keren
#define ReductionHVAR // ge-include wordt

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_set>
using namespace std;

struct Token {
    enum {
        variabele = 0, 
        lambda = 1, 
        openb = 2, 
        closedb = 3,
        space = 4
    } type;
    string var;
};

class Node {
    public:
        string data;
        int type;
        Node* left;
        Node* right;
        
        Node(const string& value, const int& token) {
            data = value;
            type = token;
            left = nullptr;
            right = nullptr;
        };
};

class Reduction {
    private:
        string input; // input
        int index;
        vector<Token> tokens;
        bool diff;
        int counter;

    public:
        Reduction(); // constructor

        ~Reduction(); // destructor

        bool tokenizer(string input); // zet input in token vector

        Node* AST(); // functie die abstract syntax tree maakt

        void ASTtraversal(Node* root); // doorloopt abstract syntax tree (in-order traversal)

        Node* copyTree(Node* node); // kopieert een boom vanaf meegegeven node

        void deleteTree(Node* &node); // verwijdert een boom vanaf meegegeven node

        Node* fullReduction(Node* root); // start het reduction-proces
    
    private:

        Node* alphaBetaRed(Node* root); // help-functie voor uitvoeren alpha conversion en beta reduction

        void alphaConv(Node* root, unordered_set<string>& vars, bool lambda); // voert alpha conversion uit

        Node* betaRed(Node* root); // voert beta reduction uit
    
};

#endif