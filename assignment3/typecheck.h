// definitie typecheck class

#ifndef TypecheckHVAR // voorkom dat dit bestand meerdere keren
#define TypecheckHVAR // ge-include wordt

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node {
    public:
        string data;
        Node* left;
        Node* right;

        Node (string var) {
            data = var;
            left = nullptr;
            right = nullptr;
        }

        Node () {
            data = "";
            left = nullptr;
            right = nullptr;
        }
};

class Typecheck {
    public:
        Typecheck(string input);
        
        void judgement();

        Node* expr(string p1);

        Node* type(string p1);

        bool lvar(string p1, Node* &node);
        
        bool uvar(string p1, Node* &node);

        void ASTtraversal(Node* root);

        void deleteAST(Node* root);

        Node* treeroot = nullptr;

    private:
        string exp;
        size_t index;
        bool arrow;
};

#endif