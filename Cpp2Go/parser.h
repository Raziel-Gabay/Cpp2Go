#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

typedef std::vector<std::pair<std::string, std::string>> tokensVector;
typedef std::pair<std::string, std::string> token;

class parser
{
public:
	// Constructor
	parser(tokensVector tokenStream);

	// Desturctor
	~parser();

	// Parse Functions
	void parseProgram();
	void parseDeclaration();
	void parseStatement();
	void parseExpression();
	void parseType();

	// Help Methods
	token getCurrentToken();
	void consumeToken();
	bool isBinaryOperator(token t);
private:
	tokensVector _tokensStream;
	size_t _currentToken;
};

