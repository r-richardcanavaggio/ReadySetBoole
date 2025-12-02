/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex07.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/12/02 13:59:31 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <string>
#include <stack>

bool	evaluate( const std::string& formula )
{
	std::stack<bool>	_stack;

	for (const auto& c : formula)
	{
		if (c == '0' || c == '1')
			_stack.push(c == '0' ? false : true);
		else
		{
			if (c == '!')
			{
				if (_stack.empty())
					throw std::runtime_error("Error: missing operand for '!");
				bool operand = _stack.top();
				_stack.pop();
				_stack.push(!operand);
			}
			else
			{
				if (_stack.size() < 2)
					throw std::runtime_error("Error: missing operand for binary operator");
				bool right = _stack.top(); _stack.pop();
				bool left = _stack.top(); _stack.pop();
				if (c == '&')
					_stack.push(left && right);
				else if (c == '|')
					_stack.push(left || right);
				else if (c == '^')
					_stack.push(left ^ right);
				else if (c == '>')
					_stack.push(!left || right);
				else if (c == '=')
					_stack.push(left == right);
				else
					throw std::runtime_error("Error: operator not supported");
			}
		}
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: wrong expression.");
	return (_stack.top());
}

std::vector<char> buildVarList( const std::string& input )
{
	std::vector<char>	res;

	for (const auto& c : input)
		if (c >= 'A' && c <= 'Z')
			res.push_back(c);
	if (res.size() < 2)
		throw std::runtime_error("Error: not enough variables");
	return (res);
}

bool	sat( const std::string& formula )
{
	std::vector<char>	vars = buildVarList(formula);
	size_t				n = vars.size();
	size_t				limit = 1U << n;

	for (size_t i = 0; i < limit; i++)
	{
		std::string expr = formula;
		for (size_t j = 0; j < n; j++)
		{
			bool	isTrue = (i >> (n - 1 - j)) & 1U;
			char	var = vars[j];
			char	val = isTrue ? '1' : '0';
			for (size_t k = 0; k < expr.length(); k++)
			{
				if (expr[k] == var)
					expr[k] = val;
			}
		}
		if (evaluate(expr) == true)
			return (true);
	}
	return (false);
}

int main( int argc, char *argv[] )
{
	if (argc != 2)
		return (std::cerr << "Wrong number of arguments" << std::endl, 1);
	try 
	{
		std::string input = argv[1];
		std::cout << std::boolalpha << sat(input) << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
