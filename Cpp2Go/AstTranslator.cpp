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
	ASTNode* programNode = new ASTNode("PROGRAM");
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
			node = new ASTNode("BLOCK");
		}
		if (cppNode->name.find("DATATYPE") != std::string::npos)
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

void AstTranslator::translateDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
}

void AstTranslator::translateStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
}

ASTNode* AstTranslator::translateIfStatement(std::string ifConditionPart, std::string ifBlockPart)
{
    return nullptr;
}

ASTNode* AstTranslator::translateWhileStatement(std::string whileConditionPart, std::string whileBlockPart)
{
    return nullptr;
}

ASTNode* AstTranslator::translateForStatement(std::string forInitializationPart, std::string forConditionPart, std::string forPostPart, std::string forBlockPart)
{
    return nullptr;
}

void AstTranslator::translateExpression(ASTNode* sourceNode, ASTNode*& destNode)
{
}
