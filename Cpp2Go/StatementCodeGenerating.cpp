#include "codeGenerator.h"

void codeGenerator::generateStatement(ASTNode* node)
{
	if (node->name == IF_STATEMENT)
		generateIfStatement(node);
	else if (node->name == ELSE_IF_STATEMENT)
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
			generateVariableDeclaration(child->children.front());
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