#include "codeGenerator.h"

codeGenerator::codeGenerator(ASTNode* destNode)
	:_destNode(destNode), _countTab(0)
{
	generateCode(destNode);
}

codeGenerator::~codeGenerator()
{
	delete _destNode;
}

void codeGenerator::iterativeGenerate(ASTNode* node)
{
	if (node->name == PROGRAM)
	{
	}
	for (ASTNode* child : node->children)
	{
		if (child->name == FUNCTION_DECLARATION)
		{
			generateFunctionDeclaration(child);
		}
		else if (child->name == BLOCK)
		{
			generateBlock(child);
		}
		else if (child->name == VARIABLE_DECLARATION)
		{
			generateVariableDeclaration(child);
		}
		else if (child->name == POINTER_DECLARATION)
		{
			generatePointerDeclaration(child);
		}
		else if (child->name == IF_STATEMENT || child->name == ELSE_IF_STATEMENT || child->name == ELSE_STATEMENT ||
			child->name == WHILE_STATEMENT || child->name == FOR_STATEMENT)
		{
			generateStatement(child);
		}
		else if (child->name == STRUCT)
		{
			generateStruct(child);
		}
		else if (child->name == IMPORT_DIRECTIVE)
		{
			generateIncludeDirective(child);
		}
		else if (child->name == FUNCTION_CALL)
		{
			generateFunctionCall(child);
		}
		else if (child->name == ARRAY_DECLARATION)
		{
			generateArrayDeclaration(child);
		}
		else if (child->name == FMT_PRINTLN)
		{
			generateStdCout(child);
		}
		else
		{
			generateExpression(child);
		}
		_code += "\n";
	}
}

void codeGenerator::generateCode(ASTNode* node)
{
	iterativeGenerate(node);
}



void codeGenerator::generateFunctionCall(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == IDENTIFIER)
		{
			_code += child->value + "(";
		}
		else if (child->name == PARAMETER)
		{
			_code += child->children.front()->value;
			if (child != node->children.back())
				_code += ", ";
			else
			{
				if (_code.back() == WHITESPACE)
				{
					_code.pop_back();
					if (_code.back() == COMMA)
					{
						_code.pop_back();
					}
				}
				_code += ")";
			}				
		}
	}
}

void codeGenerator::generateStruct(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == STRUCT_KEYWORD)
		{
			_code += child->value + " {\n";
		}
		else if (child->name == MEMBERS)
		{
			for (ASTNode* member : child->children)
			{
				_code += "\t";
				_code += member->children.front()->value + " " + member->children.back()->value + "\n";
			}
			_code += "}";
		}
		else
			_code += child->value + " ";
	}
}

void codeGenerator::generateBlock(ASTNode* node)
{
	_code += " {\n";
	for (ASTNode* child : node->children)
	{
		_code += "\t" + std::string(_countTab, '\t');
		if (child->name == VARIABLE_DECLARATION)
		{
			generateVariableDeclaration(child);
		}
		else if (child->name == POINTER_DECLARATION)
		{
			generatePointerDeclaration(child);
		}
		else if (child->name == IF_STATEMENT || child->name == ELSE_IF_STATEMENT || child->name == ELSE_STATEMENT ||
			child->name == WHILE_STATEMENT || child->name == FOR_STATEMENT)
		{
			_countTab++;
			generateStatement(child);
		}
		else if (child->name == FUNCTION_CALL)
		{
			generateFunctionCall(child);
		}
		else if (child->name == ARRAY_DECLARATION)
		{
			generateArrayDeclaration(child);
		}
		else if (child->name == FMT_PRINTLN)
		{
			generateStdCout(child);
		}
		else
		{
			generateExpression(child);
		}
		_code += "\n";
	}
	_code += std::string(_countTab, '\t') + "}";
	if (_countTab > 0)
		_countTab--;
}

void codeGenerator::generateIncludeDirective(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == IMPORT)
		{
			_code += "import ";
		}
		else if (child->name == IDENTIFIER)
		{
			_code += '"';
			_code += child->value;
			_code += '"';
		}
	}
}


void codeGenerator::generateStdCout(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == PRINTLN)
		{
			_code += "fmt.Println";
		}
		if (child->name == STRING_LITERAL)
		{
			_code += "(";
			_code += child->value;
			_code += '")';
		}
	}
}

std::string codeGenerator::getCode()
{
	return _code;
}