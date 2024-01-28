#include "AstTranslator.h"


void AstTranslator::translateVariableDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
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
		if (cppChild->name == IDENTIFIER)
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