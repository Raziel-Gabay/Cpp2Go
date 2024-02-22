#include "AstTranslator.h"


void AstTranslator::translateExpression(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* opNode = new ASTNode(sourceNode);
	ASTNode* frontNode = new ASTNode(sourceNode->children.front());
	ASTNode* backNode = new ASTNode(sourceNode->children.back());
	if (backNode->name == TERNARY_OPERATOR)
	{
		translateTernaryOperator(sourceNode, destNode);
		return;
	}
	opNode->addChild(frontNode);
	opNode->addChild(backNode);

	if (sourceNode->value == "~")
	{
		opNode->value = "^";
		destNode->addChild(opNode);
	}
	else
	{
		destNode->addChild(opNode);
	}
}

void AstTranslator::translateTernaryOperator(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* assignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
	ASTNode* ifStatementNode = new ASTNode("IF_STATEMENT");
	ASTNode* blockNode = new ASTNode("BLOCK");
	if (_identifiers.count(sourceNode->children.front()->value) > 0)
	{
		assignmentNode = new ASTNode(sourceNode);
	}
	destNode->addChild(assignmentNode);
	destNode->addChild(ifStatementNode);
	assignmentNode->addChild(new ASTNode(sourceNode->children.front())); // adding identifier
	for (ASTNode* cppChild : sourceNode->children.back()->children)
	{
		if (cppChild->name == CONDITION)
		{
			ASTNode* conditionNode = new ASTNode(cppChild);
			translateExpression(cppChild->children.front(), conditionNode);
			ifStatementNode->addChild(conditionNode);
			ifStatementNode->addChild(blockNode);
		}
		else if (cppChild->name == EXPRESSION_TRUE)
		{
			ASTNode* ifAssignmentNode = new ASTNode(ASSIGNMENT_OPERATOR, "=");
			ifAssignmentNode->addChild(new ASTNode(sourceNode->children.front()));
			ifAssignmentNode->addChild(new ASTNode(cppChild->children.front()));
			blockNode->addChild(ifAssignmentNode);
		}
		else if (cppChild->name == EXPRESSION_FALSE)
		{
			assignmentNode->addChild(new ASTNode(cppChild->children.front()));
		}
	}
}
