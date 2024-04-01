#include "lexer.h"
#include "parser.h"
#include "AstTranslator.h"
#include "codeGenerator.h"
#include <chrono>
#include <fstream>
#include <restinio/all.hpp>

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

std::string translate(std::string& code) {
	lexer::preprocessing(code);
	tokensVector tokenStream;
	tokenStream = lexer::createTokenStream(code);
	parser p = parser(tokenStream);
	AstTranslator translator = AstTranslator(p.getAST());
	codeGenerator goCode = codeGenerator(translator.getAST());
	return goCode.getCode();
}

int main(int argc, char **argv)
{
	if (argc > 1 && std::string(argv[1]) == "-s") {
		restinio::run(
			restinio::on_this_thread()
			.port(8080)
			.address("localhost")
			.request_handler([](auto req) {
				if (restinio::http_method_post() == req->header().method() && req->header().request_target() == "/") {
					// Process POST request
					// Assuming the body is plain text
					auto body = req->body();
					std::string goCode = "";
					try {
						goCode = translate(body);
					}
					catch (const std::exception& e)
					{
						goCode = e.what();
					}

					// You can process the body here, e.g., parsing JSON, handling form data, etc.

					// Respond to the client
					req->create_response()
						.append_header(restinio::http_field::access_control_allow_origin, "*")
						.append_header(restinio::http_field::content_type, "text/plain")
						.set_body(goCode)	
						.done();

					return restinio::request_accepted();
				}

				// For other requests or paths
				return restinio::request_rejected();
				})
		);
	}
	else {
		std::string code = readCodeFromFile();
		try
		{
			std::string goCode = translate(code);
			std::cout << goCode << std::endl;
			writeToDestFile(goCode);
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what();
		}
	}
	
	
	
	return 0;
}