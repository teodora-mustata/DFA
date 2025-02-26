#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <string>
#include <utility>
#include <stack>
#include "HashUtils.h"

class NondeterministicFiniteAutomaton
{
private:
	std::unordered_set<std::string> Q; //multimea starilor
	std::unordered_set<std::string> sigma; //alfabetul de intrare
	std::unordered_map <std::pair<std::string, std::string>, std::unordered_set<std::string>, HashUtils::pair_hash, HashUtils::pair_equal > delta; //functia de tranzitie; cheia: pereche (stare1, caracter), valoarea: set de stari
	std::string q0; //starea initiala
	std::unordered_set<std::string> F; //multimea starilor finale
public:
	NondeterministicFiniteAutomaton(const std::string& initial_state);
	~NondeterministicFiniteAutomaton() = default;

	void removeFinalState(const std::string& state);
	void addTransition(const std::string& from, const std::string& symbol, const std::string& to);
	void addFinalState(const std::string& state);
	void addState(const std::string& state);

	std::unordered_set<std::string> GetStates() const;
	std::string GetInitialState() const;
	std::unordered_set<std::string> GetAlphabet() const;
	std::unordered_map <std::pair<std::string, std::string>, std::unordered_set<std::string>, HashUtils::pair_hash, HashUtils::pair_equal > GetTransitions() const;
	std::unordered_set<std::string> GetTransitions(const std::string& state, const std::string& symbol) const;
	std::unordered_set<std::string> GetFinalStates() const;
	bool IsFinalState(const std::string& state) const;

	void printNFA() const;
};

