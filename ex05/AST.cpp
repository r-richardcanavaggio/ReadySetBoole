/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 15:46:09 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/27 17:03:56 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

std::unique_ptr<ASTNode>	parsePrimary( std::string::iterator &it, std::string::iterator end )
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
		std::string	value(1, *it);
		it++;
		return (std::make_unique<ASTNode>(type, value, nullptr, nullptr));
	}
	else if (isalpha(*it))
	{
		std::string	value(1, *it);
		it++;
		return (std::make_unique<ASTNode>(NodeType::VARIABLE, value, nullptr, nullptr));
	}
	else
		throw std::runtime_error("Unexpected token: " + std::string(1, *it));
}

std::unique_ptr<ASTNode>	parseFactor( std::string::iterator &it, std::string::iterator end )
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

std::unique_ptr<ASTNode>	parseTerm( std::string::iterator &it, std::string::iterator end )
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

std::unique_ptr<ASTNode>	parseDisjunction( std::string::iterator &it, std::string::iterator end )
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

std::unique_ptr<ASTNode>	parseImplication( std::string::iterator &it, std::string::iterator end )
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

std::unique_ptr<ASTNode>	parseFormula( std::string::iterator &it, std::string::iterator end )
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
