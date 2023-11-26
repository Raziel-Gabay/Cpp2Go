#include <iostream>


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