#include "RegexUtils.h"
#include<iostream>
bool RegexUtils::isOperand(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

int RegexUtils::priority(char op)
{
    if (op == '*') return 3;
    if (op == '.') return 2;
    if (op == '|') return 1;
    return 0;
}

std::string RegexUtils::toPostfix(const std::string& regex)
{
    std::string postfix;
    std::stack<char> opStack;

    for (size_t i = 0; i < regex.length(); ++i) {
        char c = regex[i];

        // Dacă este operand
        if (isOperand(c)) {
            postfix += c;
        }
        // Dacă este o paranteză deschisă
        else if (c == '(') {
            opStack.push(c);
        }
        // Dacă este o paranteză închisă, scoatem operatorii de pe stivă până la paranteza deschisă.
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.pop();  // Scoatem paranteza deschisă
        }
        else if (c == '*') {
            postfix += c;  // Just add '*' directly to the postfix expression
        }
        // Dacă este un operator
        else {
            // Ensure concatenation operator (.) is inserted between operands that are next to each other
            if (i > 0 && isOperand(regex[i - 1]) && isOperand(c)) {
                postfix += '.';  // Insert concatenation operator (.)
            }
            while (!opStack.empty() && priority(opStack.top()) >= priority(c)) {
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
        }
       
    }
    while (!opStack.empty()) {
        postfix += opStack.top();
        opStack.pop();
    }
    return postfix;

  
   
}
