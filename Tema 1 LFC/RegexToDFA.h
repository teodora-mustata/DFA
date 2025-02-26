#pragma once
#include "DeterministicFiniteAutomaton.h"
#include "NondeterministicFiniteAutomaton.h"
#include "RegexUtils.h"
#include<queue>

class RegexToDFA
{
public:
    static NondeterministicFiniteAutomaton buildNFA(const std::string& regex); // Construiește NFA dintr-o expresie regulată
    static DeterministicFiniteAutomaton convertNFAtoDFA(const NondeterministicFiniteAutomaton& nfa); // Convertește NFA în DFA
};

