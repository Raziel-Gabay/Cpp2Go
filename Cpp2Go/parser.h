#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

#define IDENTIFIER "IDENTIFIER"
#define ASSIGNMENT_OPERATOR "ASSIGNMENT_OPERATOR"
#define SEMICOLON "SEMICOLON"
#define LITERAL "LITERAL"


typedef std::vector<std::pair<std::string, std::string>> tokensVector;
typedef std::pair<std::string, std::string> token;

const std::unordered_set<std::string> ArithmeticOperators = { "+", "-", "*", "/"};
const std::unordered_set<std::string> RelationalOperators = { "==", "!=", "<", ">", "<=", ">="};
const std::unordered_set<std::string> LogicalOperators = { "&&", "||"};
const std::unordered_set<std::string> BitwiseOperators = { "&", "|", "^", "<<", ">>" };
const std::unordered_set<std::string> AssignmentOperators = { "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=" };
const std::unordered_set<std::string> AccessOperators = { ",", "->", ".", "[]" };





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
	void parseArithmeticOperator(const std::string& op);

	void parseRelationalOperator(const std::string& op);
	void parseLogicalOperator(const std::string& op);
	void parseBitwiseOperator(const std::string& op);
	void parseAssignmentOperator(const std::string& op);
	void parseAccessOperator(const std::string& op);


private:
	tokensVector _tokensStream;
	size_t _currentPosition;

	// Help Methods
	token getCurrentToken();
	void consumeToken();
	void unconsumeToken();
	bool isBinaryOperator(token t);
};

