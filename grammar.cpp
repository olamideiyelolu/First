// grammar.cpp File

#include "grammar.h"
#include <fstream>
#include <sstream>
#include <algorithm>

//******************************************************************************

Grammar::Grammar() { // Constructor to initialize startSymbol to an empty string
    startSymbol = "";
}
//******************************************************************************

void Grammar::readGrammar(std::ifstream& filename) {  
    bool errorOccurred = false;  // Flag to indicate if an error occurred

    if (filename.is_open()) {
        if (filename.peek() == std::ifstream::traits_type::eof()) {        // Checks if the file is empty
            std::cerr << "Error: The file is empty." << std::endl;
            return; 
        }

        std::string line;
        while (getline(filename, line)) {
            if (line.empty()) {   
                continue; // Ignores empty lines
            }

            std::istringstream token(line);  
            std::string lhs, arrow, rhs;

            // Reads the left-hand side (lhs) and '->'
            token >> lhs >> arrow;  

            if (lhs.empty() || lhs.size() != 1 || !isalpha(lhs[0])) { // checks the lhs (non-terminal): should be a single letter
                std::cerr << "Error: Invalid non-terminal '" << lhs << "' in line: " << line << std::endl;
                errorOccurred = true;
                break; 
            }

            if (arrow != "->") { // Checks if the format is correct (must have "->" after the lhs)
                std::cerr << "Error: Invalid grammar format. Expected 'Non-terminal -> Production ;'. Found: " << line << std::endl;
                errorOccurred = true;
                break; 
            }

            if (startSymbol.empty()) { // Adds lhs to the non-terminal list if not already present
                startSymbol = lhs;
            }

            if (std::find(nonTerminals.begin(), nonTerminals.end(), lhs) == nonTerminals.end()) {
                nonTerminals.push_back(lhs);
            }

            if (!getline(token, rhs, ';')) {  // Reads the right-hand side (rhs) until the semicolon
                std::cerr << "Error: Missing semicolon in production for '" << lhs << "' in line: " << line << std::endl;
                errorOccurred = true;
                break; 
            }

            std::istringstream rhsToken(rhs);  // Splits rhs into individual productions using spaces
            std::string production;
            while (rhsToken >> production) {
                if (production != "|") {
                    addProduction(lhs, production);   // Adds production to the list for the current lhs
                }
            }
        }

        // Reset the file stream for the second pass
        filename.clear();
        filename.seekg(0);

        std::string line2; 
        while (getline(filename, line2)) { // Goes through the file a second time to identify Terminals
            if (line2.empty()) {   
                continue; // Ignores empty lines
            }

            std::istringstream token(line2);  
            std::string lhs, arrow, rhs; 
            token >> lhs >> arrow; //Seperate lhs and arrow
            getline(token, rhs, ';'); // Read rhs and ignore the semicolon

            std::istringstream rhsToken(rhs);  // Splits rhs into individual productions using spaces
            std::string production;
            while (rhsToken >> production) {
                if (production != "|") {
                    for (char c : production) { 
                        if (isalpha(c) || c == '&') { // Validates each character in the production
                            std::string nt(1, c);
                            if (std::find(nonTerminals.begin(), nonTerminals.end(), nt) == nonTerminals.end()) { // Checks if the character is not a Nonterminal
                                if (find(terminals.begin(), terminals.end(),nt) == terminals.end() ) // Checks if the terminal is not already in the terminals vector
                                    {
                                    terminals.push_back(nt); // Adds symbol to the terminals vector
                                    }    
                            }
                        }else {
                            std::cerr << "Error: Invalid symbol '" << c << "' in production for '" << lhs << "' in line: " << line2 << std::endl;
                            errorOccurred = true;
                            break; 
                        }
                    }
                }
            }
            if (errorOccurred) {
                break;
            }
        }

        filename.close();

        if (!errorOccurred) {  // Only prints the grammar if no error occurred
            printGrammar(); 
        } else {
            clearGrammar(); 
        }
        
    }else {
        std::cerr << "Error: Unable to open file." << std::endl;
    }
}
//******************************************************************************

