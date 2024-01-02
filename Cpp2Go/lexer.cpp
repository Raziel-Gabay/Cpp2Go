#include "lexer.h"

tokensMap mapOfTokens =
{
	{"int", "DATATYPE_INT"},
	{"float", "DATATYPE_FLOAT"},
	{"bool", "DATATYPE_BOOL"},
	{"void", "DATATYPE_VOID"},
	{"std::string", "DATATYPE_STRING"},
	{"identifier", "IDENTIFIER"},
	{"intLiteral", "INT_LITERAL"},
	{"floatLiteral", "FLOAT_LITERAL"},
	{"stringLiteral", "STRING_LITERAL"},
	{"boolLiteral", "BOOL_LITERAL"},
	{"while", "WHILE_STATEMENT"},
	{"for", "FOR_STATEMENT"},
	{"if", "IF_STATEMENT"},
	{"else if", "ELSE_IF_STATEMENT"},
	{"else", "ELSE_STATEMENT"},
	{"(", "LEFT_PARENTHESIS"},
	{")", "RIGHT_PARENTHESIS"},
	{"{", "LEFT_BRACE"},
	{"}", "RIGHT_BRACE"},
	{"=", "ASSIGNMENT_OPERATOR"},
	{";", "SEMICOLON"},
	{"%", "MODULO_OPERATOR"},
	{"!=", "NOT_EQUAL_OPERATOR"},
	{"==", "EQUAL_OPERATOR"},
	{"<", "LESS_THAN_OPERATOR"},
	{">", "MORE_THAN_OPERATOR"},
	{"&&", "AND_OPERATOR"},
	{"||", "OR_OPERATOR"},
	{"++", "INCREMENT_OPERATOR"},
	{"--", "DECREMENT_OPERATOR"}
};

void lexer::preprocessing(std::string& sourceCode)
{
	deleteTab(sourceCode);
	deleteNewLine(sourceCode);
}

void lexer::deleteTab(std::string& sourceCode)
{
	sourceCode.erase(std::remove(sourceCode.begin(), sourceCode.end(), '\t'), sourceCode.cend());
}

void lexer::deleteNewLine(std::string& sourceCode)
{
	sourceCode.erase(std::remove(sourceCode.begin(), sourceCode.end(), '\n'), sourceCode.cend());
}

bool lexer::handleIdentifiers(std::string IdetifierToken)
{
	bool isValidIdentifier = true;
	bool isALetter = isalpha(IdetifierToken[0]);
	//make sure the first char is a letter or '_'
	if ((IdetifierToken[0] != '_') && (isALetter == false))
	{
		isValidIdentifier = false;
	}
	//check if there are no special chars in the string
	if (IdetifierToken[strspn(IdetifierToken.c_str(), "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_")] != 0)
	{
		isValidIdentifier = false;
	}
	//make sure there are no white spaces in the identifier string
	for (int i = 0; i < IdetifierToken.size(); i++)
	{
		if (IdetifierToken[i] == ' ')
			isValidIdentifier = false;
	}
	//make sure the identifier doesnt has a keyword name
	char keyWordList[49][10] = { "asm", "auto", "break", "case", "catch", "char", "class", "const", "continue", "default", "delete", "do", "double", "else", "enum", "extern", "float", "for", "friend", "goto", "if", "inline", "int", "long", "new", "operator", "private", "protected", "public", "register", "return", "short", "signed", "sizeof", "static", "string", "struct", "switch", "template", "this", "throw", "try", "typedef", "union", "unsigned", "virtual", "void", "volatile", "while" };
	for (int i = 0; i < 49; i++)
	{
		//comparing the name to all keywords in the list
		if (strcmp(IdetifierToken.c_str(), keyWordList[i]) == 0)
		{
			isValidIdentifier = false;
		}
	}
	return isValidIdentifier;
}

bool lexer::handleIntLiteralValue(std::string intDataTypeToken)
{
	int intValueOfToken = std::stoi(intDataTypeToken);
	//make sure the value is in the range of cpp language
	if ((intValueOfToken > INT_MAX) || (intValueOfToken < INT_MIN))
		return false;
	//make sure the value is numeric
	for (int i = 0; i < intDataTypeToken.length(); i++)
	{
		if (!isdigit(intDataTypeToken[i]))
		{
			return false;
		}
	}
	return true;
}

bool lexer::handleFloatLiteralValue(std::string floatDataTypeToken)
{
	float floatValueOfToken = std::stof(floatDataTypeToken);
	//make sure the float number is in the range
	if ((floatValueOfToken > FLT_MAX) || (floatValueOfToken < FLT_MIN))
		return false;

	int pointCount = 0;
	bool isFloat = false;
	//make sure the value is float
	for (int i = 0; i < floatDataTypeToken.length(); i++)
	{
		if (floatDataTypeToken[i] == '.')
		{
			pointCount++;
		}
		else if (isdigit(floatDataTypeToken[i]))
		{
			isFloat = true;
		}
		else
		{
			return false;
		}
	}
	if (pointCount != 1)
	{
		isFloat = false;
	}
	return isFloat;
}

