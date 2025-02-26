#include "RegexToDFA.h"

struct set_hash {
    template <typename T>
    size_t operator()(const std::unordered_set<T>& s) const {
        size_t seed = 0;
        for (const auto& elem : s) {
            seed ^= std::hash<T>{}(elem)+0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

struct set_equal {
    template <typename T>
    bool operator()(const std::unordered_set<T>& lhs, const std::unordered_set<T>& rhs) const {
        return lhs == rhs;
    }
};

NondeterministicFiniteAutomaton RegexToDFA::buildNFA(const std::string& regex) {
    RegexUtils utils;
    std::string postfix = utils.toPostfix(regex);
    std::cout << "Postfix Expression: " << postfix << std::endl;
    NondeterministicFiniteAutomaton nfa("q0");

    std::stack<std::pair<std::string, std::string>> stateStack; // Pair of (startState, endState)
    int stateCounter = 0;

    for (char c : postfix) {
        //std::cout << "Processing token: " << c << std::endl;
        if (utils.isOperand(c)) {
            std::string startState = "q" + std::to_string(stateCounter++);
            std::string endState = "q" + std::to_string(stateCounter++);

            nfa.addState(startState);
            nfa.addState(endState);
            
            nfa.addTransition(startState, std::string(1, c), endState);
            nfa.addFinalState(endState);
           
            stateStack.push({ startState, endState });
            //std::cout << "Added operand transition: " << startState << " --" << c << "--> " << endState << std::endl;
        }
        else if (c == '*') {
            auto [start, end] = stateStack.top();
            stateStack.pop();

            std::string newStart = "q" + std::to_string(stateCounter++);
            std::string newEnd = "q" + std::to_string(stateCounter++);

            // Add epsilon transitions for Kleene star
            nfa.addState(newStart);
            nfa.addState(newEnd);
            nfa.addTransition(newStart, "#", start);  // ε-transition to the original NFA
            nfa.addTransition(end, "#", newEnd);      // ε-transition to the new end state
            nfa.addTransition(newStart, "#", newEnd); // ε-transition directly to the new end state
            nfa.addTransition(end, "#", start);       // Loop back from end to start


            nfa.removeFinalState(end);              // Original end is no longer final
            nfa.addFinalState(newEnd);              // New end becomes the final state

            stateStack.push({ newStart, newEnd });
            //std::cout << "Created Kleene star structure: " << newStart << " ->* " << newEnd << std::endl;

        }
        else if (c == '.') {
            // Concatenation logic - combine two NFAs
            auto [start2, end2] = stateStack.top();
            stateStack.pop();
            auto [start1, end1] = stateStack.top();
            stateStack.pop();


            nfa.addTransition(end1, "#", start2); // ε-transition to connect the two NFAs

            nfa.removeFinalState(end1);         // First NFA's final state is no longer final
            nfa.addFinalState(end2);            // Second NFA's final state becomes the final state

            stateStack.push({ start1, end2 });
            //std::cout << "Concatenated NFAs: " << start1 << " -> " << end2 << std::endl;

        }
        else if (c == '|') {
            auto [start2, end2] = stateStack.top();
            stateStack.pop();
            auto [start1, end1] = stateStack.top();
            stateStack.pop();

            std::string newStart = "q" + std::to_string(stateCounter++);
            std::string newEnd = "q" + std::to_string(stateCounter++);

            nfa.addState(newStart);
            nfa.addState(newEnd);
            nfa.addTransition(newStart, "#", start1); // ε-transition to the first NFA
            nfa.addTransition(newStart, "#", start2); // ε-transition to the second NFA
            nfa.addTransition(end1, "#", newEnd);     // ε-transition from the first NFA to the new end state
            nfa.addTransition(end2, "#", newEnd);     // ε-transition from the second NFA to the new end state

            nfa.removeFinalState(end1);             // Original finals are no longer final
            nfa.removeFinalState(end2);
            nfa.addFinalState(newEnd);
            stateStack.push({ newStart, newEnd });
            //std::cout << "Created union structure: " << newStart << " | " << newEnd << std::endl;

        }
    }
    auto [initial, final] = stateStack.top();
    stateStack.pop();

    nfa.addFinalState(final);
    //std::cout << "Final NFA has start state: " << initial << " and final state: " << final << std::endl;


    return nfa;
}

std::unordered_set<std::string> computeLambdaClosure(const NondeterministicFiniteAutomaton& nfa, const std::unordered_set<std::string>& states) {
    std::unordered_set<std::string> closure = states;
    std::stack<std::string> stack;

    for (const auto& state : states) {
        stack.push(state);
    }

    while (!stack.empty()) {
        std::string current = stack.top();
        stack.pop();

        // Get all epsilon transitions from the current state
        for (const auto& next : nfa.GetTransitions(current, "#")) {
            if (closure.find(next) == closure.end()) {
                closure.insert(next);
                stack.push(next);
            }
        }
    }

    return closure;
}

std::unordered_set<std::string> computeMove(const NondeterministicFiniteAutomaton& nfa, const std::unordered_set<std::string>& states, const std::string& symbol) {
    std::unordered_set<std::string> result;

    for (const auto& state : states) {
        // Get all transitions for the given symbol
        for (const auto& next : nfa.GetTransitions(state, symbol)) {
            result.insert(next);
        }
    }

    return result;
}

DeterministicFiniteAutomaton RegexToDFA::convertNFAtoDFA(const NondeterministicFiniteAutomaton& nfa) {

    DeterministicFiniteAutomaton dfa;
   
    // Subset construction logic
    std::unordered_map<std::unordered_set<std::string>, std::string, set_hash, set_equal> stateMap;
    std::unordered_set<std::string> initialSet = { nfa.GetInitialState() }; // Set de stări pentru NFA
    std::unordered_set<std::string> initialClosure = computeLambdaClosure(nfa, initialSet);
    
    std::string initialStateName = "q0";
    stateMap[initialClosure] = initialStateName;
    dfa.AddState(initialStateName);
    dfa.SetInitialState(initialStateName);

 
    //Queue to process DFA states
    std::queue<std::unordered_set<std::string>> unprocessedSets;
    unprocessedSets.push(initialClosure);


    while (!unprocessedSets.empty()) {
        auto currentSet = unprocessedSets.front();
        unprocessedSets.pop();
        std::string currentStateName = stateMap[currentSet];

                        //std::cout << "Mapping NFA sets to DFA states:" << std::endl;
                        //for (const auto& entry : stateMap) {
                        //    std::cout << "{ ";
                        //    for (const auto& state : entry.first) std::cout << state << " ";
                        //    std::cout << "} -> " << entry.second << std::endl;
                        //}

                        //std::cout << "Transitions in DFA:" << std::endl;
                        //for (const auto& transition : dfa.GetTransitions()) {
                        //    std::cout << transition.first.first << " --" << transition.first.second << "--> " << transition.second << std::endl;
                        //}

        // Check if the current set contains any NFA final state
        for (const auto& state : currentSet) {
            if (nfa.IsFinalState(state)) {
                dfa.AddFinalState(currentStateName);
                break;
            }
        }

        // Process transitions for each symbol in the NFA's alphabet
        for (const std::string& symbol : nfa.GetAlphabet()) {
            if (symbol == "#") continue; // Skip epsilon transitions

            // Compute the set of states reachable by this symbol
            std::unordered_set<std::string> moveResult = computeMove(nfa, currentSet, symbol);

            // Compute the lambda-closure of the reachable states
            std::unordered_set<std::string> nextClosure = computeLambdaClosure(nfa, moveResult);

            // Skip empty closures
            if (nextClosure.empty()) continue;

            // Check if this closure already has a DFA state
            if (stateMap.find(nextClosure) == stateMap.end()) {
                std::string nextStateName = "q" + std::to_string(dfa.GetStateCount());
                stateMap[nextClosure] = nextStateName;
                dfa.AddState(nextStateName);
                unprocessedSets.push(nextClosure);

 /*                       std::cout << "Added to unprocessedSets: { ";
                        for (const auto& state : nextClosure) {
                            std::cout << state << " ";
                        }
                        std::cout << "}" << std::endl;*/
            }

            // Add the DFA transition
            dfa.AddTransition(currentStateName, symbol, stateMap[nextClosure]);
        }
    }

    return dfa;
}

