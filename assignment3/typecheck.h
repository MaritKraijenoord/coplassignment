// definitie typecheck class

#ifndef TypecheckHVAR // voorkom dat dit bestand meerdere keren
#define TypecheckHVAR // ge-include wordt

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Node {
    string data;
    Node* left;
    Node* right;

    Node (string var) {
        data = var;
        left = nullptr;
        right = nullptr;
    }

};

class Typecheck {
    public:
        Typecheck(string input);
        
        void judgement();

        void expr();

        void type();

        void AST();

    private:
        string exp;
};

#endif