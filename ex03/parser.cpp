/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 12:05:45 by rrichard          #+#    #+#             */
/*   Updated: 2025/09/22 14:16:27 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

bool	evaluate( const std::string& expression )
{
	std::istringstream	tokens(expression);
	std::string			token;
	std::stack<bool>	stack;

	while (tokens >> token)
	{
		if (token.size() == 1 && (token == "0" || token == "1"))
			stack.push(token == "0" ? false : true);
		else if (token.size() != 1)
			throw std::runtime_error("Error: invalid input.");
		else
		{
			if (token == "!")
			{
				bool operand = stack.top();
				stack.pop();
				stack.push(!operand);
			}
			else
			{
				bool operand1 = stack.top();
				stack.pop();
				bool operand2 = stack.top();
				stack.pop();
				if (token == "&")
					stack.push(operand1 && operand2);
				else if (token == "|")
					stack.push(operand1 || operand2);
				else if (token == "^")
					stack.push(operand1 ^ operand2);
				else if (token == ">")
					stack.push(!operand1 || operand2);
				else if (token == "=")
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
		std::cerr << "Too many arguments.\n";
		return (1);
	}
	try
	{
		bool	result;
		std::string	expression = argv[1];
		result = evaluate(expression);
		std::cout << "Result: " << result << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}