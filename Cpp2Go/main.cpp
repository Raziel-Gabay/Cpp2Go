#include "lexer.h"

int main()
{
	std::string code = "int main()\n{\n\tint a = 5;\n\tfloat b = 2.71;\n\tbool c = true;\n}";
	std::cout << code << std::endl;
	lexer::preproccesing(code);
	std::cout << code << std::endl;
	tokensVector tokenStream;
	try
	{
		 tokenStream = lexer::createTokenStream(code);
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
	return 0;
}