/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex03.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:05:45 by rrichard          #+#    #+#             */
/*   Updated: 2025/10/26 18:20:53 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

bool	eval_formula( std::string& str )
{
	std::stack<bool>	stack;

	for (auto c : str)
	{
		if (c == '0' || c == '1')
			stack.push(c == '0' ? false : true);
		else
		{
			if (c == '!')
			{
				if (stack.empty())
					throw std::runtime_error("Error: missing operand for '!");
				bool operand = stack.top();
				stack.pop();
				stack.push(!operand);
			}
			else
			{
				if (stack.size() < 2)
					throw std::runtime_error("Error: missing operand for binary operator.");
				bool operand1 = stack.top();
				stack.pop();
				bool operand2 = stack.top();
				stack.pop();
				if (c == '&')
					stack.push(operand1 && operand2);
				else if (c == '|')
					stack.push(operand1 || operand2);
				else if (c == '^')
					stack.push(operand1 ^ operand2);
				else if (c == '>')
					stack.push(!operand1 || operand2);
				else if (c == '=')
					stack.push(operand1 == operand2);
				else
					throw std::runtime_error("Error: operator not supported");
			}
		}
	}
	if (stack.size() != 1)
		throw std::runtime_error("Error: wrong expression.");
	return (stack.top());
}

int	main( int argc, char *argv[] )
{
	if (argc != 2)
	{
		std::cerr << "Error: wrong number of arguments.\n";
		return (1);
	}
	try
	{
		std::string	input = argv[1];
		std::cout << "Result: " << eval_formula(input) << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}