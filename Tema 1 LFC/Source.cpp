#include <iostream>
#include <fstream>
#include "RegexToDFA.h"
#include "DeterministicFiniteAutomaton.h"
#include "NondeterministicFiniteAutomaton.h"


std::string readRegexFromFile(const std::string& filePath) 
{
    std::ifstream inputFile(filePath);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Eroare: Nu se poate deschide fișierul " + filePath);
    }

    std::string regex;
    if (!std::getline(inputFile, regex)) {
        throw std::runtime_error("Eroare: Fișierul este gol sau nu s-a putut citi expresia regulată.");
    }

    inputFile.close();
    return regex;
}

int main() {

    std::string regex = readRegexFromFile("regex.txt");

    NondeterministicFiniteAutomaton nfa = RegexToDFA::buildNFA(regex);
    std::cout << "NFA: \n";
    nfa.printNFA();
    DeterministicFiniteAutomaton dfa = RegexToDFA::convertNFAtoDFA(nfa);
    std::cout << "DFA: \n";
    dfa.printDFA();

    dfa.PrintAutomaton();
    dfa.VerifyAutomaton();
    dfa.CheckWord("baabb");


    // a.b.a.(a.a|b.b)*
    // abaaa

  
    return 0;
}