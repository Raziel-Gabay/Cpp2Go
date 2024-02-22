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
	else if (sourceNode->name == FOR_STATEMENT)
		translateForStatement(sourceNode, destNode);
	else if (sourceNode->name == FOREACH_STATEMENT)
		translateForeachStatement(sourceNode, destNode);
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

void AstTranslator::translateForeachStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* foreachStatementNode = new ASTNode(sourceNode);
	destNode->addChild(foreachStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == LOOP_VARIABLE)
		{
			ASTNode* indexNode = new ASTNode("INDEX", "_");
			ASTNode* commaNode = new ASTNode(COMMA, ",");

			//adding the loop variable
			ASTNode* loopVariableNode = new ASTNode(cppChild);
			loopVariableNode->addChild(new ASTNode(cppChild->children.front()->children.back()));

			foreachStatementNode->addChild(indexNode);
			foreachStatementNode->addChild(commaNode);
			foreachStatementNode->addChild(loopVariableNode);
		}
		else if (cppChild->name == COLON_OPERATOR)
		{
			ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
			ASTNode* rangeKeywordNode = new ASTNode("RANGE_KEYWORD", "range");
			foreachStatementNode->addChild(shortAssignmentNode);
			foreachStatementNode->addChild(rangeKeywordNode);
		}
		else if (cppChild->name == CONTAINER)
		{
			//adding the container
			ASTNode* containerNode = new ASTNode(cppChild);
			containerNode->addChild(new ASTNode(cppChild->children.front()));
			foreachStatementNode->addChild(containerNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, foreachStatementNode);
		}

	}
}
