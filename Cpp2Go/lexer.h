#pragma once
#include <iostream>
#include <vector>
#include <unordered_set>
#include <map>
#include <string>

#define FLOAT_POINT '.'
typedef std::multimap<std::string, std::string> tokensMap;
typedef std::vector<std::pair<std::string, std::string>> tokensVector;
const std::unordered_set<std::string> standaloneTokens = { "+", "-", "*", "/", "=", "!", "<", ">", "&", "|", "^", "~", "%", "?", ":", ",", ";", ".", "(", ")", "{", "}", "[", "]" };

class lexer
{
public:
	static void preproccesing(std::string &sourceCode);
	static void deleteTab(std::string& sourceCode);
	static void deleteNewLine(std::string& sourceCode);
	static bool handleIdentifiers(std::string IdetifierToken);
	static bool handleIntLiteralValue(std::string intDataTypeToken);
	static bool handleFloatLiteralValue(std::string floatDataTypeToken);
	static bool handleStringLiteralValue(std::string stringDataTypeToken);
	static bool handleBoolLiteralValue(std::string boolDataTypeToken);
	static std::string getToken(std::string& code);
	static tokensMap::iterator searchToken(std::string token);
	static bool isTokensEqual(std::string token, tokensMap::iterator tokenFromMap);
	static void insertToken(std::string token, tokensMap::iterator tokenFromMap, tokensVector& codeTokensMap);
	static tokensVector createTokenStream(std::string& code);
};