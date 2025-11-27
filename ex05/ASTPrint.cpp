/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTPrint.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:13:02 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/25 18:25:03 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

void	print2D( const ASTNode* node, const std::string& prefix, bool isLeft, bool isRoot )
{
	if (!node)
		return ;

	if (node->right)
		print2D(node->right.get(), prefix + (isRoot ? "" : (isLeft ? "│   " : "    ")), false, false);

	std::cout << prefix;
	if (!isRoot)
		std::cout << (isLeft ? "└── " : "┌── ");
	std::cout << node->value << std::endl;

	if (node->left)
		print2D(node->left.get(), prefix + (isRoot ? "" : (isLeft ? "    " : "│   ")), true, false);
}

void	printASTFormula( const std::string& formula )
{
	std::string					infix;
	std::unique_ptr<ASTNode>	ast;

	infix = toInfix(formula);
	ast = toAST(infix);
	print2D(ast.get());
}

void	printASTFormulaNNF( const std::string& formula )
{
	std::string					infix;
	std::unique_ptr<ASTNode>	ast;

	infix = toInfix(formula);
	ast = toAST(infix);
	ast = eliminateImplications(std::move(ast));
	ast = transformASTtoNNF(std::move(ast));
	print2D(ast.get());
}