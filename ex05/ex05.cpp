/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex05.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/22 15:34:37 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main( int argc, char *argv[] )
{
	if (argc != 2)
		return (cerr << "Wrong number of arguments" << endl, 1);
	try 
	{
		string	expr = argv[1];
		RPN		calc(expr);
	
		calc.printASTFormula();
		cout << "----------" << endl;
		string	nnfformula = calc.toNNF();
		calc.printASTFormula();
		cout << nnfformula << endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
}