void Grammar::clearGrammar() {
    startSymbol.clear();
    terminals.clear();
    nonTerminals.clear();
    productions.clear();
}
//******************************************************************************

void Grammar::addProduction(std::string nonTerminal, const std::string& production) {
    productions[nonTerminal].push_back(production); // the production is added to the list of productions for a given non-terminal
}
//******************************************************************************

void Grammar::printGrammar() const {
    std::cout << "Start Symbol: " << startSymbol << std::endl;

    std::cout << "Terminals: ";
    for (size_t i = 0; i < terminals.size(); ++i) {
        std::cout << terminals[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Non-Terminals: ";
    for (size_t i = 0; i < nonTerminals.size(); ++i) {
        std::cout << nonTerminals[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Productions:\n";
    for (auto it = productions.begin(); it != productions.end(); ++it) {
        std::cout << it->first << " -> ";
        for (size_t i = 0; i < it->second.size(); ++i) {
            std::cout << it->second[i];
            if (i < it->second.size() - 1) { 
                std::cout << " | ";
            }
        }
        std::cout << std::endl;
    } 
}
//******************************************************************************

void Grammar::printFirst(){
    // Finds First for every Nonterminal in the Grammar and prints it
    for (size_t i = 0; i < nonTerminals.size(); ++i) {
        First(nonTerminals[i]);
        std::cout << "FIRST(" << nonTerminals[i] << ") = { ";
        
        for (size_t i = 0; i < firstSet.size(); ++i) {
            std::cout << firstSet[i];
            if (i < firstSet.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << " }" << std::endl;
        firstSet.clear(); //Clear the Set
    }

    // Finds First for every Terminal in the Grammar and prints it
    for (size_t i = 0; i < terminals.size(); ++i) {
        First(terminals[i]);
        std::cout << "FIRST(" << terminals[i] << ") = { ";
        
        for (size_t i = 0; i < firstSet.size(); ++i) {
            std::cout << firstSet[i];
            if (i < firstSet.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << " }" << std::endl;
        firstSet.clear(); //Clear the Set
    }
}
//******************************************************************************

std::string Grammar::First(const std::string& input) {
    Grammar::firstHelper(input);
    return "";
}
//******************************************************************************

void Grammar::firstHelper(const std::string& item) {
    for(size_t i = 0; i < item.size(); i++){
        std::string charStr(1, item[i]); // Converts charcater to string for comparison

        // Check if the first Character is a Nonterminal
        if(std::find(nonTerminals.begin(), nonTerminals.end(), charStr) != nonTerminals.end()){
            auto pd = productions[charStr]; // vector containing productions of the Noneterminal

            //Check if Nonterminal leads to an epsilon and the input is more than one charcater
            if(std::find(pd.begin(),pd.end(),"&") != pd.end() && item.size() > 1){
                for (size_t j = 0; j < pd.size(); j++){ // Loops through each production in pd and finds First of each production except epsilon
                    if(pd[j] != "&"){
                        firstHelper(pd[j]);
                    }
                }
                std::string remains = item.substr(1); // Initializes the variable to contain the rest of the input(excluding the first charcater)
                firstHelper(remains); // Finds First of remains since previous character leads to epsilon
                break;
            }else{

                // If item size is one we find the first for each production including epsilon(If part of the production)
                for(size_t j = 0; j < pd.size(); j++){
                    firstHelper(pd[j]);
                }
                break;
            }

            // If item is a terminal we add it to the first set.
        }else if(std::find(terminals.begin(), terminals.end(), charStr) != terminals.end()){
                if(std::find(firstSet.begin(), firstSet.end(), charStr) == firstSet.end()){
                    firstSet.push_back(charStr);
             }
             break;
        }else {
            std::cout << "Error: '" << item << "' is not in grammar. Cannot compute First" << std::endl; // Error message if symbol is not in the grammar
        }
    }
}




