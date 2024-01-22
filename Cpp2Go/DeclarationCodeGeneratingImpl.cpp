#include "codeGenerator.h"

void codeGenerator::generateVariableDeclaration(ASTNode* node)
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

void codeGenerator::generatePointerDeclaration(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == ASSIGNMENT_OPERATOR || child->name == SHORT_ASSIGNMENT_OPERATOR)
		{
			generateExpression(child);
		}
		else if (child->name == POINTER_OPERATOR)
		{
			_code += child->value;
		}
		else
		{
			_code += child->value + " ";
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