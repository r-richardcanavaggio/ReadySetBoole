/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ex09.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrichard <rrichard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:15:49 by rrichard          #+#    #+#             */
/*   Updated: 2025/11/27 13:18:04 by rrichard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <iostream>
#include <string>
#include <stack>
#include <memory>
#include <sstream>

std::vector<int>	manual_intersection( const std::vector<int>& v1, const std::vector<int>& v2 );
std::vector<int>	manual_union( const std::vector<int>& v1, const std::vector<int>& v2 );
std::vector<int>	manual_difference( const std::vector<int>& v1, const std::vector<int>& v2 );
std::vector<int>	manual_symmetric_difference( const std::vector<int>& v1, const std::vector<int>& v2 );

std::vector<int>	eval_set( const std::string& formula, std::vector<std::vector<int>>& sets )
{
	std::stack<std::vector<int>>	_stack;
	std::vector<int>				univers;

	if (sets.empty())
		throw std::runtime_error("Error: No set provided");
	for (auto& s : sets)
	{
		std::sort(s.begin(), s.end());
		auto last = std::unique(s.begin(), s.end());
		s.erase(last, s.end());
	}
	univers = sets[0];
	for (size_t i = 1; i < sets.size(); i++)
		univers = manual_union(univers, sets[i]);
	for (auto& c : formula)
	{
		if (c >= 'A' && c <= 'Z')
		{
			int index = c - 'A';
			if (index < 0 || index >= static_cast<int>(sets.size()))
				throw std::runtime_error("Error: variable out of range for provided sets");
			_stack.push(sets[index]);
		}
		else
		{
			if (c == '!')
			{
				if (_stack.empty())
					throw std::runtime_error("Error: not enough operands for operator.");
				std::vector<int>	operand = _stack.top(); _stack.pop();
				_stack.push(manual_difference(univers, operand));
			}
			else
			{
				if (_stack.size() < 2)
					throw std::runtime_error("Error: not enough operands for operator.");
				std::vector<int> rhs = _stack.top(); _stack.pop();
				std::vector<int> lhs = _stack.top(); _stack.pop();
				if (c == '&')
					_stack.push(manual_intersection(lhs, rhs));
				else if (c == '|')
					_stack.push(manual_union(lhs, rhs));
				else if (c == '^')
					_stack.push(manual_symmetric_difference(lhs, rhs));
				else if (c == '>')
				{
					std::vector<int> complement;

					complement = manual_difference(univers, lhs);
					_stack.push(manual_union(rhs, complement));
				}
				else if (c == '=')
				{
					std::vector<int> symdiff = manual_symmetric_difference(lhs, rhs);
					_stack.push(manual_difference(univers, symdiff));
				}
				else
					throw std::runtime_error("Error: operator not supported");
			}
		}
	}
	if (_stack.size() != 1)
		throw std::runtime_error("Error: wrong expression.");
	return (_stack.top());
}

int main( void )
{
	std::vector<std::vector<int>> sets = {{1, 2, 3}, {2,4}, {0,3}};
	try
	{
		std::string	input;
		std::cout << "A=1 2 3\nB=2 4\nC=0 3\nEnter propositional formula: ";
		getline(std::cin, input);
		while (input != "EXIT")
		{
			std::vector<int>	results = eval_set(input, sets);
			std::cout << "[";
			for (std::vector<int>::iterator it = results.begin(); it < results.end(); it++)
			{
				if (it != results.end() - 1)
					std::cout << *it << ", ";
				else
					std::cout << *it;
			}
			std::cout << "]";
			std::cout << std::endl << "Enter propositional formula: ";
			getline(std::cin, input);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (0);
}
