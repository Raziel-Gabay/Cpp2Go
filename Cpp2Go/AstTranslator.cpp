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
			cppChild->name == WHILE_STATEMENT || cppChild->name == FOR_STATEMENT || cppChild->name == FOREACH_STATEMENT)
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
		else if (cppNode->name == STD_CIN_DECLARATION)
		{
			translateStdCin(cppChild, node);
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
				cppChild->name == WHILE_STATEMENT || cppChild->name == FOR_STATEMENT || cppChild->name == FOREACH_STATEMENT)
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
		else if (cppChild->name == STD_CIN_DECLARATION)
		{
			translateStdCin(cppChild, blockNode);
		}
		else
		{
			translateExpression(cppChild, blockNode);
		}
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
	ASTNode* fmtprintLnNode = new ASTNode("PRINT_DECLARATION");
	destNode->addChild(fmtprintLnNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == "PRINT")
		{
			ASTNode* printLnNode = new ASTNode(cppChild->name, cppChild->value);
			fmtprintLnNode->addChild(printLnNode);
		}
		else if (cppChild->name == STRING_LITERAL)
		{
			ASTNode* stringLiteralNode = new ASTNode(cppChild->name, cppChild->value);
			fmtprintLnNode->addChild(stringLiteralNode);
		}
	}
}

void AstTranslator::translateStdCin(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* fmtprintLnNode = new ASTNode("SCAN_DECLARATION");
	destNode->addChild(fmtprintLnNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == "SCAN")
		{
			ASTNode* printLnNode = new ASTNode(cppChild->name, cppChild->value);
			fmtprintLnNode->addChild(printLnNode);
		}
		else if (cppChild->name == IDENTIFIER)
		{
			ASTNode* stringLiteralNode = new ASTNode(cppChild->name, cppChild->value);
			fmtprintLnNode->addChild(stringLiteralNode);
		}
	}
}

void AstTranslator::translateFunctionCall(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* functionCallNode = new ASTNode(sourceNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == IDENTIFIER)
		{
			ASTNode* identifierNode = new ASTNode(cppChild);
			functionCallNode->addChild(identifierNode);
		}
		else if (cppChild->name == PARAMETER)
		{
			ASTNode* parameterNode = new ASTNode(cppChild);
			ASTNode* parameterChildNode = new ASTNode(cppChild->children.front());
			parameterNode->addChild(parameterChildNode);
			functionCallNode->addChild(parameterNode);
		}
	}
}


ASTNode* AstTranslator::getAST()
{
	return _astRoot;
}
