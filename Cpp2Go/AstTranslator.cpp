#include "AstTranslator.h"

AstTranslator::AstTranslator(ASTNode* cppRoot)
    :_cppRoot(cppRoot)
{
    this->_astRoot = translateProgram(_cppRoot);
}

AstTranslator::~AstTranslator()
{
	delete _cppRoot;
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
		if (cppChild->name == FUNCTION_DECLARATION)
		{
			translateFunctionDeclaration(cppChild, node);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, node);
		}
		else if (cppChild->name == VARIABLE_DECLARATION)
		{
			translateVariableDeclaration(cppChild, node);
		}
		else if (cppChild->name == POINTER_DECLARATION)
		{
			translatePointerDeclaration(cppChild, node);
		}
		else if (cppChild->name == IF_STATEMENT || cppChild->name == ELSE_IF_STATEMENT || cppChild->name == ELSE_STATEMENT ||
			cppChild->name == WHILE_STATEMENT || cppChild->name == FOR_STATEMENT)
		{
			translateStatement(cppChild, node);
		}
		else if (cppChild->name == STRUCT)
		{
			translateStruct(cppChild, node);
		}
		else if (cppChild->name == INCLUDE_DIRECTIVE)
		{
			translateIncludeDirective(cppChild, node);
		}
		else if (cppChild->name == FUNCTION_CALL)
		{
			translateFunctionCall(cppChild, node);
		}
		else if (cppNode->name == ARRAY_DECLARATION)
		{
			translateArrayDeclaration(cppChild, node);
		}
		else if (cppNode->name == STD_COUT_DECLARATION)
		{
			translateStdCout(cppChild, node);
		}
		else
		{
			translateExpression(cppChild, node);
		}
	}
}



void AstTranslator::translateStruct(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* structNode = new ASTNode(sourceNode);
	ASTNode* typeNode = new ASTNode("TYPE_KEYWORD", "type");
	destNode->addChild(structNode);
	structNode->addChild(typeNode);

	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == IDENTIFIER)
		{
			structNode->addChild(new ASTNode(cppChild));
			structNode->addChild(new ASTNode(sourceNode->children.front()));
		}
		else if (cppChild->name == MEMBERS)
		{
			ASTNode* membersNode = new ASTNode("MEMBERS");
			structNode->addChild(membersNode);
			for (ASTNode* member : cppChild->children)
			{
				if (member->name == VARIABLE_DECLARATION)
				{
					ASTNode* goMember = new ASTNode(member);
					translateType(member->children.front(), goMember);
					std::reverse(goMember->children.begin(), goMember->children.end());
					membersNode->addChild(goMember);
				}
			}
		}
	}
}

void AstTranslator::translateStatement(ASTNode* sourceNode, ASTNode* &destNode)
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

void AstTranslator::translateIfStatement(ASTNode* sourceNode, ASTNode* &destNode)
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

void AstTranslator::translateWhileStatement(ASTNode* sourceNode, ASTNode* &destNode)
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

void AstTranslator::translateBlock(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* blockNode = new ASTNode(sourceNode);
	destNode->addChild(blockNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == VARIABLE_DECLARATION)
		{
			translateVariableDeclaration(cppChild, blockNode);
		}
		else if (cppChild->name == POINTER_DECLARATION)
		{
			translatePointerDeclaration(cppChild, blockNode);
		}
		else if (cppChild->name == ELSE_IF_STATEMENT || cppChild->name == ELSE_STATEMENT || cppChild->name == IF_STATEMENT ||
				cppChild->name == WHILE_STATEMENT || cppChild->name == FOR_STATEMENT)
		{
			translateStatement(cppChild, blockNode);
		}
		else if (cppChild->name == FUNCTION_CALL)
		{
			translateFunctionCall(cppChild, blockNode);
		}
		else if (cppChild->name == ARRAY_DECLARATION)
		{
			translateArrayDeclaration(cppChild, blockNode);
		}
		else if (cppChild->name == STD_COUT_DECLARATION)
		{
			translateStdCout(cppChild, blockNode);
		}
		else
		{
			translateExpression(cppChild, blockNode);
		}
	}
}

void AstTranslator::translateExpression(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* opNode = new ASTNode(sourceNode);
	ASTNode* frontNode = new ASTNode(sourceNode->children.front());
	ASTNode* backNode = new ASTNode(sourceNode->children.back());
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

void AstTranslator::translateIncludeDirective(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* includeNode = new ASTNode(IMPORT_DIRECTIVE);
	destNode->addChild(includeNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == INCLUDE_KEYWORD)
		{
			ASTNode* importNode = new ASTNode(IMPORT);
			includeNode->addChild(importNode);
		}
		else if (cppChild->name == IDENTIFIER)
		{
			ASTNode* libaryName = new ASTNode(IDENTIFIER, "fmt");
			includeNode->addChild(libaryName);
		}
	}
}

void AstTranslator::translateType(ASTNode* sourceNode, ASTNode*& destNode)
{
	destNode->addChild(new ASTNode(sourceNode));
	if (sourceNode->name == DATATYPE_STRING)
	{
		destNode->children.back()->value = "string";
	}
}

void AstTranslator::translateStdCout(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* fmtprintLnNode = new ASTNode(FMT_PRINTLN);
	destNode->addChild(fmtprintLnNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == STD_COUT)
		{
			ASTNode* printLnNode = new ASTNode(PRINTLN);
			fmtprintLnNode->addChild(printLnNode);
		}
		else if (cppChild->name == STRING_LITERAL)
		{
			ASTNode* stringLiteralNode = new ASTNode(STRING_LITERAL, cppChild->value);
			fmtprintLnNode->addChild(stringLiteralNode);
		}
	}
}

ASTNode* AstTranslator::getAST()
{
	return _astRoot;
}
