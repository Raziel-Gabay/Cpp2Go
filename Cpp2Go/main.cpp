#include "lexer.h"
#include "parser.h"

int main()
{
	//std::string code = "int main()\n{\n\tint a = 5;\n\tfloat b = 2.71;\n\tbool c = true;\n}";
	//std::string code = "for(int i = 0; i < 10; i++)\n\t{}";
	std::string code = "int i = 0;\nif(i == 0)\n{\n\ti = 10;\n\ti = 20;\n}";
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
	parser p = parser(tokenStream);
	return 0;
}