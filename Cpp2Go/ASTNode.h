#pragma once

#include <vector>
#include <string>




class ASTNode
{
public:
	// Variables
	std::string name;
	std::string value;
	std::vector<ASTNode*> children;

	// Constructor
	ASTNode(const std::string& nodeName, const std::string& nodeValue = "");
	ASTNode(ASTNode* otherAst);
	// Destructor
	~ASTNode();

	// Methods
	void addChild(ASTNode* childNode);
	void deleteAst(ASTNode* node);
};

