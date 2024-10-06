// grammar.h File



#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>



class Grammar {
private:
    std::vector<std::string> terminals;        // vector to store terminal symbols
    std::vector<std::string> nonTerminals;     // vector to store non-terminal symbols
    std::map<std::string, std::vector<std::string>> productions; // Map to store productions
    std::string startSymbol;                // Start symbol of the grammar
    void firstHelper(const std::string& item); // Helper function for First
    std::vector<std::string> firstSet;         // Vector to store the set of terminals for the First of an input

public:
    std::set<std::string> processedSymbols;
    std::map<std::string, std::vector<std::string>> firstSets;

    Grammar(); // Constructor

    void readGrammar(std::ifstream& filename); // Function to read grammar from a file

    void addProduction(std::string nonTerminal, const std::string& production);  // Function to print the grammar

    void printGrammar() const; // Function to print the grammar

    void clearGrammar(); // Function to clear all the grammar data, including the start symbol.

    void printFirst(); // Function to print First for every symbol

    std::string First(const std::string& input) ; // Funtion to find First for any symbol in the grammar
    
};

#endif // GRAMMAR_H
