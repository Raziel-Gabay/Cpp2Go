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
	ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
	ASTNode* ifStatementNode = new ASTNode("IF_STATEMENT");
	ASTNode* blockNode = new ASTNode("BLOCK");
	//ASTNode* expressionTrueNode = new ASTNode(sourceNode->
	destNode->addChild(shortAssignmentNode);
	destNode->addChild(ifStatementNode);
	shortAssignmentNode->addChild(new ASTNode(sourceNode->children.front())); // adding identifier
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
			ASTNode* assignmentNode = new ASTNode(ASSIGNMENT_OPERATOR, "=");
			assignmentNode->addChild(new ASTNode(sourceNode->children.front()));
			assignmentNode->addChild(new ASTNode(cppChild->children.front()));
			blockNode->addChild(assignmentNode);
		}
		else if (cppChild->name == EXPRESSION_FALSE)
		{
			shortAssignmentNode->addChild(new ASTNode(cppChild->children.front()));
		}
	}
}
