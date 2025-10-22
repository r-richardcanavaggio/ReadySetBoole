/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex06.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/22 20:17:31 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main( int argc, char *argv[] )
{
	string	expr;
	string	nnf;

	if (argc != 2)
		return (cerr << "Wrong number of arguments" << endl, 1);
	try 
	{
		expr = argv[1];
		printASTFormula(expr);
		cout << "----------" << endl;
		// nnf = negation_normal_form(expr);
		// printASTFormulaCNF(expr);
		conjunctive_normal_form(expr);
		// cout << nnf << endl;
	}
	catch(const std::exception& e)
	{
		cerr << e.what() << endl;
	}
}
