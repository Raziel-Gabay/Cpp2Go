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
	iterativeTranslate(node, programNode);
	return programNode;
}

void AstTranslator::iterativeTranslate(ASTNode* cppNode, ASTNode* node)
{
	if (cppNode->name == PROGRAM)
	{
	}
	for (ASTNode* cppChild : cppNode->children)
	{
		if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, node);
		}
		else if (cppChild->name.find(DATATYPE) != std::string::npos)
		{
			translateDeclaration(cppChild, node);
		}
		else if (cppChild->name == IF_STATEMENT || cppChild->name == WHILE_STATEMENT || cppChild->name == FOR_STATEMENT)
		{
			translateStatement(cppChild, node);
		}
		else
		{
			translateExpression(cppChild, node);
		}
	}
}

void AstTranslator::translateDeclaration(ASTNode* sourceNode, ASTNode* &destNode)
{
	ASTNode* declarationNode = new ASTNode("DECLARATION");
	destNode->addChild(declarationNode);
	if (sourceNode->children.back()->name == ASSIGNMENT_OPERATOR)
	{
		ASTNode* assignmentNode = sourceNode->children.back();
		if (_identifiers.find(assignmentNode->children.front()->value) != _identifiers.end())
		{
			declarationNode->addChild(assignmentNode);
		}
		else if (destNode->name == BLOCK || destNode->name == INITIALIZATION)
		{
			ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
			declarationNode->addChild(shortAssignmentNode);
			shortAssignmentNode->addChild(assignmentNode->children.front());
			shortAssignmentNode->addChild(assignmentNode->children.back());
			_identifiers.insert(assignmentNode->children.front()->value);

		}
		else
		{
			ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
			declarationNode->addChild(varNode);
			declarationNode->addChild(assignmentNode);
			_identifiers.insert(assignmentNode->children.front()->value);
		}
		
	}
	else
	{
		ASTNode* varNode = new ASTNode("VAR_KEYWORD", "var");
		declarationNode->addChild(varNode);
		declarationNode->addChild(sourceNode->children.back());
		declarationNode->addChild(sourceNode->children.front());
		_identifiers.insert(sourceNode->children.back()->value);
	}
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

void AstTranslator::translateIfStatement(ASTNode* sourceNode, ASTNode* &destNode)
{
	ASTNode* ifStatementNode = new ASTNode(IF_STATEMENT);
	destNode->addChild(ifStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == CONDITION)
		{
			translateExpression(cppChild, ifStatementNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, ifStatementNode);
		}
	}
}

void AstTranslator::translateWhileStatement(ASTNode* sourceNode, ASTNode* &destNode)
{
	ASTNode* whileStatementNode = new ASTNode(WHILE_STATEMENT);
	destNode->addChild(whileStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		ASTNode* child = new ASTNode(cppChild->name);
		whileStatementNode->addChild(child);
		if (cppChild->name == CONDITION)
		{
			translateExpression(cppChild, whileStatementNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, whileStatementNode);
		}
	}
}

void AstTranslator::translateForStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* forStatementNode = new ASTNode(FOR_STATEMENT);
	destNode->addChild(forStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{ 
		if (cppChild->name == INITIALIZATION)
		{
			ASTNode* child = new ASTNode(cppChild->name);
			forStatementNode->addChild(child);
			translateDeclaration(cppChild->children.front(), child);
		}
		else if (cppChild->name == CONDITION || cppChild->name == ITERATION)
		{
			translateExpression(cppChild, forStatementNode);
		}
		else if (cppChild->name == BLOCK)
		{
		translateBlock(cppChild, forStatementNode);
		}
	}
}

void AstTranslator::translateBlock(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* blockNode = new ASTNode(BLOCK);
	destNode->addChild(blockNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == DECLARATION)
		{
			translateDeclaration(cppChild, blockNode);
		}
		else if (cppChild->name == IF_STATEMENT || cppChild->name == WHILE_STATEMENT || cppChild->name == FOR_STATEMENT)
		{
			translateStatement(cppChild, blockNode);
		}
		else
		{
			translateExpression(cppChild, blockNode);
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

ASTNode* AstTranslator::getAST()
{
	return _astRoot;
}
