/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AST.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 10:13:03 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/21 13:00:29 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_HPP
# define AST_HPP

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

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
	std::unique_ptr<ASTNode>	left, right;

	ASTNode(NodeType type, std::string value, std::unique_ptr<ASTNode> left = nullptr, std::unique_ptr<ASTNode> right = nullptr)
		: type(type), value(value), left(std::move(left)), right(std::move(right)) {}

};

std::unique_ptr<ASTNode>	parseFormula( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseImplication( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseDisjunction( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseTerm( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parseFactor( std::string::iterator &it, std::string::iterator end );
std::unique_ptr<ASTNode>	parsePrimary( std::string::iterator &it, std::string::iterator end );

#endif //AST_HPP