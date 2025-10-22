/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTNode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:53:44 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/21 17:03:32 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASTNode.hpp"

ASTNode::ASTNode( NodeType _type, string _value, std::unique_ptr<ASTNode> _left = nullptr, unique_ptr<ASTNode> _right = nullptr ) 
			: type(_type), value(_value), left(std::move(_left)), right(std::move(_right)) {}

ASTNode::~ASTNode() {}

ASTNode::ASTNode( const ASTNode& other ) : type(other.type), value(other.value)
{
	if (other.left)
		left = other.left->clone();
	if (other.right)
		right = other.right->clone();
}

unique_ptr<ASTNode>	ASTNode::clone() const
{
	auto n = make_unique<ASTNode>(type, value, nullptr, nullptr);
	if (left)
		n->left = left->clone();
	if (right)
		n->right = right->clone();
	return (n);
}

ASTNode&	ASTNode::operator=( const ASTNode& other )
{
	if (this == &other)
		return (*this);
	ASTNode	tmp(other);
	std::swap(type, tmp.type);
	std::swap(value, tmp.value);
	std::swap(left, tmp.left);
	std::swap(right, tmp.right);
	return (*this);
}
