#pragma once
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <tuple>
#include <string>
#include <utility>
#include "HashUtils.h"

class DeterministicFiniteAutomaton
{
private:
	std::unordered_set<std::string> Q; //multimea starilor
	std::unordered_set<std::string> sigma; //alfabetul de intrare
	std::unordered_map<std::pair<std::string, std::string>, std::string, HashUtils::pair_hash, HashUtils::pair_equal> delta; //functia de tranzitie; cheia: pereche (stare1, caracter), valoarea: stare2
	std::string q0; //starea initiala
	std::unordered_set<std::string> F; //multimea starilor finale

public:
	DeterministicFiniteAutomaton(const std::string& initial_state);
	DeterministicFiniteAutomaton() = default;
	~DeterministicFiniteAutomaton() = default;

	void SetInitialState(const std::string& state);
	void AddState(const std::string& state);
	void AddTransition(const std::string& from, const std::string& symbol, const std::string& to);
	void AddFinalState(const std::string& state);
	int GetStateCount() const;

	std::string GetInitialState() const;
	std::unordered_set<std::string> GetAlphabet() const;
	std::unordered_set<std::string> GetStates() const;
	std::unordered_set<std::string> GetFinalStates() const;
	std::unordered_map<std::pair<std::string, std::string>, std::string, HashUtils::pair_hash, HashUtils::pair_equal> GetTransitions() const;

	bool VerifyAutomaton() const;
	void PrintAutomaton() const;
	bool CheckWord(const std::string& word) const;

	void printDFA() const;
};

