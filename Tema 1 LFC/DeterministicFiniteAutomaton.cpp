#include "DeterministicFiniteAutomaton.h"
#include <fstream>
DeterministicFiniteAutomaton::DeterministicFiniteAutomaton(const std::string& initial_state) : q0(initial_state)
{
    Q.insert(q0);
}

bool DeterministicFiniteAutomaton::VerifyAutomaton() const
{
    // Verificarea că starea inițială există în mulțimea de stări
    if (Q.find(q0) == Q.end()) {
        std::cout << "Starea initiala nu este valida.\n";
        return false;
    }

    // 2. Verificăm că toate stările finale sunt în mulțimea de stări
    for (const std::string& finalState : F) {
        auto it = Q.find(finalState);
        if (it == Q.end()) {
            std::cout << "Stare finala invalida: '" << finalState << "'\n";
            return false;
        }
    }

    // 3. Verificăm că toate perechile (stare, simbol) din `delta` sunt definite în `Q` și `sigma`
    for (const auto& [key, targetState] : delta) {
        const auto& [state, symbol] = key;

        // Verificăm dacă `state` există în mulțimea `Q`
        if (Q.find(state) == Q.end()) {
            std::cout << "Stare invalida în functia de tranzitie: " << state << "\n";
            return false;
        }

        // Verificăm dacă `symbol` există în alfabetul `sigma`
        if (sigma.find(symbol) == sigma.end()) {
            std::cout << "Simbol necunoscut în functia de tranzitie: " << symbol << "\n";
            return false;
        }

        // Verificăm dacă `targetState` există în `Q`
        if (Q.find(targetState) == Q.end()) {
            std::cout << "Stare de destinatie invalida: " << targetState << "\n";
            return false;
        }
    }

    std::cout << "Automatul este valid.\n";
    return true;
}

void DeterministicFiniteAutomaton::PrintAutomaton() const
{
    std::ofstream out("automaton.txt");

    std::cout << std::endl << "      ||";
    out << std::endl << "     ||";

    for (const std::string& word : sigma) {
        std::cout << "  " << word << " ";
        out << "  " << word << " ";
    }
    std::cout << std::endl;
    out << std::endl;
    std::cout << "------++";
    out << "------++";
    for (const std::string& symbol : sigma) {
        std::cout << "----";
        out << "----";
    }
    std::cout << std::endl;
    out << std::endl;

    for (const std::string& state : Q) {
        std::string formattedState = state;
        if (state.size() == 2 && std::isdigit(state[1]) && state[1] <= '9') {
            formattedState += " ";
        }

        if (state == q0) {
            std::cout << "-> " << formattedState << "||";
            out << "-> " << formattedState << "||";
        }
        else if (F.find(state) != F.end()) {
            std::cout << " * " << formattedState << "||";
            out << " * " << formattedState << "||";
        }
        else {
            std::cout << "   " << formattedState << "||";
            out << "   " << formattedState << "||";
        }

        for (const std::string& symbol : sigma) {
            std::pair<std::string, std::string> key(state, symbol);
            auto transition = delta.find(key);
            if (transition != delta.end()) {
                std::cout << " " << transition->second << " ";
                out << " " << transition->second << " ";
            }
            else {
                std::cout << "    ";
                out << "    ";
            }
        }
        std::cout << std::endl;
        out << std::endl;
    }
}


bool DeterministicFiniteAutomaton::CheckWord(const std::string& word) const
{
    std::string currentState = q0; // Începem din starea inițială

    for (char c : word) {
        std::string symbol(1, c); // Convertim caracterul într-un string de lungime 1

        // Verificăm dacă simbolul face parte din alfabetul de intrare
        if (sigma.find(symbol) == sigma.end()) {
            std::cout << "Simbolul '" << symbol << "' nu face parte din alfabet.\n";
            return false;
        }

        // Căutăm tranziția pentru perechea (stare curentă, simbol)
        auto it = delta.find({ currentState, symbol });
        if (it == delta.end()) {
            std::cout << "Tranzitie lipsa pentru starea " << currentState
                << " cu simbolul " << symbol << "\n";
            return false;
        }

        // Actualizăm starea curentă la starea de destinație
        currentState = it->second;
    }

    // Verificăm dacă am ajuns într-o stare finală
    if (F.find(currentState) != F.end()) {
        std::cout << "Cuvantul '" << word<< "' este acceptat de automat.\n";
        return true;
    }
    else {
        std::cout << "Cuvantul '" << word << "' NU este acceptat de automat.\n";
        return false;
    }
}
void DeterministicFiniteAutomaton::SetInitialState(const std::string& state)
{
    if (Q.find(state) != Q.end()) {
        q0 = state; // Set the new initial state
    }
}

void DeterministicFiniteAutomaton::AddState(const std::string& state) {
    Q.insert(state);
}

std::string DeterministicFiniteAutomaton::GetInitialState() const {
    return q0;
}

std::unordered_set<std::string> DeterministicFiniteAutomaton::GetAlphabet() const {
    return sigma;
}

std::unordered_set<std::string> DeterministicFiniteAutomaton::GetStates() const {
    return Q;
}

std::unordered_set<std::string> DeterministicFiniteAutomaton::GetFinalStates() const {
    return F;
}

std::unordered_map<std::pair<std::string, std::string>, std::string, HashUtils::pair_hash, HashUtils::pair_equal> DeterministicFiniteAutomaton::GetTransitions() const {
    return delta;
}

void DeterministicFiniteAutomaton::AddTransition(const std::string& from, const std::string& symbol, const std::string& to) {
    delta[{from, symbol}] = to;
    sigma.insert(symbol);
}

void DeterministicFiniteAutomaton::AddFinalState(const std::string& state) {
    F.insert(state);
}

int DeterministicFiniteAutomaton::GetStateCount() const {
    return Q.size();
}

void DeterministicFiniteAutomaton::printDFA() const {
    // Tipărim mulțimea stărilor
    std::cout << "States (Q): { ";
    for (const auto& state : Q) {
        std::cout << state << " ";
    }
    std::cout << "}\n";

    // Tipărim alfabetul
    std::cout << "Alphabet (Sigma): { ";
    for (const auto& symbol : sigma) {
        std::cout << symbol << " ";
    }
    std::cout << "}\n";

    // Tipărim funcția de tranziție
    std::cout << "Transition function (Delta):\n";
    for (const auto& entry : delta) {
        std::cout << "(" << entry.first.first << ", " << entry.first.second << ") -> " << entry.second << "\n";
    }

    // Tipărim starea inițială
    std::cout << "Initial state (q0): " << q0 << "\n";

    // Tipărim stările finale
    std::cout << "Final states (F): { ";
    for (const auto& state : F) {
        std::cout << state << " ";
    }
    std::cout << "}\n";
}
