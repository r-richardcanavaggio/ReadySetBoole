/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 15:09:29 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/12 17:44:50 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

enum class NodeType
{
	FALSE,
	TRUE,
	NEGATION,
	CONJUNCTION,
	DISJUNCTION,
	EXCLUSIVE_DISJUNCTION,
	MATERIAL_CONDITION,
	LOGICAL_EQUIVALENCE,
	VARIABLE,
};

struct ASTNode
{
	NodeType					type;
	std::string					value;
	std::unique_ptr<ASTNode>	left;
	std::unique_ptr<ASTNode>	right;

	ASTNode(NodeType type, std::string value, std::unique_ptr<ASTNode> left = nullptr, std::unique_ptr<ASTNode> right = nullptr)
		: type(type), value(value), left(std::move(left)), right(std::move(right)) {}
};

std::unique_ptr<ASTNode>	parseFormula( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end );
std::unique_ptr<ASTNode>	parseImplication( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end );
std::unique_ptr<ASTNode>	parseDisjunction( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end );
std::unique_ptr<ASTNode>	parseTerm( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end );
std::unique_ptr<ASTNode>	parseFactor( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end );
std::unique_ptr<ASTNode>	parsePrimary( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end );

std::unique_ptr<ASTNode>	parseFormula( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto	left = parseImplication(it, end);
	while (it != end && *it == "=")
	{
		it++;
		auto	right = parseImplication(it, end);
		left = std::make_unique<ASTNode>(NodeType::LOGICAL_EQUIVALENCE, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseImplication( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto	left = parseDisjunction(it, end);
	while (it != end && *it == ">")
	{
		it++;
		auto	right = parseDisjunction(it, end);
		left = std::make_unique<ASTNode>(NodeType::MATERIAL_CONDITION, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseDisjunction( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto left = parseTerm(it, end);
	while (it != end && (*it == "^" || *it == "|"))
	{
		std::string op = *it;
		it++;
		auto	right = parseTerm(it, end);
		if (op == "^")
			left = std::make_unique<ASTNode>(NodeType::EXCLUSIVE_DISJUNCTION, "", std::move(left), std::move(right));
		else // op == "|"
			left = std::make_unique<ASTNode>(NodeType::DISJUNCTION, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseTerm( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	auto	left = parseFactor(it, end);
	while (it != end && *it == "&")
	{
		it++;
		auto	right = parseFactor(it, end);
		left = std::make_unique<ASTNode>(NodeType::CONJUNCTION, "", std::move(left), std::move(right));
	}
	return (left);
}

std::unique_ptr<ASTNode>	parseFactor( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	if (it != end && *it == "!")
	{
		it++;
		auto operand = parseFactor(it, end);
		return (std::make_unique<ASTNode>(NodeType::NEGATION, "", std::move(operand), nullptr));
	}
	else
		return (parsePrimary(it, end));
}

std::unique_ptr<ASTNode>	parsePrimary( std::vector<std::string>::iterator &it, std::vector<std::string>::iterator end )
{
	if (it == end)
		throw std::runtime_error("Unexpected end of input");
	if (*it == "(")
	{
		it++;
		auto expr = parseFormula(it, end);
		if (it == end || *it != ")")
			throw std::runtime_error("Expected ')'");
		it++;
		return (expr);
	}
	else if (*it == "0" || *it == "1")
	{
		NodeType	type = (*it == "0") ? NodeType::FALSE : NodeType::TRUE;
		std::string	value = *it;
		it++;
		return (std::make_unique<ASTNode>(type, std::move(value), nullptr, nullptr));
	}
	else if (it->size() == 1 && isalpha((*it)[0]))
	{
		std::string	value = *it;
		it++;
		return (std::make_unique<ASTNode>(NodeType::VARIABLE, std::move(value), nullptr, nullptr));
	}
	else
		throw std::runtime_error("Unexpected token: " + *it);
}

std::vector<std::string>	tokenize( const std::string& input )
{
	std::vector<std::string>	tokens;
	std::istringstream			iss(input);
	std::string					token;

	while (iss >> token)
		tokens.push_back(token);
	return (tokens);
}

std::unique_ptr<ASTNode>	parse( const std::string& input )
{
	auto	tokens = tokenize(input);
	auto	it = tokens.begin();
	auto	ast = parseFormula(it, tokens.end());

	if (it != tokens.end())
		throw std::runtime_error("Unexpected token at end of input: " + *it);
	return (ast);
}

std::string	getNodeSymbol( const ASTNode* node )
{
	switch (node->type)
	{
		case NodeType::FALSE: return "0";
		case NodeType::TRUE: return "1";
		case NodeType::NEGATION: return "!";
		case NodeType::CONJUNCTION: return "&";
		case NodeType::DISJUNCTION: return "|";
		case NodeType::EXCLUSIVE_DISJUNCTION: return "^";
		case NodeType::MATERIAL_CONDITION: return "->";
		case NodeType::LOGICAL_EQUIVALENCE: return "<->";
		case NodeType::VARIABLE: return node->value;
		default: return "?";
	}
}

void	printAST( const ASTNode* node, const std::string& prefix = "", bool isLeft = true )
{
	if (!node)
		return ;
	if (node->right)
		printAST(node->right.get(), prefix + (isLeft ? "|   " : "    "), false);
	std::cout << prefix << (isLeft ? "`-- " : ",-- ") << getNodeSymbol(node) << std::endl;
	if (node->left)
		printAST(node->left.get(), prefix + (isLeft ? "    " : "|   "), true);
}

int	main( void )
{
	std::string	input;
	std::cout << "Enter a propositional logic formula: ";
	std::getline(std::cin, input);

	try
	{
		auto	ast = parse(input);
		printAST(ast.get());
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
	}
	return (0);
}