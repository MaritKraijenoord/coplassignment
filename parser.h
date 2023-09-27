// definitie parser class

#ifndef ParserHVAR // voorkom dat dit bestand meerdere keren
#define ParserHVAR // ge-include wordt

#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class Parser {
    public:
        Parser (string input);

        void expr ();

        void exprap();

        void lexpr ();

        void pexpr ();

    private:
        vector<pair<int,char>> tokens;
        size_t index;

};

#endif