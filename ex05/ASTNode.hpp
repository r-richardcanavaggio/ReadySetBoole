/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTNode.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 16:50:47 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/22 13:42:46 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASTNODE_HPP
#define ASTNODE_HPP

#include <string>
#include <memory>

using namespace std;

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

	ASTNode( NodeType, string, unique_ptr<ASTNode>, unique_ptr<ASTNode> );
	ASTNode( const ASTNode& );
	ASTNode& operator=( const ASTNode& );
	~ASTNode();

	unique_ptr<ASTNode> clone() const;
};

#define GLOBALSPACE 5

unique_ptr<ASTNode>			eliminateImplications( unique_ptr<ASTNode> node );

#endif