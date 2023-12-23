#include "AstTranslator.h"

AstTranslator::AstTranslator(ASTNode* cppRoot)
    :_cppRoot(cppRoot)
{
    this->_astRoot = translateProgram(_cppRoot);
}

AstTranslator::~AstTranslator()
{
}

ASTNode* AstTranslator::translateProgram(ASTNode* node)
{
	ASTNode* programNode = new ASTNode(PROGRAM);
	recursiveTranslate(node, programNode);
	return programNode;
}

void AstTranslator::recursiveTranslate(ASTNode* cppNode, ASTNode* node)
{
	if (cppNode)
	{
		if (cppNode->name == PROGRAM)
		{
		}
		else if (cppNode->name == BLOCK)
		{
			node = new ASTNode(BLOCK);
		}
		if (cppNode->name.find(DATATYPE) != std::string::npos)
		{
			translateDeclaration(cppNode, node);
		}
		else if (cppNode->name == IF_STATEMENT || cppNode->name == WHILE_STATEMENT || cppNode->name == FOR_STATEMENT)
		{
			translateStatement(cppNode, node);
		}
		else
		{
			translateExpression(cppNode, node);
		}
		// Recursively translate children
		for (ASTNode* cppChild : cppNode->children) {
			
			ASTNode* child = 0;
			node->addChild(child);
			recursiveTranslate(cppChild, child);
		}
	}
}

void AstTranslator::translateDeclaration(ASTNode* sourceNode, ASTNode* &destNode)
{
}

void AstTranslator::translateStatement(ASTNode* sourceNode, ASTNode* &destNode)
{
	if (sourceNode->name == IF_STATEMENT)
		translateIfStatement(sourceNode, destNode);
	else if (sourceNode->name == WHILE_STATEMENT)
		translateWhileStatement(sourceNode, destNode);
	else
		translateForStatement(sourceNode, destNode);
}

ASTNode* AstTranslator::translateIfStatement(ASTNode* sourceNode, ASTNode* &destNode)
{
	if (sourceNode)
	{
		if (sourceNode->name == IF_STATEMENT)
		{
		}
		else if (sourceNode->name == CONDITION)
		{
			translateExpression(destNode, sourceNode->children.front());
		}
		else if (sourceNode->name == BLOCK)
		{
			translateBlock(destNode, sourceNode->children.front());
		}

		for (ASTNode* cppChild : sourceNode->children) 
		{

			ASTNode* child = 0;
			destNode->addChild(child);
			recursiveTranslate(cppChild, child);
		}
	}
}

ASTNode* AstTranslator::translateWhileStatement(ASTNode* sourceNode, ASTNode* &destNode)
{
	if (sourceNode)
	{
		if (sourceNode->name == WHILE_STATEMENT)
		{
		}
		else if (sourceNode->name == CONDITION)
		{
			translateExpression(destNode, sourceNode->children.front());
		}
		else if (sourceNode->name == BLOCK)
		{
			translateBlock(destNode, sourceNode->children.front());
		}

		for (ASTNode* cppChild : sourceNode->children)
		{

			ASTNode* child = 0;
			destNode->addChild(child);
			recursiveTranslate(cppChild, child);
		}
	}
}

ASTNode* AstTranslator::translateForStatement(ASTNode* sourceNode, ASTNode* &destNode)
{
	if (sourceNode)
	{
		if (sourceNode->name == FOR_STATEMENT)
		{
		}
		else if (sourceNode->name == INITIALIZATION)
		{
			translateDeclaration(destNode ,sourceNode->children.front());
		}
		else if (sourceNode->name == CONDITION || sourceNode->name == ITERATION)
		{
			translateExpression(destNode, sourceNode->children.front());
		}
		else if (sourceNode->name == BLOCK)
		{
			translateBlock(destNode, sourceNode->children.front());
		}

		for (ASTNode* cppChild : sourceNode->children) 
		{
			ASTNode* child = 0;
			destNode->addChild(child);
			recursiveTranslate(cppChild, child);
		}
	}
}

ASTNode* AstTranslator::translateBlock(ASTNode* sourceNode, ASTNode*& destNode)
{
	if (sourceNode)
	{
		if (sourceNode->name == BLOCK)
		{
		}
		else 
		{
			translateExpression(destNode, sourceNode->children.front());
		}

		for (ASTNode* cppChild : sourceNode->children) {

			ASTNode* child = 0;
			destNode->addChild(child);
			recursiveTranslate(cppChild, child);
		}
	}
}

void AstTranslator::translateExpression(ASTNode* sourceNode, ASTNode*& destNode)
{
	if (sourceNode->value == "~")
	{
		ASTNode* opNode = sourceNode;
		opNode->value = "^";
		destNode->addChild(opNode);
	}
	else
	{
		destNode->addChild(sourceNode);
	}
}
