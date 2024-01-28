#include "AstTranslator.h"

void AstTranslator::translateStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	if (sourceNode->name == IF_STATEMENT)
		translateIfStatement(sourceNode, destNode);
	else if (sourceNode->name == ELSE_IF_STATEMENT)
		translateElseIfStatement(sourceNode, destNode);
	else if (sourceNode->name == ELSE_STATEMENT)
		translateElseStatement(sourceNode, destNode);

	else if (sourceNode->name == WHILE_STATEMENT)
		translateWhileStatement(sourceNode, destNode);
	else
		translateForStatement(sourceNode, destNode);
}

void AstTranslator::translateIfStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* ifStatementNode = new ASTNode(sourceNode);
	destNode->addChild(ifStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == CONDITION)
		{
			ASTNode* conditionNode = new ASTNode(cppChild);
			translateExpression(cppChild->children.front(), conditionNode);
			ifStatementNode->addChild(conditionNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, ifStatementNode);
		}
	}
}

void AstTranslator::translateElseIfStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* elseIfStatementNode = new ASTNode(sourceNode);
	destNode->addChild(elseIfStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == CONDITION)
		{
			ASTNode* conditionNode = new ASTNode(cppChild);
			translateExpression(cppChild->children.front(), conditionNode);
			elseIfStatementNode->addChild(conditionNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, elseIfStatementNode);
		}
	}
}

void AstTranslator::translateElseStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* elseIfStatementNode = new ASTNode(sourceNode);
	destNode->addChild(elseIfStatementNode);
	translateBlock(sourceNode->children.front(), elseIfStatementNode);
}

void AstTranslator::translateWhileStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* whileStatementNode = new ASTNode(sourceNode);
	destNode->addChild(whileStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		ASTNode* child = new ASTNode(cppChild);
		whileStatementNode->addChild(child);
		if (cppChild->name == CONDITION)
		{
			ASTNode* conditionNode = new ASTNode(cppChild);
			translateExpression(cppChild->children.front(), conditionNode);
			whileStatementNode->addChild(conditionNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, whileStatementNode);
		}
	}
}

void AstTranslator::translateForStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* forStatementNode = new ASTNode(sourceNode);
	destNode->addChild(forStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == INITIALIZATION)
		{
			ASTNode* child = new ASTNode(cppChild);
			forStatementNode->addChild(child);
			translateVariableDeclaration(cppChild->children.front(), child);
		}
		else if (cppChild->name == CONDITION)
		{
			ASTNode* conditionNode = new ASTNode(cppChild);
			translateExpression(cppChild->children.front(), conditionNode);
			forStatementNode->addChild(conditionNode);
		}
		else if (cppChild->name == ITERATION)
		{
			ASTNode* iterationNode = new ASTNode(cppChild);
			ASTNode* frontNode = new ASTNode(cppChild->children.front());
			ASTNode* backNode = new ASTNode(cppChild->children.back());
			iterationNode->addChild(frontNode);
			iterationNode->addChild(backNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, forStatementNode);
		}
	}
}
