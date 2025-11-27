/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:50:47 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/25 18:20:54 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <memory>

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

	ASTNode( NodeType _type, std::string _value, std::unique_ptr<ASTNode> _left, std::unique_ptr<ASTNode> _right ) 
			: type(_type), value(_value), left(std::move(_left)), right(std::move(_right)) {}
	

	std::unique_ptr<ASTNode>	clone() const
	{
		std::unique_ptr<ASTNode> node = std::make_unique<ASTNode>(type, value, nullptr, nullptr);

		if (left)
			node->left = left->clone();
		if (right)
			node->right = right->clone();
		return (node);
	}
};

#endif