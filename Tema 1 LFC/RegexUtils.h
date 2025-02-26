#pragma once
#include <string>
#include <stack>

class RegexUtils
{
public:
	bool isOperand(char c);
	int priority(char op);
	std::string toPostfix(const std::string& regex);
	
};

