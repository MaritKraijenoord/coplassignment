// definitie reduction class

#ifndef ReductionHVAR // voorkom dat dit bestand meerdere keren
#define ReductionHVAR // ge-include wordt

#include <fstream>
#include <iostream>
using namespace std;

class Node {
    public:
        string data;
        Node* left;
        Node* right;
        
    Node(const string& value) {
        data = value;
        left = nullptr;
        right = nullptr;
    }  
};

class Reduction {
    private:
        string input; // input
    public:
        Reduction(string input); // constructor

        ~Reduction(); // destructor

        Node* AST();

        // functie voor alfa reductie
        
        // functie voor beta reductie
};

#endif