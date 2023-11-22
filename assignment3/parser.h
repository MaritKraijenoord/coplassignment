// definitie parser class

#ifndef ParserHVAR3 // voorkom dat dit bestand meerdere keren
#define ParserHVAR3 // ge-include wordt

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Parser {
    public:
        Parser (string input);

        void expr ();

        void exprap();

        void lexpr ();

        void pexpr ();

    private:
        vector<pair<int,string>> tokens;
        string resultaat;
        size_t index;

};

#endif