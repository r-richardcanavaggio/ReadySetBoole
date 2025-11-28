/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTHelper.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:59:51 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/28 12:16:05 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include <regex>

std::unique_ptr<ASTNode>	eliminateImplications( std::unique_ptr<ASTNode> node )
{
	if (!node)
		return nullptr;
	node->left = eliminateImplications(std::move(node->left));
	node->right = eliminateImplications(std::move(node->right));

	switch (node->type)
	{
		case NodeType::MATERIAL_CONDITION:
			return (std::make_unique<ASTNode>(NodeType::DISJUNCTION, "|",
				std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(node->left), nullptr), std::move(node->right)));
		case NodeType::LOGICAL_EQUIVALENCE:
		{
			// (A & B) | (!A & !B))
			auto A = std::move(node->left);
			auto B = std::move(node->right);
			auto cpA = A->clone();
			auto cpB = B->clone();
			auto newLeft = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(A), std::move(B));
			auto newRight = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "&",
					std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(cpA), nullptr),
					std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(cpB), nullptr));
			return (std::make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(newLeft), std::move(newRight)));
		}
		case NodeType::EXCLUSIVE_DISJUNCTION:
		{
			// A ^ B  === (A & !B) | (!A & B)
			auto A = std::move(node->left);
			auto B = std::move(node->right);
			auto cpA = A->clone();
			auto cpB = B->clone();
			auto newLeft = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(A),
				std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(B), nullptr));
			auto newRight = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "&",
				std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(cpA), nullptr), std::move(cpB));
			return (std::make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(newLeft), std::move(newRight)));
		}
		default:
			return (node);
	}
	return (node);
}

std::unique_ptr<ASTNode>	transformASTtoNNF( std::unique_ptr<ASTNode> node, bool neg )
{
	if (!node)
		return nullptr;
	
	if (node->type == NodeType::VARIABLE)
	{
		if (!neg)
			return (node);
		return (std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(node), nullptr));
	}
	if (node->type == NodeType::NEGATION)
		return (transformASTtoNNF(std::move(node->left), !neg));
	if (!neg)
	{
		node->left = transformASTtoNNF(std::move(node->left), false);
		node->right = transformASTtoNNF(std::move(node->right), false);
		return (node);
	}
	else
	{
		if (node->type == NodeType::CONJUNCTION)
		{
			auto L = transformASTtoNNF(std::move(node->left), true);
			auto R = transformASTtoNNF(std::move(node->right), true);
			return (std::make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(L), std::move(R)));
		}
		if (node->type == NodeType::DISJUNCTION)
		{
			auto L = transformASTtoNNF(std::move(node->left), true);
			auto R = transformASTtoNNF(std::move(node->right), true);
			return (std::make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(L), std::move(R)));
		}
	}
	return (node);
}

std::string	negation_normal_form( const std::string& formula )
{
	std::unique_ptr<ASTNode>	ast;
	std::string					infix;
	std::string					nnf;

	infix = toInfix(formula);
	ast = toAST(infix);
	ast = eliminateImplications(std::move(ast));
	ast = transformASTtoNNF(std::move(ast));
	computeToRpn(ast.get(), nnf);
	return (nnf);
}

void	computeToRpn( ASTNode* node, std::string& str )
{
	if (!node)
		return ;
	computeToRpn(node->left.get(), str);
	computeToRpn(node->right.get(), str);
	str += node->value;
}

std::unique_ptr<ASTNode>	toAST( const std::string& formula )
{	
	std::string					input;
	std::string::iterator		it;
	std::unique_ptr<ASTNode>	ast;

	input = formula;
	input = std::regex_replace(input, std::regex(" "), "");
	it = input.begin();
	ast = parseFormula(it, input.end());
	if (it != input.end())
		throw std::runtime_error(std::string("Unexpected token at end of input: ") + std::string(1, *it));
	return (ast);
}