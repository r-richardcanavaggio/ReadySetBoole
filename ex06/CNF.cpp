/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CNF.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 12:59:04 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/24 13:01:35 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

vector<unique_ptr<ASTNode>>	flattenAnd( unique_ptr<ASTNode> node )
{
	vector<unique_ptr<ASTNode>>	clauses;
	
	if (!node)
		return (clauses);

	if (node->type == NodeType::CONJUNCTION)
	{
		vector<unique_ptr<ASTNode>>	left_clauses  = flattenAnd(std::move(node->left));	
		vector<unique_ptr<ASTNode>>	right_clauses = flattenAnd(std::move(node->right));
		std::move(left_clauses.begin(), left_clauses.end(), back_inserter(clauses));
		std::move(right_clauses.begin(), right_clauses.end(), back_inserter(clauses));
	}
	else
		clauses.push_back(std::move(node));
	return (clauses);
}

vector<unique_ptr<ASTNode>>	flattenOr( unique_ptr<ASTNode> node )
{
	vector<unique_ptr<ASTNode>>	v;

	if (!node)
		return (v);
	if (node->type == NodeType::DISJUNCTION)
	{
		auto	L = flattenOr(std::move(node->left));
		auto	R = flattenOr(std::move(node->right));
		std::move(L.begin(), L.end(), std::back_inserter(v));
		std::move(R.begin(), R.end(), std::back_inserter(v));
	}
	else
		v.push_back(std::move(node));
	return (v);
}

unique_ptr<ASTNode>	buildOrRight( vector<unique_ptr<ASTNode>>& v )
{
	if (v.empty())
		return (nullptr);
	unique_ptr<ASTNode>	root = std::move(v.back());
	v.pop_back();
	while (!v.empty())
	{
		unique_ptr<ASTNode> left = std::move(v.back());
		v.pop_back();
		root = make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(left), std::move(root));
	}
	return (root);
}

unique_ptr<ASTNode>	buildAndRight( vector<unique_ptr<ASTNode>>& v )
{
	if (v.empty())
		return (nullptr);
	unique_ptr<ASTNode>	root = std::move(v.back());
	v.pop_back();
	while (!v.empty())
	{
		unique_ptr<ASTNode> left = std::move(v.back());
		v.pop_back();
		root = make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(left), std::move(root));
	}
	return (root);
}

unique_ptr<ASTNode>	transformASTtoCNF( unique_ptr<ASTNode> node )
{
	if (!node)
		return (nullptr);
	if (node->type == NodeType::VARIABLE || (node->type == NodeType::NEGATION && node->left != nullptr && node->left->type == NodeType::VARIABLE))
		return (node);
	if (node->type == NodeType::CONJUNCTION)
	{
		node->left  = transformASTtoCNF(std::move(node->left));
		node->right = transformASTtoCNF(std::move(node->right));
		
		auto partsL = flattenAnd(std::move(node->left));
		auto partsR = flattenAnd(std::move(node->right));
		partsL.insert(partsL.end(), make_move_iterator(partsR.begin()), make_move_iterator(partsR.end()));
		return (buildAndRight(partsL));
	}
	if (node->type == NodeType::DISJUNCTION)
	{
		auto L = transformASTtoCNF(std::move(node->left));
		auto R = transformASTtoCNF(std::move(node->right));

		vector<unique_ptr<ASTNode>>	leftClauses  = flattenAnd(std::move(L));
		vector<unique_ptr<ASTNode>>	rightClauses = flattenAnd(std::move(R));
		vector<unique_ptr<ASTNode>>	newClauses;
		for (const auto& right : rightClauses)
		{
			for (const auto& left : leftClauses)
			{
				auto Ld = flattenOr(left->clone());
				auto Rd = flattenOr(right->clone());
				Ld.insert(Ld.end(), std::make_move_iterator(Rd.begin()), std::make_move_iterator(Rd.end()));
				newClauses.push_back(buildOrRight(Ld));
			}
		}
		if (newClauses.empty())
			return (nullptr);
		unique_ptr<ASTNode>	newRoot = std::move(newClauses.back());
		newClauses.pop_back();
		while (!newClauses.empty())
		{
			auto	left = std::move(newClauses.back());
			newClauses.pop_back();
			newRoot = make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(left), std::move(newRoot));
		}
		return (newRoot);
	}
	return (node);
}

void	printASTFormulaCNF( const string& formula )
{
	string				infix;
	unique_ptr<ASTNode>	ast;

	infix = toInfix(formula);
	ast = toAST(infix);
	ast = eliminateImplications(std::move(ast));
	ast = transformASTtoNNF(std::move(ast));
	ast = transformASTtoCNF(std::move(ast));
	print2D(ast.get());
}

string	conjunctive_normal_form( const string& formula )
{
	unique_ptr<ASTNode> ast;
	string				infix, cnf;

	infix = toInfix(formula);
	ast = toAST(infix);
	ast = eliminateImplications(std::move(ast));
	ast = transformASTtoNNF(std::move(ast));
	ast = transformASTtoCNF(std::move(ast));
	computeToRpn(ast.get(), cnf);
	return (cnf);
}
