// main.cpp

#include "grammar.h"
#include <iostream>
#include <fstream>

int main() {
    Grammar grammar;

    std::string filename = "grammar.txt"; 

    std::ifstream file(filename);

    grammar.readGrammar(file);
    grammar.printFirst();

    return 0;
}
