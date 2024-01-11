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
			node->addChild(cppChild);
		}
		else if (cppNode->name == ARRAY_DECLARATION)
		{
			translateArrayDeclaration(cppChild, node);
		}
		else
		{
			translateExpression(cppChild, node);
		}
	}
}

void AstTranslator::translateVariableDeclaration(ASTNode* sourceNode, ASTNode* &destNode)
{
	ASTNode* variableDeclarationNode = new ASTNode("VARIABLE_DECLARATION");
	destNode->addChild(variableDeclarationNode);
	if (sourceNode->children.back()->name == ASSIGNMENT_OPERATOR)
	{
		ASTNode* assignmentNode = sourceNode->children.back();
		if (_identifiers.find(assignmentNode->children.front()->value) != _identifiers.end())
		{
			variableDeclarationNode->addChild(assignmentNode);
		}
		else if (destNode->name == BLOCK || destNode->name == INITIALIZATION)
		{
			ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
			variableDeclarationNode->addChild(shortAssignmentNode);
			shortAssignmentNode->addChild(assignmentNode->children.front());
			shortAssignmentNode->addChild(assignmentNode->children.back());
			_identifiers.insert(assignmentNode->children.front()->value);

		}
		else
		{
			ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
			variableDeclarationNode->addChild(varNode);
			variableDeclarationNode->addChild(assignmentNode);
			_identifiers.insert(assignmentNode->children.front()->value);
		}
		
	}
	else
	{
		ASTNode* varNode = new ASTNode("VAR_KEYWORD", "var");
		variableDeclarationNode->addChild(varNode);
		variableDeclarationNode->addChild(sourceNode->children.back());
		translateType(sourceNode->children.front(), variableDeclarationNode);
		_identifiers.insert(sourceNode->children.back()->value);
	}
}

void AstTranslator::translateFunctionDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* functionDeclarationNode = new ASTNode("FUNCTION_DECLARATION");
	ASTNode* funcNode = new ASTNode("FUNC_KEYWORD", "func");
	ASTNode* returnValueNode = sourceNode->children.front();
	destNode->addChild(functionDeclarationNode);
	functionDeclarationNode->addChild(funcNode);


	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == IDENTIFIER || cppChild->name == PARAMETER)
			functionDeclarationNode->addChild(cppChild);
		else if (cppChild->name == BLOCK)
		{
			if (returnValueNode->children.front()->value != "void" && sourceNode->children.begin()[1]->value != "main")
				functionDeclarationNode->addChild(returnValueNode);
			translateBlock(cppChild, functionDeclarationNode);
		}
	}
}


void AstTranslator::translateStruct(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* structNode = new ASTNode("STRUCT");
	ASTNode* typeNode = new ASTNode("TYPE_KEYWORD", "type");
	destNode->addChild(structNode);
	structNode->addChild(typeNode);

	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == IDENTIFIER)
		{
			structNode->addChild(cppChild);
			structNode->addChild(sourceNode->children.front());
		}
		else if (cppChild->name == MEMBERS)
		{
			ASTNode* membersNode = new ASTNode("MEMBERS");
			structNode->addChild(membersNode);
			for (ASTNode* member : cppChild->children)
			{
				if (member->name == VARIABLE_DECLARATION)
				{
					ASTNode* goMember = member;
					if (goMember->children.front()->name == DATATYPE_STRING)
					{
						goMember->children.front()->value = "string";
					}
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

void AstTranslator::translateElseIfStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* elseIfStatementNode = new ASTNode(ELSE_IF_STATEMENT);
	destNode->addChild(elseIfStatementNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == CONDITION)
		{
			translateExpression(cppChild, elseIfStatementNode);
		}
		else if (cppChild->name == BLOCK)
		{
			translateBlock(cppChild, elseIfStatementNode);
		}
	}
}

void AstTranslator::translateElseStatement(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* elseIfStatementNode = new ASTNode(ELSE_STATEMENT);
	destNode->addChild(elseIfStatementNode);
	translateBlock(sourceNode->children.front(), elseIfStatementNode);
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
			translateVariableDeclaration(cppChild->children.front(), child);
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
			blockNode->addChild(cppChild);
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
	destNode->addChild(sourceNode);
	if (sourceNode->name == DATATYPE_STRING)
	{
		destNode->children.back()->value = "string";
	}
}

void AstTranslator::translateArrayDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* arrayDeclarationNode = new ASTNode(ARRAY_DECLARATION);
	destNode->addChild(arrayDeclarationNode);
	for (ASTNode* cppChild : sourceNode->children)
	{
		if (cppChild->name == ARRAY_LENGTH)
		{
			ASTNode* arrayLengthNode = new ASTNode(ARRAY_LENGTH);
			arrayDeclarationNode->addChild(arrayLengthNode);
		}
		else if (cppChild->name == INT_LITERAL)
		{
			ASTNode* intLiteralNode = new ASTNode(INT_LITERAL);
			arrayDeclarationNode->addChild(intLiteralNode);
		}
		else if (cppChild->name == IDENTIFIER)
		{
			ASTNode* ArrayNameNode = new ASTNode(IDENTIFIER);
			arrayDeclarationNode->addChild(ArrayNameNode);
		}
	}
}

void AstTranslator::translatePointerDeclaration(ASTNode* sourceNode, ASTNode*& destNode)
{
	ASTNode* pointerDeclarationNode = new ASTNode("POINTER_DECLARATION");
	destNode->addChild(pointerDeclarationNode);
	if (sourceNode->children.back()->name == ASSIGNMENT_OPERATOR)
	{
		ASTNode* assignmentNode = sourceNode->children.back();
		if (_identifiers.find(assignmentNode->children.front()->value) != _identifiers.end())
		{
			pointerDeclarationNode->addChild(assignmentNode);
		}
		else if (destNode->name == BLOCK || destNode->name == INITIALIZATION)
		{
			ASTNode* shortAssignmentNode = new ASTNode(SHORT_ASSIGNMENT_OPERATOR, ":=");
			pointerDeclarationNode->addChild(shortAssignmentNode);
			shortAssignmentNode->addChild(assignmentNode->children.front());
			shortAssignmentNode->addChild(assignmentNode->children.back());
			_identifiers.insert(assignmentNode->children.front()->value);

		}
		else
		{
			ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
			pointerDeclarationNode->addChild(varNode);
			pointerDeclarationNode->addChild(assignmentNode);
			_identifiers.insert(assignmentNode->children.front()->value);
		}

	}
	else
	{
		ASTNode* varNode = new ASTNode(VAR_KEYWORD, "var");
		//ASTNode* addressNode = sourceNode->children.back();
		pointerDeclarationNode->addChild(varNode); // added var keyword
		pointerDeclarationNode->addChild(sourceNode->children.back()); // added identifier
		pointerDeclarationNode->addChild(sourceNode->children[1]); // added pointer operator
		translateType(sourceNode->children.front(), pointerDeclarationNode); // add datatype
		_identifiers.insert(sourceNode->children.back()->value);
	}
}

ASTNode* AstTranslator::getAST()
{
	return _astRoot;
}
