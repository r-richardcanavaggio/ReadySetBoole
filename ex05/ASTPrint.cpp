/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ASTPrint.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 16:13:02 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/24 16:14:43 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"

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