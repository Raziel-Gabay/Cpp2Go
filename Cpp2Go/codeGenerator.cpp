#include "codeGenerator.h"

codeGenerator::codeGenerator(ASTNode* destNode)
	:_destNode(destNode), _countTab(0)
{
	generateCode(destNode);
}

codeGenerator::~codeGenerator()
{
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
		else if (child->name == DECLARATION)
		{
			generateDeclaration(child);
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

void codeGenerator::generateDeclaration(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == ASSIGNMENT_OPERATOR || child->name == SHORT_ASSIGNMENT_OPERATOR)
		{
			generateExpression(child);
		}
		else
		{
			_code += child->value + " ";
		}
	}
}

void codeGenerator::generateFunctionDeclaration(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == IDENTIFIER)
		{
			_code += child->value + " (";
		}
		else if (child->name == PARAMETER)
		{
			_code += child->children.back()->value + " " + child->children.front()->value + ", ";
		}
		else if (child->name == RETURN_VALUE)
		{
			if (_code.back() == WHITESPACE)
			{
				_code.pop_back();
				if (_code.back() == COMMA)
				{
					_code.pop_back();
				}
			}
			_code += ") " + child->children.front()->value;
		}
		else if (child->name == BLOCK)
		{
			if (node->children[1]->value == "main")
				_code += ")";
			else if (node->children.size() > 2)
			{
				if (node->children[node->children.size() - 2]->name != RETURN_VALUE)
				{
					if (_code.back() == WHITESPACE)
					{
						_code.pop_back();
						if (_code.back() == COMMA)
						{
							_code.pop_back();
						}
						_code += ")";
					}
				}
			}
			generateBlock(child);
		}
		else
		{
			_code += child->value + " ";
		}
	}
}

void codeGenerator::generateFunctionCall(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == IDENTIFIER)
		{
			_code += child->value + " (";
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

void codeGenerator::generateStatement(ASTNode* node)
{
	if (node->name == IF_STATEMENT)
		generateIfStatement(node);
	else if(node->name == ELSE_IF_STATEMENT)
	{ 
		_code.erase(_code.size() - _countTab);
		_code.pop_back();
		_code += " ";
		generateElseIfStatement(node);
	}
	else if (node->name == ELSE_STATEMENT)
	{
		_code.erase(_code.size() - _countTab);
		_code.pop_back();
		_code += " ";
		generateElseStatement(node);
	}
	else if (node->name == WHILE_STATEMENT)
		generateWhileStatement(node);
	else
		generateForStatement(node);
}

void codeGenerator::generateIfStatement(ASTNode* node)
{
	_code += "if ";
	for (ASTNode* child : node->children)
	{
		if (child->name == CONDITION)
		{
			generateExpression(child->children);
		}
		else if (child->name == BLOCK)
		{
			generateBlock(child);
		}
	}
}

void codeGenerator::generateElseIfStatement(ASTNode* node)
{
	_code += "else if ";
	for (ASTNode* child : node->children)
	{
		if (child->name == CONDITION)
		{
			generateExpression(child->children);
		}
		else if (child->name == BLOCK)
		{
			generateBlock(child);
		}
	}
}

void codeGenerator::generateElseStatement(ASTNode* node)
{
	_code += "else ";
	generateBlock(node->children.front());
}

void codeGenerator::generateWhileStatement(ASTNode* node)
{
	_code += "for "; //in go, the while loop is represented by the 'for' keyword
	for (ASTNode* child : node->children)
	{
		if (child->name == CONDITION)
		{
			generateExpression(child->children);
		}
		else if (child->name == BLOCK)
		{
			generateBlock(child);
		}
	}
}

void codeGenerator::generateForStatement(ASTNode* node)
{
	_code += "for ";
	for (ASTNode* child : node->children)
	{
		if (child->name == INITIALIZATION)
		{
			generateDeclaration(child->children.front());
			_code += "; ";
		}
		else if (child->name == CONDITION)
		{
			generateExpression(child->children);
			_code += "; ";
		}
		else if (child->name == ITERATION)
		{
			_code += child->children.front()->value + child->children.back()->value;
		}
		else if (child->name == BLOCK)
		{
			generateBlock(child);
		}
	}
}

void codeGenerator::generateBlock(ASTNode* node)
{
	_code += " {\n";
	for (ASTNode* child : node->children)
	{
		_code += "\t" + std::string(_countTab, '\t');
		if (child->name == DECLARATION)
		{
			generateDeclaration(child);
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

void codeGenerator::generateExpression(ASTNode* node)
{
	_code += node->children.front()->value + " " +  node->value + " " + node->children.back()->value;
}

void codeGenerator::generateExpression(std::vector<ASTNode*> nodes)
{
	std::string last_value = nodes.front()->children.back()->value;
	for (ASTNode* node : nodes)
	{
		if (node->children.front()->value == last_value)
			_code += " " + node->value + " " + node->children.back()->value;
		else
			generateExpression(node);
		last_value = node->children.back()->value;
	}
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

void codeGenerator::generateArrayDeclaration(ASTNode* node)
{
	_code += "var ";
	for (ASTNode* child : node->children)
	{
		if (child->name == IDENTIFIER)
		{
			_code += child->value + " ";
		}
	}
	for (ASTNode* child : node->children)
	{
		if (child->name == ARRAY_LENGTH)
		{
			_code += '[';
			_code += child->children.back()->value;
			_code += ']';
		}
		if (child->name.find("DATATYPE") != std::string::npos)
		{
			_code += child->value + " ";
		}
	}
}

std::string codeGenerator::getCode()
{
	return _code;
}
