/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex05.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/24 16:18:19 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "AST.hpp"
#include "RPN.hpp"

int main( int argc, char *argv[] )
{
	std::string	expr;
	std::string	nnf;

	if (argc != 2)
		return (std::cerr << "Wrong number of arguments" << std::endl, 1);
	try 
	{
		expr = argv[1];
		printASTFormula(expr);
		std::cout << "----------" << std::endl;
		nnf = negation_normal_form(expr);
		printASTFormulaNNF(expr);
		std::cout << nnf << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
