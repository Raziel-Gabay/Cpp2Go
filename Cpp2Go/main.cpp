#include <iostream>
#include <vector>

std::string deleteTab(std::string sourceCode)
{
	sourceCode.erase(std::remove(sourceCode.begin(), sourceCode.end(), '\t'), sourceCode.cend());
	return sourceCode;
}

std::string deleteNewLine(std::string sourceCode)
{
	sourceCode.erase(std::remove(sourceCode.begin(), sourceCode.end(), '\n'), sourceCode.cend());
	std::string code = deleteTab(sourceCode);
	return code;
}

bool handleIdentifiers(std::string IdetifierToken)
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

std::string getWord(std::string& code)
{
	std::string word;
	for (auto it = code.begin(); it != code.end(); ++it)
	{
		word += *it;
        if (*it == ' ')
		{
			code = std::string(it + 1, code.end());
			return word;
        }
	}
}

int main()
{
	std::string code = "int main()\n{\n\tint a = 5;\n}";

	return 0;
}