/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex06.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/26 14:58:10 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include "AST.hpp"

int main( int argc, char *argv[] )
{
	std::string	expr;
	std::string	cnf;

	if (argc != 2)
		return (std::cerr << "Wrong number of arguments" << std::endl, 1);
	try 
	{
		expr = argv[1];
		printASTFormula(expr);
		std::cout << "----------" << std::endl;
		printASTFormulaCNF(expr);
		cnf = conjunctive_normal_form(expr);
		std::cout << cnf << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
