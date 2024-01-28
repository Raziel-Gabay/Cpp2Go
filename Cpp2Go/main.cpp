#include "lexer.h"
#include "parser.h"
#include "AstTranslator.h"
#include "codeGenerator.h"
#include <chrono>
#include <fstream>

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
std::string readCodeFromFile()
{
	std::string code = "";
	std::fstream cppTextFile;
	cppTextFile.open("cppCode.txt", std::ios::in); //open the file to read
	if (cppTextFile.is_open())
	{	//checking whether the file is open
		std::string data;
		while (getline(cppTextFile, data)) //read data from the text file
		{
			code += data + "\n";
		}
		cppTextFile.close(); //close the file object
	}
	return code;
}

void writeToDestFile(std::string goCode)
{
	std::ofstream goTextFile("goCode.txt");
	if (goTextFile.is_open())
	{
		goTextFile << goCode;
		goTextFile.close(); //close the file object
	}
}

int main()
{

	std::string code = readCodeFromFile();
	std::cout << code << std::endl;
	lexer::preprocessing(code);
	std::cout << code << std::endl;
	tokensVector tokenStream;
	try
	{
		// Start the timer
		auto start = std::chrono::high_resolution_clock::now();

		tokenStream = lexer::createTokenStream(code);

		// End the timer
		auto end = std::chrono::high_resolution_clock::now();

		// Calculate the duration
		std::chrono::duration<double> duration = end - start;

		// Print the duration in seconds
		std::cout << "Execution time: " << duration.count() << " seconds." << std::endl;

		 parser p = parser(tokenStream); 
		 printAST(p.getAST());  
		 AstTranslator translator = AstTranslator(p.getAST());
		 printAST(translator.getAST());
		 codeGenerator goCode = codeGenerator(translator.getAST());
		 std::cout << goCode.getCode() << std::endl;
		 writeToDestFile(goCode.getCode());
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what();
	}
	return 0;
}