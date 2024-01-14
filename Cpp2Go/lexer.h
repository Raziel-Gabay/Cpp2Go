#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <string>

#define FLOAT_POINT '.'
#define IDENTIFIER "IDENTIFIER"
#define POINTER "*"
#define STR_KEYWORD "std::string"
#define STR_LEN 11
#define STD_COUT_KEYWORD "std::cout"
#define STD_COUT_LEN 9
#define IF "if"
#define ELSE "else"
#define HASHTAG "#"
#define AND_OPERATOR "AND_OPERATOR"
#define ADDRESS_OF_OPERATOR "ADDRESS_OF_OPERATOR"

typedef std::pair<std::string, std::string> token;
typedef std::multimap<std::string, std::string> tokensMap;
typedef std::vector<std::pair<std::string, std::string>> tokensVector;
const std::unordered_set<std::string> operatorWithTwoCharsTokens = { "==", "!=", "<=", ">=", "++", "--", "&&", "||", "<<", ">>", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^="};
const std::unordered_set<std::string> standaloneTokens = { "+", "-", "*", "/", "=", "!", "<", ">", "&", "|", "^", "~", "%", "?", ":", ",", ";", ".", "(", ")", "{", "}", "[", "]" };
const std::unordered_set<std::string> UnaryOperators = { "++", "--" };

class lexer
{
public:
	static void preprocessing(std::string &sourceCode);
	static void deleteTab(std::string& sourceCode);
	static void deleteNewLine(std::string& sourceCode);
	static bool handleIdentifiers(std::string IdetifierToken);
	static bool handleIntLiteralValue(std::string intDataTypeToken);
	static bool handleFloatLiteralValue(std::string floatDataTypeToken);
	static bool handleStringLiteralValue(std::string stringDataTypeToken);
	static bool handleBoolLiteralValue(std::string boolDataTypeToken);
	static std::string getToken(std::string& code);
	static std::string elseCheck(std::string& code, size_t separatorPos, std::string token);
	static void floatCheck(std::string& code, size_t& separatorPos, std::string& token);
	static tokensMap::iterator searchToken(std::string token);
	static bool isTokensEqual(std::string token, tokensMap::iterator tokenFromMap);
	static void insertToken(std::string token, tokensMap::iterator tokenFromMap, tokensVector& codeTokensMap);
	static tokensVector createTokenStream(std::string& code);
};