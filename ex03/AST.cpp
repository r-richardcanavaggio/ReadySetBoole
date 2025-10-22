/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:09:29 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/21 10:11:22 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

using namespace std;

std::unique_ptr<ASTNode>	parsePrimary( string::iterator &it, string::iterator end )
{
	if (it == end)
		throw std::runtime_error("Unexpected end of input");
	if (*it == '(')
	{
		it++;
		auto expr = parseFormula(it, end);
		if (it == end || *it != ')')
			throw std::runtime_error("Expected ')'");
		it++;
		return (expr);
	}
	else if (*it == '0' || *it == '1')
	{
		NodeType	type = (*it == '0') ? NodeType::FALSE : NodeType::TRUE;
		string	value(1, *it);
		it++;
		return (std::make_unique<ASTNode>(type, value, nullptr, nullptr));
	}
	else if (isalpha(*it))
	{
		string	value(1, *it);
		it++;
		return (std::make_unique<ASTNode>(NodeType::VARIABLE, value, nullptr, nullptr));
	}
	else
		throw std::runtime_error("Unexpected token: " + string(1, *it));
}

std::unique_ptr<ASTNode>	parseFactor( string::iterator &it, string::iterator end )
{
	if (it != end && *it == '!')
	{
		it++;
		auto operand = parseFactor(it, end);
		return (std::make_unique<ASTNode>(NodeType::NEGATION, "!", std::move(operand), nullptr));
	}
	else
		return (parsePrimary(it, end));
}

std::unique_ptr<ASTNode>	parseTerm( string::iterator &it, string::iterator end )
{
	auto	left = parseFactor(it, end);
	while (it != end && *it == '&')
	{
		it++;
		auto	right = parseFactor(it, end);
		left = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "&", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseDisjunction( string::iterator &it, string::iterator end )
{
	auto left = parseTerm(it, end);
	while (it != end && (*it == '^' || *it == '|'))
	{
		char op = *it;
		it++;
		auto	right = parseTerm(it, end);
		if (op == '^')
			left = std::make_unique<ASTNode>(NodeType::EXCLUSIVE_DISJUNCTION, "^", std::move(left), std::move(right));
		else // op == '|'
			left = std::make_unique<ASTNode>(NodeType::DISJUNCTION, "|", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseImplication( string::iterator &it, string::iterator end )
{
	auto	left = parseDisjunction(it, end);
	while (it != end && *it == '>')
	{
		it++;
		auto	right = parseDisjunction(it, end);
		left = std::make_unique<ASTNode>(NodeType::MATERIAL_CONDITION, ">", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseFormula( string::iterator &it, string::iterator end )
{
	auto	left = parseImplication(it, end);
	while (it != end && *it == '=')
	{
		it++;
		auto	right = parseImplication(it, end);
		left = std::make_unique<ASTNode>(NodeType::LOGICAL_EQUIVALENCE, "=", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parse( string& input )
{
	input.erase(remove(input.begin(), input.end(), ' '), input.end());
	string::iterator	it = input.begin();
	auto				ast = parseFormula(it, input.end());

	if (it != input.end())
		throw std::runtime_error(std::string("Unexpected token at end of input: ") + string(1, *it));
	return (ast);
}

int GLOBALSPACE = 5;
void	print2D( ASTNode* node, int space )
{
	if (!node)	return ;
	
	space += GLOBALSPACE;
	print2D(node->right.get(), space);
	cout << endl;
	string	s(space - GLOBALSPACE, ' ');
	cout << s << node->value << endl;
	print2D(node->left.get(), space);
}

int	main( void )
{
	std::string	input;
	std::cout << "Enter a propositional logic formula: ";
	std::getline(std::cin, input);

	try
	{
		if (input == "manual")
		{
			std::unique_ptr<ASTNode> A = std::make_unique<ASTNode>(NodeType::VARIABLE, "A", nullptr, nullptr);
			std::unique_ptr<ASTNode> B = std::make_unique<ASTNode>(NodeType::VARIABLE, "B", nullptr, nullptr);
			std::unique_ptr<ASTNode> C = std::make_unique<ASTNode>(NodeType::VARIABLE, "C", nullptr, nullptr);
			std::unique_ptr<ASTNode> I = std::make_unique<ASTNode>(NodeType::VARIABLE, "I", nullptr, nullptr);
			std::unique_ptr<ASTNode> J = std::make_unique<ASTNode>(NodeType::VARIABLE, "J", nullptr, nullptr);
			std::unique_ptr<ASTNode> H = std::make_unique<ASTNode>(NodeType::VARIABLE, "H", nullptr, nullptr);
			std::unique_ptr<ASTNode> K = std::make_unique<ASTNode>(NodeType::VARIABLE, "K", nullptr, nullptr);
			std::unique_ptr<ASTNode> L = std::make_unique<ASTNode>(NodeType::VARIABLE, "L", nullptr, nullptr);
			std::unique_ptr<ASTNode> P = std::make_unique<ASTNode>(NodeType::VARIABLE, "P", nullptr, nullptr);
			
			std::unique_ptr<ASTNode> F = std::make_unique<ASTNode>(NodeType::VARIABLE, "F", std::move(H), std::move(P));
			std::unique_ptr<ASTNode> G = std::make_unique<ASTNode>(NodeType::VARIABLE, "G", std::move(F), nullptr);
			std::unique_ptr<ASTNode> D = std::make_unique<ASTNode>(NodeType::VARIABLE, "D", std::move(B), std::move(C));
			std::unique_ptr<ASTNode> E = std::make_unique<ASTNode>(NodeType::VARIABLE, "E", std::move(A), std::move(D));
			std::unique_ptr<ASTNode> M = std::make_unique<ASTNode>(NodeType::VARIABLE, "M", std::move(I), std::move(J));
			std::unique_ptr<ASTNode> N = std::make_unique<ASTNode>(NodeType::VARIABLE, "N", std::move(G), std::move(M));
	
			std::unique_ptr<ASTNode> root = std::make_unique<ASTNode>(NodeType::VARIABLE, "O", std::move(E), std::move(N));
			print2D(root.get(), GLOBALSPACE);
		}
		else
		{
			auto	ast = parse(input);
			print2D(ast.get(), GLOBALSPACE);
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (0);
}