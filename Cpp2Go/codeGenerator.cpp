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
		if (child->name == BLOCK)
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
		else
		{
			generateExpression(child);
		}
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
			generateExpression(child->children.front());
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
			generateExpression(child->children.front());
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
			generateExpression(child->children.front());
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
			generateExpression(child->children.front());
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

std::string codeGenerator::getCode()
{
	return _code;
}
