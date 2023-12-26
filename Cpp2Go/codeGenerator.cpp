#include "codeGenerator.h"

codeGenerator::codeGenerator(ASTNode* destNode)
	:_destNode(destNode)
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
		else if (child->name.find(DATATYPE) != std::string::npos)
		{
			generateDeclaration(child);
		}
		else if (child->name == IF_STATEMENT || child->name == WHILE_STATEMENT || child->name == FOR_STATEMENT)
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
}

void codeGenerator::generateDeclaration(ASTNode* node)
{
	for (ASTNode* child : node->children)
	{
		if (child->name == ASSIGNMENT_OPERATOR)
		{
			generateExpression(child);
		}
		else
		{
			_code += child->value;
		}
	}

}

void codeGenerator::generateStatement(ASTNode* node)
{
}

void codeGenerator::generateIfStatement(ASTNode* node)
{
}

void codeGenerator::generateWhileStatement(ASTNode* node)
{
}

void codeGenerator::generateForStatement(ASTNode* node)
{
	_code = "for ";
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
			generateExpression(child->children.front());
		}
		else if (child->name == BLOCK)
		{
			_code += "\n";
			generateBlock(child);
		}
	}
}

void codeGenerator::generateBlock(ASTNode* node)
{
	_code += "{\n";
	for (ASTNode* child : node->children)
	{
		_code += "\t";
		if (child->name.find(DATATYPE) != std::string::npos)
		{
			generateDeclaration(child);
		}
		else if (child->name == IF_STATEMENT || child->name == WHILE_STATEMENT || child->name == FOR_STATEMENT)
		{
			generateStatement(child);
		}
		else
		{
			generateExpression(child);
		}
		_code += "\n";
	}
	_code += "}\n";
}

void codeGenerator::generateExpression(ASTNode* node)
{
	_code += node->children.front()->value + " " +  node->value + " " + node->children.back()->value;
}
