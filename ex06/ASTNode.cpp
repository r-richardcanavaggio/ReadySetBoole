/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTNode.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:53:44 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/22 17:40:25 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ASTNode.hpp"

ASTNode::ASTNode( NodeType _type, string _value, std::unique_ptr<ASTNode> _left = nullptr, unique_ptr<ASTNode> _right = nullptr ) 
			: type(_type), value(_value), left(std::move(_left)), right(std::move(_right)) {}

unique_ptr<ASTNode>	ASTNode::clone() const
{
	unique_ptr<ASTNode> node;

	node = make_unique<ASTNode>(type, value, nullptr, nullptr);
	if (left)
		node->left = left->clone();
	if (right)
		node->right = right->clone();
	return (node);
}