bool lexer::handleStringLiteralValue(std::string stringDataTypeToken)
{
	if (stringDataTypeToken[0] != '"' || (stringDataTypeToken[stringDataTypeToken.length() - 1]) != '"')
	{
		if (stringDataTypeToken[0] != '\'' || (stringDataTypeToken[stringDataTypeToken.length() - 1]) != '\'')
			return false;
	}
	return true;
}

bool lexer::handleBoolLiteralValue(std::string boolDataTypeToken)
{
	if (boolDataTypeToken != "false" && boolDataTypeToken != "true")
		return false;
	return true;
}

/*
input: string that repersent the code
output: string that reprsent a token
This function will get the first token of the code and then remove it and spaces that may come afterwards
*/
std::string lexer::getToken(std::string& code)
{
	std::string token = "";
	// Check if the input string is empty
	if (code.empty()) {
		return token;
	}

	// check if the token is string
	token = code.substr(0, STR_LEN);
	if ( token == STR)
	{
		code.erase(0, STR_LEN + 1);
		return token;
	}


	// Get the 2 first characters
	token = code.substr(0, 2);
	if (operatorWithTwoCharsTokens.count(token) > 0)
	{
		code.erase(0, 2); // Remove the processed token
		if (code[0] == ' ')
		{
			code.erase(0, 1); // Remove the whitespace that may come afterward
		}
		return token;
	}
	// Get the first character
	token = code[0];
	
	if (token == "\"")
	{
		code.erase(0, 1);
		auto separatorPos = code.find_first_of("\"");
		token += code.substr(0, separatorPos + 1);
		code.erase(0, separatorPos + 1);
		return token;
	}

	// If the character is a standalone token, return it as a separate token
	if (standaloneTokens.count(token) > 0) {
		code.erase(0, 1); // Remove the processed token
		if (code[0] == ' ')
		{
			code.erase(0, 1); // Remove the whitespace that may come afterward
		}
		return token;
	}

	// Find the position of the first separator (including newline characters)
	auto separatorPos = code.find_first_of(" ,.;:{}[]<>()");

	// Extract the token up to the first separator (or the end of the string if no separator found)
	token = code.substr(0, separatorPos);

	//handling unary operator
	if (token.length() > 2)
	{
		if (UnaryOperators.count(token.substr(token.length() - 2)) > 0)
		{
			token = code.substr(0, separatorPos - 2);
			code.erase(0, separatorPos - 2);
			return token;
		}
	}
	
	if (token == ELSE)
	{
		if (separatorPos != std::string::npos && standaloneTokens.find(code.substr(separatorPos, 1)) == standaloneTokens.end())
		{
			code.erase(0, separatorPos + 1);
		}
		else
		{
			code.erase(0, separatorPos);
		}
		auto separatorPos = code.find_first_of(" ,.;:{}[]<>()");
		std::string next_token = code.substr(0, separatorPos);
		if (next_token == IF)
		{
			token += " " + next_token;
			code.erase(0, separatorPos + 1);
			return token;
		}
		else
		{
			return token;
		}

	}
	//handling float
	if (code[separatorPos] == FLOAT_POINT && std::stoi(token))
	{
		token += FLOAT_POINT;
		code.erase(0, separatorPos + 1);
		separatorPos = code.find_first_of(" ,.;:{}[]<>()");
		token += code.substr(0, separatorPos);
	}
	// Remove the processed token (including the separator, if any)
	if (separatorPos != std::string::npos && standaloneTokens.find(code.substr(separatorPos, 1)) == standaloneTokens.end())
	{
		code.erase(0, separatorPos + 1);
	}
	else
	{
		code.erase(0, separatorPos);
	}
	return token;
}

tokensMap::iterator lexer::searchToken(std::string token)
{
	return mapOfTokens.find(token);
}

bool lexer::isTokensEqual(std::string token, tokensMap::iterator tokenFromMap)
{
	return (tokenFromMap != mapOfTokens.end());
}

void lexer::insertToken(std::string token, tokensMap::iterator tokenFromMap, tokensVector& codeTokensMap)
{
	codeTokensMap.push_back({ token, tokenFromMap->second });
}

tokensVector lexer::createTokenStream(std::string& code)
{
	tokensVector tokenStream;
	while (!code.empty())
	{
		std::string token = getToken(code);
		tokensMap::iterator tokenFromMap = searchToken(token);
		if (isTokensEqual(token, tokenFromMap))
		{
			insertToken(token, tokenFromMap, tokenStream);
		}
		else if (handleBoolLiteralValue(token))
		{
			tokenFromMap = searchToken("boolLiteral");
			insertToken(token, tokenFromMap, tokenStream);
		}
		else if (handleIdentifiers(token))
		{
			tokenFromMap = searchToken("identifier");
			insertToken(token, tokenFromMap, tokenStream);
		}
		else if (handleStringLiteralValue(token))
		{
			tokenFromMap = searchToken("stringLiteral");
			insertToken(token, tokenFromMap, tokenStream);
		}
		else if (handleIntLiteralValue(token))
		{
			tokenFromMap = searchToken("intLiteral");
			insertToken(token, tokenFromMap, tokenStream);
		}
		else if (handleFloatLiteralValue(token))
		{
			tokenFromMap = searchToken("floatLiteral");
			insertToken(token, tokenFromMap, tokenStream);
		}
		else
		{
			throw std::runtime_error("ERROR: didn't find any token by this name...");
		}
	}
	return tokenStream;
}

