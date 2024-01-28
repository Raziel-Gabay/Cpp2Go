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

void AstTranslator::translateVariableDeclaration(ASTNode* sourceNode, ASTNode* &destNode)
{
	ASTNode* variableDeclarationNode = new ASTNode(sourceNode);
	destNode->addChild(variableDeclarationNode);
	if (sourceNode->children.back()->name == ASSIGNMENT_OPERATOR)
	{
		ASTNode* assignmentNode = sourceNode->children.back();
		if (_identifiers.find(assignmentNode->children.front()->value) != _identifiers.end())
		{
			translateExpression(assignmentNode, variableDeclarationNode);
		}
		else if (destNode->name == BLOCK || destNode->name == INITIALIZATION)
		{
			ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
			variableDeclarationNode->addChild(shortAssignmentNode);
			shortAssignmentNode->addChild(new ASTNode(assignmentNode->children.front()));
			shortAssignmentNode->addChild(new ASTNode(assignmentNode->children.back()));
			_identifiers.insert(assignmentNode->children.front()->value);

		}
		else
		{
			ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
			variableDeclarationNode->addChild(varNode);
			translateExpression(assignmentNode, variableDeclarationNode);
			_identifiers.insert(assignmentNode->children.front()->value);
		}
		
	}
	else
	{
		ASTNode* varNode = new ASTNode("VAR_KEYWORD", "var");
		variableDeclarationNode->addChild(varNode);
		variableDeclarationNode->addChild(new ASTNode(sourceNode->children.back()));
		translateType(sourceNode->children.front(), variableDeclarationNode);
		_identifiers.insert(sourceNode->children.back()->value);
	}
}

void AstTranslator::translateFunctionDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* functionDeclarationNode = new ASTNode(sourceNode);
	ASTNode* funcNode = new ASTNode("FUNC_KEYWORD", "func");
	ASTNode* returnValueNode = sourceNode->children.front();
	ASTNode* goReturnValueNode = new ASTNode(returnValueNode);
	translateType(returnValueNode->children.front(), goReturnValueNode);
	destNode->addChild(functionDeclarationNode);
	functionDeclarationNode->addChild(funcNode);


	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == IDENTIFIER )
		{
			ASTNode* goNode = new ASTNode(cppChild);
			functionDeclarationNode->addChild(goNode);
		}
		else if (cppChild->name == PARAMETER)
		{
			ASTNode* parameterNode = new ASTNode(cppChild);
			ASTNode* parameterChildNode = new ASTNode(cppChild->children.front());
			parameterNode->addChild(parameterChildNode);
			functionDeclarationNode->addChild(parameterNode);
		}
		else if (cppChild->name == BLOCK)
		{
			if (returnValueNode->children.front()->value != "void" && sourceNode->children.begin()[1]->value != "main")
				functionDeclarationNode->addChild(goReturnValueNode);
			translateBlock(cppChild, functionDeclarationNode);
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

void AstTranslator::translateArrayDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* arrayDeclarationNode = new ASTNode(sourceNode);
	destNode->addChild(arrayDeclarationNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == ARRAY_LENGTH)
		{
			ASTNode* arrayLengthNode = new ASTNode(cppChild);
			
			ASTNode* intLiteralNode = new ASTNode(cppChild->children.front());
			arrayLengthNode->addChild(intLiteralNode);
			arrayDeclarationNode->addChild(arrayLengthNode);
		}
		else if (cppChild->name == IDENTIFIER)
		{
			ASTNode* ArrayNameNode = new ASTNode(cppChild);
			arrayDeclarationNode->addChild(ArrayNameNode);
			_identifiers.insert(cppChild->value);
		}
	}
}

void AstTranslator::translatePointerDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* pointerDeclarationNode = new ASTNode(sourceNode);
	destNode->addChild(pointerDeclarationNode);
	if (sourceNode->children.back()->name == ASSIGNMENT_OPERATOR)
	{
		ASTNode* assignmentNode = sourceNode->children.back();
		if (_identifiers.find(assignmentNode->children.front()->value) != _identifiers.end())
		{
			translateExpression(assignmentNode, pointerDeclarationNode);
		}
		else if (destNode->name == BLOCK || destNode->name == INITIALIZATION)
		{
			ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
			pointerDeclarationNode->addChild(shortAssignmentNode);
			shortAssignmentNode->addChild(new ASTNode(assignmentNode->children.front()));
			shortAssignmentNode->addChild(new ASTNode(assignmentNode->children.back()));
			_identifiers.insert(assignmentNode->children.front()->value);
		}
		else
		{
			ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
			pointerDeclarationNode->addChild(varNode);
			translateExpression(assignmentNode, pointerDeclarationNode);
			_identifiers.insert(assignmentNode->children.front()->value);
		}

	}
	else
	{
		ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
		//ASTNode* addressNode = sourceNode->children.back();
		pointerDeclarationNode->addChild(varNode); // added var keyword
		pointerDeclarationNode->addChild(new ASTNode(sourceNode->children.back())); // added identifier
		pointerDeclarationNode->addChild(new ASTNode(sourceNode->children[1])); // added pointer operator
		translateType(sourceNode->children.front(), pointerDeclarationNode); // add datatype
		_identifiers.insert(sourceNode->children.back()->value);
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
