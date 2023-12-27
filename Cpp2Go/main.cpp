#include "lexer.h"
#include "parser.h"
#include "AstTranslator.h"
#include "codeGenerator.h"
void printAST(const ASTNode* node, int depth = 0) {
	if (node) {
		// Print current node
		for (int i = 0; i < depth; ++i) {
			std::cout << "  "; // Adjust indentation for depth
		}
		std::cout << node->name << std::endl;

		// Recursively print children
		for (const ASTNode* child : node->children) {
			printAST(child, depth + 1);
		}
	}
}

int main()
{
	//std::string code = "int a = 5;\n\tfloat b = 2.71;\n\tbool c = true;\n";
	std::string code = "int i = 10;\nstd::string str = \"hello\";\nfor(int i = 0; i < 10; i++)\n{\n\tif(i == 0)\n\t{\n\t\ti = 1;\n\t\ti = 2;\n\t}\n}";
	//std::string code = "int i = 0;\nif(i == 0)\n{\n\ti = 10;\n\ti = 20;\n}";
	std::cout << code << std::endl;
	lexer::preprocessing(code);
	std::cout << code << std::endl;
	tokensVector tokenStream;
	try
	{
		 tokenStream = lexer::createTokenStream(code); 
		 parser p = parser(tokenStream); 
		 printAST(p.getAST());  
		 AstTranslator translator = AstTranslator(p.getAST());
		 printAST(translator.getAST());
		 codeGenerator goCode = codeGenerator(translator.getAST());
		 std::cout << goCode.getCode() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
	return 0;
}