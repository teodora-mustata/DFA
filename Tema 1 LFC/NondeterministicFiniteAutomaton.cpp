#include "NondeterministicFiniteAutomaton.h"

NondeterministicFiniteAutomaton::NondeterministicFiniteAutomaton(const std::string& initial_state) : q0{ initial_state }
{
	Q.insert(initial_state);
}

void NondeterministicFiniteAutomaton::addTransition(const std::string& from, const std::string& symbol, const std::string& to)
{
    
    sigma.insert(symbol);
    delta[{from, symbol}].insert(to);

}
void NondeterministicFiniteAutomaton::removeFinalState(const std::string& state)
{
    auto it = std::find(F.begin(), F.end(), state);
    if (it != F.end()) {
        F.erase(it);
        //std::cout << "Removed final state: " << state << std::endl;
    }
    else {
        //std::cout << "State " << state << " was not a final state." << std::endl;
    }
}

void NondeterministicFiniteAutomaton::addFinalState(const std::string& state)
{
    if (F.find(state) == F.end()) {
        F.insert(state);
    }
}

void NondeterministicFiniteAutomaton::addState(const std::string& state)
{
    Q.insert(state);
}


std::string NondeterministicFiniteAutomaton::GetInitialState() const {
    return q0;
}

std::unordered_set<std::string> NondeterministicFiniteAutomaton::GetAlphabet() const 
{
    return sigma;
}

std::unordered_map <std::pair<std::string, std::string>, std::unordered_set<std::string>, HashUtils::pair_hash, HashUtils::pair_equal > NondeterministicFiniteAutomaton::GetTransitions() const {
    return delta; 
}

std::unordered_set<std::string> NondeterministicFiniteAutomaton::GetTransitions(const std::string& state, const std::string& symbol) const {
    auto it = delta.find({ state, symbol });
    if (it != delta.end()) {
        return it->second;  // Returnează setul de stări pentru tranziția găsită
    }
    return {};  // Dacă nu există tranziții, returnează un set gol
}

std::unordered_set<std::string> NondeterministicFiniteAutomaton::GetFinalStates() const
{
    return F;
}

bool NondeterministicFiniteAutomaton::IsFinalState(const std::string& state) const
{
    return F.find(state) != F.end();
}
std::unordered_set<std::string> NondeterministicFiniteAutomaton::GetStates() const
{
    return Q;
}

void NondeterministicFiniteAutomaton::printNFA() const {
    // 1. Print stările
    std::cout << "States (Q):" << std::endl;
    for (const auto& state : Q) {
        std::cout << state << " ";
    }
    std::cout << std::endl;

    // 2. Print alfabetul de intrare
    std::cout << "Alphabet (Sigma):" << std::endl;
    for (const auto& symbol : sigma) {
        std::cout << symbol << " ";
    }
    std::cout << std::endl;

    // 3. Print tranzițiile
    std::cout << "Transitions (Delta):" << std::endl;
    for (const auto& transition : delta) {
        const auto& key = transition.first;
        const auto& states = transition.second;
        std::cout << "(" << key.first << ", " << key.second << ") -> { ";
        for (const auto& state : states) {
            std::cout << state << " ";
        }
        std::cout << "}" << std::endl;
    }

    // 4. Print starea inițială
    std::cout << "Initial state (q0): " << q0 << std::endl;

    // 5. Print stările finale
    std::cout << "Final states (F):" << std::endl;
    for (const auto& finalState : F) {
        std::cout << finalState << " ";
    }
    std::cout << std::endl;
}